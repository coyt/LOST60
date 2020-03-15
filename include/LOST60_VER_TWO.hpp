#ifndef LOST60_VER_TWO_H
#define LOST60_VER_TWO_H

/*
 
*/


//misc definitions
#define MYLED 6
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


#endif