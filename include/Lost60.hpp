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
#include <SparkFunBQ27441.h>


//********** START CORE Configuration Selection **********
// COMMENT OUT ALL EXCEPT ONE OF THE FOLLOWING:

//#define HELLO_WORLD      //this blinks the "user" LED on the PCB for testing that all is working.
#define LOST60_VER_ONE    //this selects code for version 1 of the LOST60 with much less features - used for legacy systems
//#define LOST60_VER_TWO    //this selects code for version 2 of the LOST60 with significant updates in features.

//********** END CORE Configuration Selection **********


//Selector for config file based on version of keyboard
#if defined(HELLO_WORLD)
  #include "HELLO_WORLD.hpp"
#elif defined(LOST60_VER_ONE)
  #include "LOST60_VER_ONE.hpp"
#else
  #include "LOST60_VER_TWO.hpp"
#endif 

 
//objects
extern BLEDis bledis;
extern BLEHidAdafruit blehid;
extern BLEBas blebas;


//global variables and data exchange across rtos threads
extern volatile bool global_flag_SleepModeOne; 


//misc definitions
#define STATE_ONE_TIME 2000
#define STATE_TWO_TIME 50
#define LOOP_STACK_SZ   (512*3)

#define SLEEP_MODE_ONE_TIMEOUT 60000*5 //1 min * num minutes

#define MAX_PRPH_CONNECTION 2

//Set BATTERY_CAPACITY to the design capacity of your battery.
#define BATTERY_CAPACITY 1600 // e.g. 850mAh battery

#endif