#ifndef LOST60_H
#define LOST60_H

/*
  lost60.h

  external declarations for shared items in the bluetooth keyboard system.
  Each cpp file should include this.
*/


//dependencies
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <bluefruit.h>


//********** START CORE Configuration Selection **********
// COMMENT OUT ALL EXCEPT ONE OF THE FOLLOWING:

#define HELLO_WORLD      //this blinks the "user" LED on the PCB for testing that all is working.
//#define LOST60_VER_ONE    //this selects code for version 1 of the LOST60 with much less features - used for legacy systems
//#define LOST60_VER_TWO    //this selects code for version 2 of the LOST60 with significant updates in features.

//********** END CORE Configuration Selection **********


//Selector for config file based on version of keyboard
#ifdef HELLO_WORLD
  #include "HELLO_WORLD.hpp"
#elif LOST60_VER_ONE
  #include "LOST60_VER_ONE.hpp"
#else LOST60_VER_TWO
  #include "LOST60_VER_TWO.hpp"
#endif 


//objects
extern BLEDis bledis;
extern BLEHidAdafruit blehid;


//misc definitions
#define STATE_ONE_TIME 2000
#define STATE_TWO_TIME 50
#define LOOP_STACK_SZ   (512*3)




#endif