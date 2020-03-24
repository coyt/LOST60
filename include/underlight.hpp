#ifndef UNDERLIGHT_H
#define UNDERLIGHT_H

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
// task to control led matrix underlighting
//
void underlight_task (void* arg);

void setupUnderLightingLeds(void);
void animationOne(void);
void animationTwo(void);
void animationThree(void);
void underlightOff(void);
void underlightOn(void);
void loadNewShiftRegisterData(uint16_t localData);

#endif