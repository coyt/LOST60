#ifndef LOST60_VER_TWO_H
#define LOST60_VER_TWO_H

/*

LOST60 Version Two.hpp

This header defines compile time settings and pin mappings for various versions of the LOST60

LOST60 Version Two.hpp uses Adafruit "feather_nrf52840_express" bootloader and pinouts as
defined in https://github.com/adafruit/Adafruit_nRF52_Arduino/blob/master/variants/feather_nrf52840_express/variant.cpp

Pin mappings themselves are completely different between LOST60 PCB revisions. New mapping and additonal features are defined in this file. 
*/

//definitions for Bluetooth 
#define BLUETOOTH_NAME "Lost60 Keyboard V2"
#define BLUETOOTH_MODEL "Lost60 V2.0"
#define BLUETOOTH_MANUFACTURER "Barringer Engineering LLC"



//misc definitions
#define MYLED           6       //D6 is on P0.07

//QSPI Pins
//already defined and wired as the default in Variant.cpp

//Underlighting "neopixels"
#define PIXEL_PIN       14      //D14 is A0 on P0.04   // Digital IO pin connected to the NeoPixels. 
#define PIXEL_COUNT     12      //Number of NeoPixel
#define PIXEL_ENABLE    2       //D2  is P0.10 (NFC2) //enable load switch to turn neopixels on 

//for switch backlighting leds
#define ULED_CLK        25      // D25 is P0.13
#define ULED_SIN        26      // D26 is P0.14  
#define ULED_LATCH      24      // D24 is on P0.15 
#define ULED_BLANK      8       // D8 is on P0.16

#define ULED_ROW1       20      // D20 is A6 on P0.29
#define ULED_ROW2       9       // D9 on P0.26
#define ULED_ROW3       45      // D45 on P1.12
#define ULED_ROW4       16      // D16 is P0.30 (A2)
#define ULED_ROW5       21      // D21 is P0.31 (A7, ARef)

//Speaker Pins
#define SPEAKER_A       18      // D18 is P0.02 (A4)
#define SPEAKER_B       19      // D19 is P0.03 (A5)

//Rotary Encoder Pins
#define ROTARY_A        15      // D15 is P0.05 (A1)
#define ROTARY_B        11      // D11 is P0.06

//I2C Bus Pins
#define SCL_PIN         23      // D23 is P0.11 (SCL)
#define SDA_PIN         22      // D22 is P0.12 (SDA)

//switch MATRIX Shift Register Pins
//Dammit adafruit - thanks for not defining these - now we've gotta edit a custom variant.cpp file.
//To make this work, Just UNCOMMENT Adafruit's section called "// The remaining pins are not connected (per schematic)" 
//By doing this, we add all to those pins in order to the digital pin map and they should be usable as gpio

//Variant File should have this seciton uncommented:
//Additionally, we start at D34 and incriment because adafruit stopped at "D33 is P0.09"
//ALSO YOU MUST MAKE #define PINS_COUNT           (46) IN THE variant.h file equal to 46!!!!!!!

// The remaining pins are not connected (per schematic)
// 33,      //D34 // P1.01 is not connected per schematic
// 35,      //D35 // P1.03 is not connected per schematic
// 36,      //D36 // P1.04 is not connected per schematic
// 37,      //D37 // P1.05 is not connected per schematic
// 38,      //D38 // P1.06 is not connected per schematic
// 39,      //D39 // P1.07 is not connected per schematic
// 43,      //D40 // P1.11 is not connected per schematic
// 44,      //D41 // P1.12 is not connected per schematic
// 45,      //D42 // P1.13 is not connected per schematic
// 46,      //D43 // P1.14 is not connected per schematic

#define SER_DATA        42      // D42 is P1.13
#define SER_CLK         43      // D43 is P1.14
#define SER_LATCH       3       // D3  is P1.15

// ********** SWITCH MATRIX KEYBOARD MAPPINGS AS FOLLOWS **********

//rows will be digitalInputs and each will be read 
#define ROW_1   40      //row 1 = P1.11 = D40 (based on our hack above)
#define ROW_2   33      //row 2 = P0.09 = D33
#define ROW_3   10      //row 3 = P0.27 = 10
#define ROW_4   17      //row 4 = P0.28 = 17
#define ROW_5   12      //row 5 = P0.08 = 12 

#endif