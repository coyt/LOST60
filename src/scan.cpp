// ******************************
//
// Initial Test Code for Lost60 Bluetooth Keyboard
//
// Written by Coyt Barringer - Summer 2019
//
// ******************************

//core config and dependencies in here
#include "scan.hpp"

//select board version
//for version one
#if defined(LOST60_VER_ONE)
    //for version two
    //cols will be digitalOutputs and driven HIGH (1) while each row is scanned
    uint8_t cols[] = {COL_1,COL_2,COL_3,COL_4,COL_5,COL_6,COL_7,COL_8,COL_9,COL_10,COL_11,COL_12,COL_13,COL_14};
#elif defined(LOST60_VER_TWO)
  //function definitions
#else
  //nothing
#endif

//rows will be digitalInputs and each will be read 
uint8_t rows[] = {ROW_1,ROW_2,ROW_3,ROW_4,ROW_5};

//uint8_t* layerOf(uint8_t);

//
bool global_flag_SleepModeOne = false;


uint8_t colCount = 14; //sizeof(cols);
uint8_t rowCount = 5; //sizeof(rows);


const int length = sizeof(colCount*rowCount);

//misc variables
volatile unsigned long localTimerOne = millis();


//keyboard state variables
bool keyPressedPreviously = false;
//hid mapping layer zero (normal default layer)
uint8_t layerMap0[] = 
{ 
//COl1
HID_KEY_ESCAPE, //normally HID_KEY_GRAVE - I like escape here instead
HID_KEY_TAB,
HID_KEY_CAPS_LOCK,
HID_KEY_SHIFT_LEFT,
HID_KEY_CONTROL_LEFT,

//COL2
HID_KEY_1,
HID_KEY_Q,
HID_KEY_A,
HID_KEY_NONE, //left ISO extra key - not used so make it none
HID_KEY_GUI_LEFT,

//COL3
HID_KEY_2,
HID_KEY_W,
HID_KEY_S,
HID_KEY_Z,
HID_KEY_ALT_LEFT,

//COL4
HID_KEY_3,
HID_KEY_E,
HID_KEY_D,
HID_KEY_X,
HID_KEY_NONE, //holder for empty spot in matrix

//COL5
HID_KEY_4,
HID_KEY_R,
HID_KEY_F,
HID_KEY_C,
HID_KEY_NONE, //holder for empty spot in matrix

//COL6
HID_KEY_5,
HID_KEY_T,
HID_KEY_G,
HID_KEY_V,
HID_KEY_SPACE,

//COL7
HID_KEY_6,
HID_KEY_Y,
HID_KEY_H,
HID_KEY_B,
HID_KEY_NONE, //holder for empty spot in matrix

//COL8
HID_KEY_7,
HID_KEY_U,
HID_KEY_J,
HID_KEY_N,
HID_KEY_NONE, //holder for empty spot in matrix

//COL9
HID_KEY_8,
HID_KEY_I,
HID_KEY_K,
HID_KEY_M,
HID_KEY_NONE, //holder for empty spot in matrix

//COL10
HID_KEY_9,
HID_KEY_O,
HID_KEY_L,
HID_KEY_COMMA,
HID_KEY_ESCAPE, //extra key after backspace - not used so make it none //  THIS IS ROTARY ENCODER!!!!!!

//COL11
HID_KEY_0,
HID_KEY_P,
HID_KEY_SEMICOLON,
HID_KEY_PERIOD,
HID_KEY_ARROW_LEFT,//HID_KEY_ALT_RIGHT, //mod4

//COL12
HID_KEY_MINUS,
HID_KEY_BRACKET_LEFT,
HID_KEY_APOSTROPHE,
HID_KEY_SLASH,
HID_KEY_ARROW_UP, //HID_KEY_GUI_RIGHT, //mod5

//COL13
HID_KEY_EQUAL,
HID_KEY_BRACKET_RIGHT,
HID_KEY_NONE, //right ISO extra key - not used so make it none
HID_KEY_NONE, //right "FN" extra key - not used so make it none
HID_KEY_ARROW_DOWN, //HID_KEY_APPLICATION, //mod6 "menu" / fn key is HID code 0x65

//COL14
HID_KEY_BACKSPACE,
HID_KEY_BACKSLASH,
HID_KEY_RETURN, //enter
HID_KEY_SHIFT_RIGHT,
HID_KEY_ARROW_RIGHT//HID_KEY_CONTROL_RIGHT //mod7
};

//hid mapping layer one (second layer)
uint8_t layerMap1[] = 
{ 
//COl1
HID_KEY_TAB,//HID_KEY_ESCAPE, 
HID_KEY_TAB,
HID_KEY_CAPS_LOCK,
HID_KEY_SHIFT_LEFT,
HID_KEY_CONTROL_LEFT,

//COL2
HID_KEY_1,
HID_KEY_Q,
HID_KEY_A,
HID_KEY_NONE, //left ISO extra key - not used so make it none
HID_KEY_GUI_LEFT,

//COL3
HID_KEY_2,
HID_KEY_W,
HID_KEY_S,
HID_KEY_Z,
HID_KEY_ALT_LEFT,

//COL4
HID_KEY_3,
HID_KEY_E,
HID_KEY_D,
HID_KEY_X,
HID_KEY_NONE, //holder for empty spot in matrix

//COL5
HID_KEY_4,
HID_KEY_R,
HID_KEY_F,
HID_KEY_C,
HID_KEY_NONE, //holder for empty spot in matrix

//COL6
HID_KEY_5,
HID_KEY_T,
HID_KEY_G,
HID_KEY_V,
HID_KEY_SPACE,

//COL7
HID_KEY_6,
HID_KEY_Y,
HID_KEY_H,
HID_KEY_B,
HID_KEY_NONE, //holder for empty spot in matrix

//COL8
HID_KEY_7,
HID_KEY_U,
HID_KEY_J,
HID_KEY_N,
HID_KEY_NONE, //holder for empty spot in matrix

//COL9
HID_KEY_8,
HID_KEY_I,
HID_KEY_K,
HID_KEY_M,
HID_KEY_NONE, //holder for empty spot in matrix

//COL10
HID_KEY_9,
HID_KEY_O,
HID_KEY_L,
HID_KEY_COMMA,
HID_KEY_NONE, //extra key after backspace - not used so make it none

//COL11
HID_KEY_0,
HID_KEY_P,
HID_KEY_SEMICOLON,
HID_KEY_PERIOD,
HID_KEY_ARROW_LEFT,//HID_KEY_ALT_RIGHT, //mod4

//COL12
HID_KEY_MINUS,
HID_KEY_BRACKET_LEFT,
HID_KEY_APOSTROPHE,
HID_KEY_SLASH,
HID_KEY_ARROW_UP, //HID_KEY_GUI_RIGHT, //mod5

//COL13
HID_KEY_EQUAL,
HID_KEY_BRACKET_RIGHT,
HID_KEY_NONE, //right ISO extra key - not used so make it none
HID_KEY_NONE, //right "FN" extra key - not used so make it none
HID_KEY_ARROW_DOWN, //HID_KEY_APPLICATION, //mod6 "menu" / fn key is HID code 0x65

//COL14
HID_KEY_BACKSPACE,
HID_KEY_BACKSLASH,
HID_KEY_RETURN, //enter
HID_KEY_SHIFT_RIGHT,
HID_KEY_ARROW_RIGHT//HID_KEY_CONTROL_RIGHT //mod7
};


//ISR functions
void calledOnAnyPress(){
  breakdownLowPowerModeOne();
  localTimerOne = millis();
  global_flag_SleepModeOne = false;

}


//variables for 
//#define KEYBOARD_SLEEP_TIME 60000 //60s 
//bool usbHIDEnabled = false;
//bool bleHIDEnabled = false;


// Modifier keys, only take cares of Shift
// ATL, CTRL, CMD keys are left for user excersie.
// uint8_t shiftPin = 11;


//
// task to control keyboard matrix scanning
//
void scan_task (void* arg)
{

    (void) arg;

    //call this task setup function once
    scanSetup();

    //run task forever
    while (1)
    {
        //yield(); // yield run usb background task
        scanLoop();
    }

}


//
// this thread's setup function - will run once at start of thread
//
void scanSetup(){

    //setup row I/O as inputs with pulldowns
    for(uint8_t i=0; i < rowCount; i++){
        pinMode(rows[i], INPUT_PULLDOWN);
    }

    //select board version
    //for version one
    #if defined(LOST60_VER_ONE)

          
        //setup column I/O as outputs
        for(uint8_t i=0; i < colCount; i++){
            pinMode(cols[i], OUTPUT);
        }

        //setup column I/O as LOW (0) before program starts
        for(uint8_t i=0; i < colCount; i++){
            digitalWrite(cols[i], LOW);
        }

        //rows will be digitalInputs and each will be read 
        //uint8_t rows[] = {ROW_1,ROW_2,ROW_3,ROW_4,ROW_5};

        //cols will be digitalOutputs and driven HIGH (1) while each row is scanned
        //uint8_t cols[] = {COL_1,COL_2,COL_3,COL_4,COL_5,COL_6,COL_7,COL_8,COL_9,COL_10,COL_11,COL_12,COL_13,COL_14};


    //for version two
    #elif defined(LOST60_VER_TWO)
      
        //setup shift registers
        pinMode(SER_LATCH, OUTPUT);
        pinMode(SER_DATA, OUTPUT);
        pinMode(SER_CLK, OUTPUT);

        digitalWrite(SER_LATCH, HIGH);
        digitalWrite(SER_DATA, LOW);
        digitalWrite(SER_CLK, LOW);



    #else
      //nothing

    #endif

}


bool modifierKeyPressedPreviously = false;


//
// this thread's loop function - will run in loops forever once started
//
void scanLoop(){

  //if our timer runs down with no keybaord activity, we need to goto sleep and tell other threads to do the same
  if(millis() - localTimerOne >= SLEEP_MODE_ONE_TIMEOUT){

    //setup interrupt based low power mode waiting
    setupLowPowerModeOne();

    global_flag_SleepModeOne = true;
    delay(1000); //give RTOS more sleep / low power time
  }

  //if global flag indicates "Awake" we should be operating as normal
  if(!global_flag_SleepModeOne){
    //we feed localTimerOne in the scanKeyMatrix function as long as valid key presses are received - so we stay awake
    scanKeyMatrix();
    delay(10); //matrix poll interval
  }

}


//iterate through keybaord matrix, handle modifiers, and send key presses
void scanKeyMatrix(){

      //-------------- Scan Pin Array and send report ---------------------
    bool anyKeyPressed = false;


    uint8_t modifier = 0;
    uint8_t count=0;
    uint8_t keycode[6] = { 0 };


    //outside loop for iterating through each column
    for(uint8_t i=0; i < colCount; i++){

      #if defined(LOST60_VER_ONE)
        //pullup the I/O line for the column
        digitalWrite(cols[i], HIGH);
      #else
        //if we're using the shift registers for V2.0:
        shiftOutToMakeColumnHigh(i);
      #endif

      //inside loop for iterating through each row
      //scan normal key and send report
      for(uint8_t j=0; j < rowCount; j++){

        //check each key - if one is pressed we enter here
        if ( 1 == digitalRead(rows[j]) ){

          //feed the local timer
          localTimerOne = millis();

          /*
          //HERE we check for modifier keys properly!
          //shift key left
          if((i == LEFT_SHIFT_COL_POSITION) && (j == LEFT_SHIFT_ROW_POSITION)){
              modifier |= KEYBOARD_MODIFIER_LEFTSHIFT;
          }
          //shift key right
          if((i == RIGHT_SHIFT_COL_POSITION) && (j == RIGHT_SHIFT_ROW_POSITION)){
              modifier |= KEYBOARD_MODIFIER_RIGHTSHIFT;
          }
          //control key left
          if((i == LEFT_CONTROL_COL_POSITION) && (j == LEFT_CONTROL_ROW_POSITION)){
              modifier |= KEYBOARD_MODIFIER_LEFTCTRL;
          }
          //windows key left
          if((i == LEFT_WINDOWS_COL_POSITION) && (j == LEFT_WINDOWS_ROW_POSITION)){
              modifier |= KEYBOARD_MODIFIER_LEFTGUI;
          }
          //alt key left
          if((i == LEFT_ALT_COL_POSITION) && (j == LEFT_ALT_ROW_POSITION)){
              modifier |= KEYBOARD_MODIFIER_LEFTALT;
          }
          */

          uint8_t temp = layerMap0[(i*5)+j]; //activeLayers[k][(i*5)+j];

                    //HERE we check for modifier keys properly!
          if(temp == HID_KEY_SHIFT_LEFT){
            modifier |= KEYBOARD_MODIFIER_LEFTSHIFT;
          }

          if(temp == HID_KEY_SHIFT_RIGHT){
            modifier |= KEYBOARD_MODIFIER_RIGHTSHIFT;
          }

          if(temp == HID_KEY_CONTROL_LEFT){
            modifier |= KEYBOARD_MODIFIER_LEFTCTRL;
          }

          if(temp == HID_KEY_CONTROL_RIGHT){
            modifier |= KEYBOARD_MODIFIER_RIGHTCTRL;
          }

          if(temp == HID_KEY_GUI_LEFT){
            modifier |= KEYBOARD_MODIFIER_LEFTGUI;
          }

          if(temp == HID_KEY_GUI_RIGHT){
            modifier |= KEYBOARD_MODIFIER_RIGHTGUI;
          }

          if(temp == HID_KEY_ALT_LEFT){
            modifier |= KEYBOARD_MODIFIER_LEFTALT;
          }

          if(temp == HID_KEY_ALT_RIGHT){
            modifier |= KEYBOARD_MODIFIER_RIGHTALT;
          }
            
          keycode[count++] = temp;

          //6 is max keycode per report
          if ( count == 6)
          {
            //tud_hid_keyboard_keycode(modifier, keycode);
            blehid.keyboardReport(modifier, keycode);

            // reset report
            count = 0;
            memset(keycode, 0, 6);
          }


          //used later
          anyKeyPressed = true;
          keyPressedPreviously = true;
        }

       //check anything else?

      }

      #if defined(LOST60_VER_ONE)
        //pullup the I/O line for the column
        digitalWrite(cols[i], LOW);
      #else
        //if we're using the shift registers for V2.0:
        shiftOutToMakeColumnLow(i);
      #endif

    }


    // Send any remaining keys (not accumulated up to 6)
    if ( count )
    {
      //tud_hid_keyboard_keycode(modifier, keycode);
      blehid.keyboardReport(modifier, keycode);
    }



    // Send All-zero report to indicate there is no keys pressed
    // Most of the time, it is, though we don't need to send zero report
    // every loop(), only a key is pressed in previous loop() 
    if ( !anyKeyPressed && keyPressedPreviously )
    {
      keyPressedPreviously = false;
      //tud_hid_keyboard_key_release();
      blehid.keyRelease();
    }  

}


//setup for low power mode one
void setupLowPowerModeOne(){

  #if defined(LOST60_VER_ONE)
    //make all column rows high
    for(uint8_t i=0; i < colCount; i++){
        digitalWrite(cols[i], HIGH);
    }
  #else
      //make all column shift registers high - we want all buttons to wait for any one button press
    shiftOutToMakeAllColumnsHigh();
  #endif

  delay(10);

  //attach interrupts for rows
  attachInterrupt(ROW_1, calledOnAnyPress,RISING);
  attachInterrupt(ROW_2, calledOnAnyPress,RISING);
  attachInterrupt(ROW_3, calledOnAnyPress,RISING);
  attachInterrupt(ROW_4, calledOnAnyPress,RISING);
  attachInterrupt(ROW_5, calledOnAnyPress,RISING);

  delay(10);

}

void breakdownLowPowerModeOne(){

  #if defined(LOST60_VER_ONE)
    //make all column rows low again
    for(uint8_t i=0; i < colCount; i++){
        digitalWrite(cols[i], LOW);
    }
  #else
    //make all column shift registers low - back to normal
    shiftOutToMakeColumnLow(1);
  #endif

  //attach interrupts for rows
  detachInterrupt(ROW_1);
  detachInterrupt(ROW_2);
  detachInterrupt(ROW_3);
  detachInterrupt(ROW_4);
  detachInterrupt(ROW_5);

  //setup row I/O as inputs with pulldowns
  for(uint8_t i=0; i < rowCount; i++){
      pinMode(rows[i], INPUT_PULLDOWN);
  }

}


#if defined(LOST60_VER_TWO)

    void shiftOutToMakeColumnHigh(int column){

        uint16_t shiftValue = pow(2, column);
        uint8_t shiftValLow = shiftValue & 0xFF;
        uint8_t shiftValHigh = (shiftValue & 0xFF00) >> 8;

        digitalWrite(SER_LATCH, LOW);
        shiftOut(SER_DATA, SER_CLK, MSBFIRST, shiftValHigh); //first number to 255
        shiftOut(SER_DATA, SER_CLK, MSBFIRST, shiftValLow); //second number to 255
        digitalWrite(SER_LATCH, HIGH);
    }


    void shiftOutToMakeColumnLow(int column){

        uint8_t shiftValLow = 0x00;
        uint8_t shiftValHigh = 0x00;

        digitalWrite(SER_LATCH, LOW);
        shiftOut(SER_DATA, SER_CLK, LSBFIRST, shiftValHigh); //first number to 255
        shiftOut(SER_DATA, SER_CLK, LSBFIRST, shiftValLow); //second number to 255
        digitalWrite(SER_LATCH, HIGH);
    }


    void shiftOutToMakeAllColumnsHigh(){

        uint8_t shiftValLow = 0xFF;
        uint8_t shiftValHigh = 0xFF;

        digitalWrite(SER_LATCH, LOW);
        shiftOut(SER_DATA, SER_CLK, LSBFIRST, shiftValHigh); //first number to 255
        shiftOut(SER_DATA, SER_CLK, LSBFIRST, shiftValLow); //second number to 255
        digitalWrite(SER_LATCH, HIGH);
    }
#endif 