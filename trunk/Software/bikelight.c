/*!\file bikelight.c
 * R0.0: First working version.
 * R0.1: Changing documentation to Doxygen style
 * R0.2: Solving bug: Sometimes 15 times larger power consumption when OFF.
 *			When waking from OFF-mode, when it was too bright, the case 2 of timerA interrupt vector was never run.
 *			So the analog comparator was not disabled before going to sleep.  This is corrected now and the current
 *			consumption has been reduced from 14.2µA@3V to 200nA@3V.
 *			
 *
 * Programming the low-fuse: avrdude -p t13 -P usb -c avrispmkII -U lfuse:w:0x6B:m
 * This makes the CPU run at 16kHz. -> ISP freq. MAX=16kHz/4=4kHz.-> 250us period 
 * -> use at least 300us SCK-period for programming.
 *
 * The device could no longer be programmed with the avrispmkII.  The stk200 had to be used:
 *		"avrdude -p t13 -c stk200 -U lfuse:w:0x6A:m -i 500" to restore the fuse to its original value.
 *
 * Another solution is to use avrdude in terminal mode:
 * 		avrdude -c avrispmkII -P usb -p t13 -F -u -t
 *		then type: sck 300
 *		then type: quit
 * The PC will memorize that sck value.
 * After that, you can use the following command again. DON'T USE the -i OPTION!!!!
 * avrdude -p t13 -P usb    -c avrispmkII    -U flash:w:main.hex 
 
 * Light detection should only be done when button is pushed!
 */

#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

//!All possible light modes for the three LEDs
typedef enum{
	STATE_OFF,											//!<LEDs OFF
	STATE_FLASHING1,									//!<LEDs ON duty cycle=25%, f=10Hz
	STATE_FLASHING2,									//!<LEDs ON duty cycle=75%, F=10Hz
	STATE_CONT_ON										//!<LEDs continuously ON
} OPERATING_STATES;

static void setState(void);

static volatile OPERATING_STATES yState=STATE_OFF;		//!<Holds the current state of the state machine
static volatile OPERATING_STATES yNextState=STATE_OFF;	//!<Holds the next state of the state machine
static volatile uint8_t bTooBright=0;					//!<Signals that it's still to bright outside to turn the
														//		LEDs on.

/*!Main function.  This one does it all.
 *This function initializes the necessary registers and after that enters an infinite loop.
 *When the LEDs are off, the MCU enters deep sleep mode to save power.
 *The MCU will be waked when the user presses a button, because this will cause a pin change interrupt.
 */
int main(void){
	//Disable clock dividing now, because it causes a lot of problems with programming 
	//	timeout of the debugger.
	//Divide the main oscillator by 256 => fcpu = 16kHz/256 = 62.5Hz
	//CLKPR=(1<<CLKPCE);
	//CLKPR=(1<<CLKPS3);
	
	PCMSK|=(1<<PCINT0);								//Enable pin change interrupts on pin PORTB.0
	GIMSK|=(1<<PCIE);									//Enable pin change interrupts
	TCCR0A|=(1<<WGM00)|(1<<WGM01);						//Fast PWM-mode, TOP of counter = OCRA.
	TCCR0B|=(1<<WGM02)|(1<<CS01);						//Prescaler /8 -> 2kHz clock
	OCR0A=199;
	
	TIMSK0|=(1<<OCIE0A);								//Enable interrupt on OCR0A match.

	ACSR|=(1<<ACBG)|(1<<ACIS1)|(1<<ACIS0);			
														//Trigger interrupt on rising edge.
														//Set bandgap voltage as IN+ of the analog comparator
	ADMUX|=(1<<MUX1)|(1<<MUX0);						//Set ADC3 as IN- of the analog comparator
	ACSR|=(1<<ACD);									//Disable analog comparator

	setState();
	sei();
	
	while(1){
		if(yState==STATE_OFF && yNextState==STATE_OFF){
			set_sleep_mode(SLEEP_MODE_PWR_DOWN);		//Prepare going to sleep
			sleep_enable();
			sleep_cpu();
			sleep_disable();							//MCU continues here after waking from sleep.
		}
	}
	return 0;
}//main


/*!Pin change interrupt
 *Every time the user presses the button, this interrupt routine will be executed.
 *The state machine will go to a next state, which means it will change the lighting mode of the LED.
 */ 
ISR(PCINT0_vect){
	TCNT0=0;
	switch(yState){
		case STATE_OFF:
			yNextState=STATE_FLASHING1;
			break;
		case STATE_FLASHING1:
			yNextState=STATE_FLASHING2;
			break;
		case STATE_FLASHING2:
			yNextState=STATE_CONT_ON;
			break;
		case STATE_CONT_ON:
			yNextState=STATE_OFF;
	}
}//PCINT0_vect

/*!Timer complete interrupt
 *Every time the timer reaches a complete period, this interrupt routine will be executed.  The flashing frequency
 *of the LEDs is the inverse of this frequency.
 *If the user has pressed the button, it means that he/she wants to move the state machine to a next state.  Before
 *changing the state, there will be a check first to see if it's dark enough.  This prevents "bad" people from 
 *turning on the light at daytime.
 *When it's dark enough, the next light mode will be active, otherwise the state machine goes to the OFF mode (all
 *LEDs off).
 */
ISR(TIM0_COMPA_vect){
	static uint8_t delay=0;
	static uint16_t ontime=0;
	
	if(yState!=yNextState){
		switch(delay){
			case 0:
											//Disable LEDs
				DDRB&=~(1<<DDB1);				//pB.1 = input
				PORTB&=~(1<<PB1);				//Disable pull-ups on pB.1
											//Discharge capacitor
				DDRB|=(1<<DDB3);				//pB.3 = output
				PORTB|=(1<<PB3);				//pB.3 = high
				break;
			case 1:
											//Charge capacitor through LDR
				DDRB&=~(1<<DDB3);				//pB.3 = input
				PORTB&=~(1<<PB3);				//Disable pull-up on pB.3
											//Make MCU accept analog comparator interrupts
				ADCSRB|=(1<<ACME);				//Select analog comparator input as pin ADC3
				bTooBright=0;					//Reset the indicator for checking if it's too bright.
				ACSR&=~(1<<ACD);				//Enable analog comparator
				ACSR|=(1<<ACIE);				//Enable interrupts on analog comparator
				break;
			case 2:
				ACSR|=(1<<ACD);				//Disable analog comparator
				ACSR&=~(1<<ACIE);				//Disable interrupts on analog comparator
				if(bTooBright)yNextState=STATE_OFF;
				yState=yNextState;
				setState();
				break;
		}
		if(delay<2){
			delay++;
		}else{
			delay=0;
		}
		ontime=0;
	}else{
		if(ontime<36000){
			ontime++;
		}else{
			ontime=0;
			yNextState=STATE_OFF;
		}
	}
}//TIM0_COMPA_vect

/*!Analog comparator interrupt
 *This is used for checking the light level.  If it's too bright, the LDR resistance will drop sharply.
 *A smaller resistance of the LDR will make the capacitor charge faster.  If the analog reference level
 *is reached to soon (because it's too bright), then the state machine will move to the OFF-state.
 */
ISR(ANA_COMP_vect){
	//See what the value of counter 0 is.
	//If it's too small, this means it's too bright to put the light on.
	if(TCNT0<120){
		bTooBright=1;		
	}
}//ANA_COMP_vect

/*!The necessary registers for every single LED lighting mode are set here.
 */
void setState(){
	switch(yState){
		case STATE_FLASHING1:
			DDRB|=(1<<DDB1);							//Configure PORTB.1 as output
			TCCR0A|=(1<<COM0B1)|(1<<COM0B0);			//Set OC0B (=portB.1 pin) on compare match, 
														//clear OC0B when timer reaches TOP.
			OCR0B=149;
			break;
		case STATE_FLASHING2:
			DDRB|=(1<<DDB1);							//Configure PORTB.1 as output
			TCCR0A|=(1<<COM0B1)|(1<<COM0B0);			//Set OC0B (=portB.1 pin) on compare match, 
														//clear OC0B when timer reaches TOP.
			OCR0B=49;
			break;
		case STATE_CONT_ON:
			DDRB|=(1<<DDB1);							//Configure PORTB.1 as output
			TCCR0A&=~((1<<COM0B1)|(1<<COM0B0));		//Disconnect PORTB.1 from timer0
			PORTB|=(1<<PB1);							//Set PORTB.1 high
			break;
		case STATE_OFF:
			DDRB&=~((1<<DDB1)|(1<<DDB3));				//Set pB.1 and pB.3 as input
			PORTB&=~((1<<PB1)|(1<<PB3));				//Disable pull-ups on pB.1 and pB.3
			PORTB|=(1<<PB0)|(1<<PB2)|(1<<PB4)|(1<<PB5);
														//other pins: enable pull-ups 
														//	(to minimise power consumption)
			break;
	}
}//setState