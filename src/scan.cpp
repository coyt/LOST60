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


uint8_t colCount = sizeof(cols);
uint8_t rowCount = sizeof(rows);


const int length = sizeof(colCount*rowCount);
Vector<uint8_t[length]> activeLayers;


//keyboard state variables
bool keyPressedPreviously = false;
bool modifierPressedPreviously = false;


//variables for 
//#define KEYBOARD_SLEEP_TIME 60000 //60s 
//bool usbHIDEnabled = false;
//bool bleHIDEnabled = false;
//bool keyboardActive = false;
bool layerChange = false;

enum layer{LAYER_0,LAYER_1,LAYER_2,LAYER_3,LAYER_4,LAYER_5,LAYER_6,LAYER_7,LAYER_8,LAYER_9,LAYER_10,LAYER_11,LAYER_12,LAYER_13,LAYER_14,LAYER_15};
enum layer myLayer; //myLayer is a variable of type layer 

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

    /*
    //-------------- Handle layer switching ---------------------

    //check if layer has changed
    if(layerChange){

        //if layer has changed switch hidmap to that layer
        switch(myLayer){
            case LAYER_0:
            layerMapPointer = layerMap0;
            break;

            case LAYER_1:
            layerMapPointer = layerMap1;
            break;

            case LAYER_2:
            //layerMapPointer = layerMap2;
            break;

            case LAYER_3:
            //layerMapPointer = layerMap3;
            break;

            case LAYER_4:
            //layerMapPointer = layerMap4;
            break;

            case LAYER_5:
            //layerMapPointer = layerMap5;
            break;

            case LAYER_6:
            //layerMapPointer = layerMap6;
            break;

            case LAYER_7:
            //layerMapPointer = layerMap7;
            break;

            case LAYER_8:
            //layerMapPointer = layerMap8;
            break;

            case LAYER_9:
            //layerMapPointer = layerMap9;
            break;

            case LAYER_10:
            //layerMapPointer = layerMap10;
            break;

            case LAYER_11:
            //layerMapPointer = layerMap11;
            break;

            case LAYER_12:
            //layerMapPointer = layerMap12;
            break;

            case LAYER_13:
            //layerMapPointer = layerMap13;
            break;

            case LAYER_14:
            //layerMapPointer = layerMap14;
            break;

            case LAYER_15:
            //layerMapPointer = layerMap15;
            break;

            default:
            break;
        }

    }
    */

    //-------------- Scan Pin Array and send report ---------------------
    bool anyKeyPressed = false;


    uint8_t modifier = 0;
    uint8_t count=0;
    uint8_t keycode[6] = { 0 };

    
    // scan modifier key (only SHIFT), user implement ATL, CTRL, CMD if needed
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


          //check if its a special key within the current keymap: i.e. change layers:
          //maybe change to switch statement to handle output of checkLaerMdKey fic.... different switch statements handle toggle, momentary, etc...
          if(checkIfLayerModifierKey(i, j)){

            //we got a layer modifier key!

            //handle toggle
            //user has pressed modifier key - we must either go up a layer if it's the first time this mod key has been pressed, or go back if the key has already been pressed
            if(modifierKeyPressedPreviously){

              //remove the layer - IE revert to previous layer because button was released
              activeLayers.pop_back();

            }else{

              //add the layer that was selected by user as the next in the array of layers
              activeLayers.push_back(layerDeterminedByModKey());

            }



            //determine if the layer we're on right now is the one associated with the modifer


            //set state variables:
            //let the next loop know that a modifier key was pressed in this loop
            //modifierPressedPreviously = true;

          }
          //this occurs when a key was pressed BUT the key was not a modifier - thus it's a real key to send
          else{

            //set state variables:
            //reset the modifier key flag so on the next loop we know that the modifer key was not pressed
            //modifierKeyPressedPreviously = false;

            //if pin is active (high), add its hid code to key report
            //keycode[count++] = layerMapPointer[(i*5)+j];
            //layerMapPointer = activeLayers[0];
            //keycode[count++] = activeLayers[layerMapPointer[(i*5)+j]];
            //layerMapPointer = activeLayers[0]; //layerMapPointer = layerMap0;
            //keycode[count++] = activeLayers[0][(i*5)+j];

            //for each layer selected and added by user inside our layer vector, we:
            //maybe off by 1 error?
            for(int k = activeLayers.size(); k >= 0; k-- ){

              //check if the value on the "layer under test" is NULL - if it's not, we've foud a real key
              if(activeLayers[k][(i*5)+j] != NULLVAL){

                //a real key was found! add it to the keycode buffer!
                keycode[count++] = activeLayers[k][(i*5)+j];

                break;

              }

              //a NULL key was found - NOT a real key OR a Modifier key - because we already checked for modifier keys. This means we fallthrough to the next layer. 
              //Base layer must not have nulls - if it does we need to add a catch here
              //else{
              //  
              //}

            }



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


//
// task to control keyboard matrix scanning
//
bool checkIfLayerModifierKey(uint8_t indexI, uint8_t indexJ){

  //activeLayers[k][(i*5)+j]

  

  return false;
}


//
// task to control keyboard matrix scanning
//
void handlelayerModifierKey(){


}