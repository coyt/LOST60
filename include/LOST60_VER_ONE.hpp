#ifndef LOST60_VER_ONE_H
#define LOST60_VER_ONE_H

/*

LOST60 Version One.hpp

This header defines compile time settings and pin mappings for various versions of the LOST60

LOST60 Version One.hpp uses Adafruit "feather_nrf52840_express" bootloader and pinouts as
defined in https://github.com/adafruit/Adafruit_nRF52_Arduino/blob/master/variants/feather_nrf52840_express/variant.cpp

*/

//definitions for Bluetooth 
#define BLUETOOTH_NAME "Lost60 Keyboard V1"
#define BLUETOOTH_MODEL "Lost60 V1.0"
#define BLUETOOTH_MANUFACTURER "Barringer Engineering LLC"

//misc definitions
#define MYLED 6 //D6 is on P0.07
#define VBATPIN A4 //D18 is A4 on P0.02 where voltage divider is connected
#define PIXEL_PIN   14  // Digital IO pin connected to the NeoPixels. //D14 is A0 on P0.04
#define PIXEL_COUNT 12  // Number of NeoPixel
#define PIXEL_ENABLE 16 //enable load switch to turn neopixels on //D16 is A2 on P0.30

//for switch underlighting leds
#define ULED_CLK    29 // D29 is on P0.17
#define ULED_SIN    26 // D26 is on P0.14 
#define ULED_LATCH  24 // D24 is on P0.15 
#define ULED_BLANK  8  // D8 is on P0.16

#define ULED_ROW1   20 // D20 is A6 on P0.29
#define ULED_ROW2   9  // D9 on P0.26
#define ULED_ROW3   45 // D45 on P1.12
#define ULED_ROW4   31 // D31 is on P0.23
#define ULED_ROW5   0  // D0 is on P0.25


// ********** SWITCH MATRIX KEYBOARD MAPPINGS AS FOLLOWS **********

//rows will be digitalInputs and each will be read 
#define ROW_1   1       //good //row 1 = P0.24 = 
#define ROW_2   32      //good //row 2 = P0.21 = 32
#define ROW_3   10      //row 3 = P0.27 = 10
#define ROW_4   17      //row 4 = P0.28 = 17
#define ROW_5   28      //row 5 = P0.20 = 28

//cols will be digitalOutputs and driven HIGH (1) while each row is scanned
#define COL_1   37//41      //col 1 = P1.05 = 41
#define COL_2   38 //42      //col 2 = P1.06 = 42
#define COL_3   39 //43      //col 3 = P1.07 = 43
#define COL_4   5       //col 4 = P1.08 = 5
#define COL_5   13      //col 5 = P1.09 = 13
#define COL_6   30      //col 6 = P0.22 = 30
#define COL_7   43 //47      //col 7 = P1.14 = 47
#define COL_8   42 //46      //col 8 = P1.13 = 46
#define COL_9   4       //col 9 = P1.10 = 4
#define COL_10  3       //col 10 = P1.15 = 3
#define COL_11  34 //38      //col 11 = P1.01 = 38
#define COL_12  36 //40      //col 12 = P1.04 = 40
#define COL_13  35 //39      //col 13 = P1.03 = 39
#define COL_14  7       //col 14 = P1.02 = 7





#endif