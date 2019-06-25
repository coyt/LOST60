#ifndef BACKLIGHT_H
#define BACKLIGHT_H

// ******************************
//
// Initial Test Code for Lost60 Bluetooth Keyboard
//
// Written by Coyt Barringer - Summer 2019
//
// ******************************

//core config and dependencies in here
#include "Lost60.hpp"



//
// task to control RGB backlighting
//
void backlight_task (void* arg);


/*
 * function to setup backlighing on startup - run once
 */
void setupBacklighting();

void colorWipe(uint32_t color, int wait);
void theaterChase(uint32_t color, int wait);
void rainbow(int wait);
void theaterChaseRainbow(int wait);
void backlightAnimationOne(void);
void backlightAnimationTwo(void);
void backlightAnimationThree(void);


#endif