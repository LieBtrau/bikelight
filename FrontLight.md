# Housing #
## Requirements ##
  * hold two LEDs and the regulating electronics.
  * made out of metal in order to transfer heat away from the LEDs
## Choice ##
  * CAMDEN-BOSS - BIM 5002/12 - DIECAST BOX, can be ordered at  [Farnell](http://be.farnell.com/camden-boss/bim-5002-12/box-diecast/dp/1171660?Ntt=117-1660)

# Electronics #
[pilom.com](http://pilom.com/BicycleElectronics/DynamoCircuits.htm) is the only site I found on the internet that has such a detailed analysis of dynamos and schematics for powering LEDs.
Circuit 7 has been made with two LEDs and powered by a bottle dynamo.
As it is mentioned on the site, remove the two zener diodes from your dynamo to get maximum power from your dynamo.

# LEDs #
  * Avago ASMT-MY22-NMNW0
  * Cree Inc. MCEEZW-A1-0000-0000K040H
  * Cree Inc. XMLAWT-00-0000-0000T6053
  * Luxeon LXML-PWC2

# LED star modules (added 2013) #
  * [Digikey 1125-1163-ND](http://www.digikey.be/search/en?x=0&y=0&lang=en&site=be&keywords=1125-1163-ND)
  * [Conrad 182774 - 89](http://www.conrad.be/ce/nl/product/182774/Cree-XLamp-MX-6-op-Star-print-CREE-MX6AWT-A1-STAR-000E51-Koud-wit-130-lm-120-)

The Luxeon LED had been chosen because of highest efficiency.  The package is hard to solder by hand.  So it's much better to opt for this LED presoldered on a star-PCB.  This can be found at:
  * [Future Electronics](http://www.futureelectronics.com/en/Technologies/Product.aspx?ProductID=REBELSTARESCW200OPULENT9003841)
  * [Luxeon star](http://www.luxeonstar.com/Cool-White-5650K-20mm-Star-Rebel-235-lm-p/mr-wc310-20s.htm)

# Lenses #
The power LED has an opening angle that is too wide.  Too much light is lost at the sides.  So a lens is needed.  Two lenses have been chosen: 18° and 28° opening angle.  Currently the 18° lenses are mounted, but they seem to lose quite some light at the sides.  Maybe that's good to ride winding roads, but on long straight roads, a lot of light is wasted.
The original idea was to mount two different lenses: a wide angle lens for nearby lighting and a narrow lens for lighting further away.  The 18° lens is already wide to my mind, so I won't use the wider 28° lenses.
  * [18° lens](http://be02.rs-online.com/web/p/led-accessories/7240854/)
  * [28° lens](http://be02.rs-online.com/web/p/led-accessories/7107776/)

# Pictures #
## Mounted on the bike ##
[![](http://bikelight.googlecode.com/files/DSC05288s.jpg)](http://bikelight.googlecode.com/files/DSC05288.JPG)
[![](http://bikelight.googlecode.com/files/DSC05286s.jpg)](http://bikelight.googlecode.com/files/DSC05286.JPG)

The wire to the dynamo still had to be shortened. The unit has been aimed so that the brightest spot is three to four meter in front of the bike.

## Front view ##
[![](http://bikelight.googlecode.com/files/DSC05277s.jpg)](http://bikelight.googlecode.com/files/DSC05277.JPG)
[![](http://bikelight.googlecode.com/files/DSC05284s.jpg)](http://bikelight.googlecode.com/files/DSC05284.JPG)
[![](http://bikelight.googlecode.com/files/DSC05283s.jpg)](http://bikelight.googlecode.com/files/DSC05238.JPG)
[![](http://bikelight.googlecode.com/files/DSC05282s.jpg)](http://bikelight.googlecode.com/files/DSC05282.JPG)
  * The protective coating from the front screen is yet to be removed.
  * There's a knot in the wire going to the dynamo, serving as "pull" protection.
  * All components have been soldered on some piece of breadboard.
  * The base of the lenses has been removed, because they didn't fit on the LED star.  Actually you need lenses without such a wide base.  It's quite a hassle to remove it.
  * Lenses have been fixed to the housing using thermal glue.  The adhesion of the glue is not so good to be honest.

## Bottom view ##
[![](http://bikelight.googlecode.com/files/DSC05281s.jpg)](http://bikelight.googlecode.com/files/DSC05281.JPG)
  * Two holes have been drilled in the bottom, just to make sure that the incoming water is drained properly.

## Side and back view ##
[![](http://bikelight.googlecode.com/files/DSC05280s.jpg)](http://bikelight.googlecode.com/files/DSC05280.JPG)
[![](http://bikelight.googlecode.com/files/DSC05279s.jpg)](http://bikelight.googlecode.com/files/DSC05279.JPG)
[![](http://bikelight.googlecode.com/files/DSC05278s.jpg)](http://bikelight.googlecode.com/files/DSC05278.JPG)
  * Out of some piece of scrap metal, an L-piece for the mounting bracket has been made.
  * The mounting bracket comes from a scrap bicycle.  An M6 screw is used to fix it to the L-piece.
  * The L-piece is mounted to the housing with 3 M3-screws.  Two of the screws are also used to hold the LED star on the inside of the housing.
  * Each LED is fixed to the housing with two M3-screws, 20mm apart.
  * Use plastic washers on your screws to mount the LEDs, otherwise you might make a short circuit on the power terminals of the LED-stars.
  * Thermal paste is used on the back side of the LED-stars and on the mounting bracket to improve heat conductivity.
  * Attentive readers may have noticed that there's a hole too many at the back of the housing.  Yeah, it's an error or an extra feature, depending on the way you look at it.  It enables you to see from the back of the housing that the LEDs are on.