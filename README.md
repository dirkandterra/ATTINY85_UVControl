# ATTINY85_UVControl
Controlling UV LED Strip with ATTINY85 and scrap printer part

The attached code is to run on an ATTINY85, but there is a compiler directive that will allow a build to nano for testing (to have serial)

The ATTINY is programmed using Arduino as ISP.  Hook the wires up accordingly:

Arduino====>Attiny====>Attiny85   
D10=======>RESET======>PIN1   
D11=======>MOS======>PIN5   
D12=======>MISO======>PIN6    
D13=======>SCK======>PIN7   
5V========>VCC======>PIN8 (I just power on the board instead of hooking this up)    
GND======>GND=====>PIN4   
(MOSI,MISO,RESET. 5V for programming is supplied from the device, not the programmer.)

**IMPORTANT**
You will also need a fairly large cap (100-1000uF) across the RST of the Arduino pin (+ side if cap is polarized) and GND.  This is to keep the Nano from resetting itself when programming.  You will also have to upload the Arduino as ISP image to the nano first for it to function properly.

Add "https://raw.githubusercontent.com/damellis/attiny/ide-1.6.x-boards-manager/package_damellis_attiny_index.json" to board manager URLs

Board Settings:
Device: ATTINY25/45/85
Internal Clk: 1MHZ
Processor: ATTINY85
Programmer: Arduino as ISP

Program Description:    
The top button starts the UV timer and the side button toggles between the "set time" mode and the "UV light" mode.   
In the "set time mode", each top button press adds 30 sec to the time.  The top button led flashes once for every minute    
and a quick blip if a 30 sec addition is made.  Default is seven minutes.

For LEDs: (AMARS Black LED Lights Strip for Decoration, DC12V 24W 5050 SMD 2M/6.6FT 390nm-405nm), 54 LEDs were used and draw 500mA
