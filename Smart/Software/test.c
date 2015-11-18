#define F_CPU 128000    //define clock as 128KHz
#include <avr/sleep.h>
#include <avr/power.h>
#include <util/delay.h>
#include <avr/interrupt.h>

const uint8_t ON_TIME_CTR=100;//about 50s
uint8_t yDownCounter=0;

ISR(PCINT0_vect)
{
    yDownCounter=ON_TIME_CTR;
}

void setup(void)
{
    //Set main clock as 128KHz.
    CCP=0xD8;   //Configuration change protection register
    CLKMSR|=_BV(CLKMS0);
    CCP=0xD8;   //Configuration change protection register
    CLKMSR&=~(_BV(CLKMS1));
    //Set Peripheral clock equal to system clock
    CCP=0xD8;   //Configuration change protection register
    CLKPSR&= ~(_BV(CLKPS3)|_BV(CLKPS2)|_BV(CLKPS1)|_BV(CLKPS0));
    //Power save settings
    power_timer0_disable();
    //PORTB.0 is a pull-upped input
    PUEB|=_BV(PUEB0);
    //PORTB.1 & PORTB.2 are outputs
    DDRB|= _BV(DDB1) | _BV(DDB2);
    //Enable pin change interrupts
    PCICR|=_BV(PCIE0);
    //Enable pin change interrupt 0
    PCMSK|=_BV(PCINT0);
    //Initialize the down counter
    yDownCounter=ON_TIME_CTR;
    //power ON 5V source
    PORTB&= ~(_BV(DDB2));
    sei();
}

void shortFlash(void){
    PORTB|=_BV(DDB1);
    _delay_ms(50);
    PORTB&= ~(_BV(DDB1));
    _delay_ms(50);
}

void longFlash(void){
    PORTB|=_BV(DDB1);
    _delay_ms(3000);
    PORTB&= ~(_BV(DDB1));
    _delay_ms(50);
}


void loop(void)
{
    if(yDownCounter){
        yDownCounter--;
        shortFlash();
        shortFlash();
        shortFlash();
        _delay_ms(100);
    }else{
        //power OFF 5V source
        PORTB|=_BV(DDB2);
        //Wait for C1 to discharge
        longFlash();
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        cli();
        //PORTB.0 pull-up disable
        PUEB&=~(_BV(PUEB0));
        //PORTB.0 is output (set low)
        DDRB|=_BV(DDB0);
        sleep_enable();
        sleep_cpu();
        //Sleep forever. (until power is cycled, or an unlikely reset condition occurs)
    }
}

int main(void)
{
    setup();
    while(1){
        loop();
    }
    return 0;
}

