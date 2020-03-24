#ifndef AUDIO_H
#define AUDIO_H

// ******************************
//
// Initial Test Code for Lost60 Bluetooth Keyboard
//
// Written by Coyt Barringer - Summer 2019
//
// ******************************

//core config and dependencies in here
#include "Lost60.hpp"
#include <NoodleSynth.h>


//
// task to control RGB backlighting
//
void audio_task (void* arg);

void setupAudio();



#endif