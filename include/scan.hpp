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

//enum modifiers{MO_0, MO_1, MO_2, MO_3, MO_4, MO_5, MO_6, MO_7, MO_8, MO_9, MO_10, MO_11, MO_12, MO_13, MO_14, MO_15};
//enum modifier myModifier;


enum layerTapFunction{MO,TG,TO,TT,DF,OSL};

typedef struct {

    layerTapFunction myLayerFunction;


} QUANTUM_CONFIG;

extern QUANTUM_CONFIG myQuantumSettings;



//
// task to control RGB backlighting
//
void scan_task (void* arg);
void hid_task(void* params);
void scanSetup();
void scanLoop();
bool checkIfLayerModifierKey(uint8_t myKey);
void handleLayerModifierKey();
void shiftOutToMakeColumnHigh(int column);
void shiftOutToMakeColumnLow(int column);




#endif