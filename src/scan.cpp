// ******************************
//
// Initial Test Code for Lost60 Bluetooth Keyboard
//
// Written by Coyt Barringer - Summer 2019
//
// ******************************


//core config and dependencies in here
#include "scan.hpp"



//rows will be digitalInputs and each will be read 
uint8_t rows[] = {1,32,10,17,28};
//row 1 = P0.24 = 1
//row 2 = P0.21 = 32
//row 3 = P0.27 = 10
//row 4 = P0.28 = 17
//row 5 = P0.20 = 28


//cols will be digitalOutputs and driven HIGH (1) while each row is scanned
uint8_t cols[] = {41,42,43,5,13,30,47,46,4,3,38,40,39,7};
//col 1 = P1.05 = 41
//col 2 = P1.06 = 42
//col 3 = P1.07 = 43
//col 4 = P1.08 = 5
//col 5 = P1.09 = 13
//col 6 = P0.22 = 30
//col 7 = P1.14 = 47
//col 8 = P1.13 = 46
//col 9 = P1.10 = 4
//col 10 = P1.15 = 3
//col 11 = P1.01 = 38
//col 12 = P1.04 = 40
//col 13 = P1.03 = 39
//col 14 = P1.02 = 7


//pointer to active HID Layer Map
uint8_t *layerMapPointer;


//hid mapping layer zero (normal default layer)
uint8_t layerMap0[] = 
{ 
//COl1
HID_KEY_ESCAPE, 
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

uint8_t* layerOf(uint8_t);


uint8_t colCount = sizeof(cols);
uint8_t rowCount = sizeof(rows);


const int length = sizeof(colCount*rowCount);
Vector<uint8_t[length]> activeLayers;


//keyboard state variables
bool keyPressedPreviously = false;
//bool modifierPressedPreviously = false;


//variables for 
//#define KEYBOARD_SLEEP_TIME 60000 //60s 
//bool usbHIDEnabled = false;
//bool bleHIDEnabled = false;
//bool keyboardActive = false;
//bool layerChange = false;

//enum layer{LAYER_0,LAYER_1,LAYER_2,LAYER_3,LAYER_4,LAYER_5,LAYER_6,LAYER_7,LAYER_8,LAYER_9,LAYER_10,LAYER_11,LAYER_12,LAYER_13,LAYER_14,LAYER_15};
//enum layer myLayer; //myLayer is a variable of type layer 



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

    //setup column I/O as outputs
    for(uint8_t i=0; i < colCount; i++){
        pinMode(cols[i], OUTPUT);
    }

    //setup column I/O as LOW (0) before program starts
    for(uint8_t i=0; i < colCount; i++){
        digitalWrite(cols[i], LOW);
    }

}

bool modifierKeyPressedPreviously = false;

//
// this thread's loop function - will run in loops forever once started
//
void scanLoop(){


    //-------------- Scan Pin Array and send report ---------------------
    bool anyKeyPressed = false;


    uint8_t modifier = 0;
    uint8_t count=0;
    uint8_t keycode[6] = { 0 };

    
    // scan modifier key (only SHIFT), user implement ALT, CTRL, CMD if needed
    //if ( 1 == digitalRead(shiftPin) )
    //{
    //  modifier |= KEYBOARD_MODIFIER_LEFTSHIFT;
    //}


    //outside loop for iterating through each column
    for(uint8_t i=0; i < colCount; i++){

      //pullup the I/O line for the column
      digitalWrite(cols[i], HIGH);

      //inside loop for iterating through each row
      //scan normal key and send report
      for(uint8_t j=0; j < rowCount; j++){

        //check each key - if one is pressed we enter here
        if ( 1 == digitalRead(rows[j]) ){

          //lookup the key in the current HID table
          for(int k = activeLayers.size(); k >= 0; k-- ){

            //uint8_t key = activeLayers[k][(i*5)+j];

            keycode[count++] = activeLayers[k][(i*5)+j];

          }

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

      //pulldown the I/O line for the column
      digitalWrite(cols[i], LOW);

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
    


    // Poll interval
    delay(10);

}