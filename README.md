# OneWire-Projects-Arduino

## Description
Theses projects exist to try the capabilities of the One-Wire with the use of the Arduino and the switch DS2406.


**DS2406_RW**

This simple project allows the Arduino to search and find the switch with the One-Wire bus.


**DS2406_RW_x2**

This project uses 2 DS2406 to control the blinking of 2 LEDs.
Each LED is controlled by one switch and one PMOS.
The addresses used in this project were found with the use of *DS2406_RW*.


**DS2406_x2_1LED**

This project uses 2 DS2406 to control the blinking 1 LED.
One switch is used at a time to control the LED, and the other do nothing.
If the main switch is disconnected, the second one takes the lead and controls the LED.
The addresses used in this project were found with the use of *DS2406_RW*.

All files use the OneWire library from here : [Link](https://www.arduino.cc/reference/en/libraries/onewire/)
