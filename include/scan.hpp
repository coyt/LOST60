#ifndef SCAN_H
#define SCAN_H


// ******************************
//
// Initial Test Code for Lost60 Bluetooth Keyboard
//
// Written by Coyt Barringer - Summer 2019
//
// ******************************

//core config and dependencies in here
#include "Lost60.hpp"
#include "Vector.h"

//
// task to control RGB backlighting
//
void scan_task (void* arg);
void hid_task(void* params);
void scanSetup();
void scanLoop();
bool checkIfLayerModifierKey(uint8_t indexI, uint8_t indexJ);
void handleLayerModifierKey();

#endif