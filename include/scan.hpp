#pragma once

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
void scan_task (void* arg);
void hid_task(void* params);