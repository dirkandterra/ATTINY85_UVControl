# ATTINY85_UVControl
Controlling UV LED Strip with ATTINY85 and scrap printer part

The attached code is to run on an ATTINY85, but there is a compiler directive that will allow a build to nano for testing (to have serial)

The ATTINY is programmed using Arduino as ISP.  Hook the wires up accordingly. 5V for programming is supplied from the device, not the programmer.
Add "https://raw.githubusercontent.com/damellis/attiny/ide-1.6.x-boards-manager/package_damellis_attiny_index.json" to board manager URLs

Device: ATTINY25/45/85
Internal Clk: 1MHZ
Processor: ATTINY85
Programmer: Arduino as ISP
