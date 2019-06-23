// ******************************
//
// Initial Test Code for Lost60 Bluetooth Keyboard
//
// Written by Coyt Barringer - Summer 2019
// test
//
// ******************************


//core config and dependencies in here
#include "Lost60.hpp"
#include "underlight.hpp"
#include "backlight.hpp"
#include "scan.hpp"
#include "power.hpp"


//objects
BLEDis bledis;
BLEHidAdafruit blehid;


//global variables
bool hasKeyPressed = false;
unsigned long previousTime = millis();
unsigned long previousTimeTwo = millis();
bool keyPressedPreviously = false;
int i = 0;


//function declarations
void set_keyboard_led(uint16_t conn_hdl, uint8_t led_bitmap);
void startAdv(void);
void setupBluetooth(void);
void checkBattery(void);
void colorWipe(uint32_t color, int wait);


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


//hid mapping
uint8_t hidcode[] = 
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


uint8_t colCount = sizeof(cols);
uint8_t rowCount = sizeof(rows);


// Modifier keys, only take cares of Shift
// ATL, CTRL, CMD keys are left for user excersie.
// uint8_t shiftPin = 11;


//setup - this code runs once at startup
void setup() 
{
  //begin uart
  Serial.begin(115200);

  //uncomment line below to force code to wait for uart
  //while ( !Serial ) delay(10);   // for nrf52840 with native usb

  //startup message
  Serial.println("*** Lost60 Bluetooth Keyboard Starting Up... ***");


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


  //setup bluetooth
  setupBluetooth();

  //Set up and start advertising
  startAdv();

  //board debug led
  pinMode(MYLED, OUTPUT);
  

  //**********
  //setup other FreeRTOS tasks
  //**********

  // Create a task for underlighting (under key leds)
  xTaskCreate( underlight_task, "underlight", LOOP_STACK_SZ, NULL, TASK_PRIO_LOW, NULL);

  // Create a task for backlighting (rgb neopixels on bottom)
  xTaskCreate( backlight_task, "backlight", LOOP_STACK_SZ, NULL, TASK_PRIO_LOW, NULL);

  // Create a task for keyboard matrix scanning
  //xTaskCreate( scan_task, "scan", LOOP_STACK_SZ, NULL, TASK_PRIO_LOW, NULL);

  //xTaskCreate( hid_task, "hid", 128, NULL, configMAX_PRIORITIES-2, NULL);

}


//this code loops forever
void loop() 
{

   
  
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

      //check each key
      if ( 1 == digitalRead(rows[j]) ){
        // if pin is active (high), add its hid code to key report
        keycode[count++] = hidcode[(i*5)+j];

        // 6 is max keycode per report
        if ( count == 6)
        {
          blehid.keyboardReport(modifier, keycode);

          // reset report
          count = 0;
          memset(keycode, 0, 6);
        }

        // used later
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
    blehid.keyboardReport(modifier, keycode);
  }


  // Send All-zero report to indicate there is no keys pressed
  // Most of the time, it is, though we don't need to send zero report
  // every loop(), only a key is pressed in previous loop() 
  if ( !anyKeyPressed && keyPressedPreviously )
  {
    keyPressedPreviously = false;
    blehid.keyRelease();
  }  
  

  // Poll interval
  delay(10);



  // Poll interval
  delay(10);


  //time to blink led
  if(millis() - previousTime >= STATE_ONE_TIME ){
    previousTime = millis();

    if(digitalRead(MYLED)){
      digitalWrite(MYLED, LOW);
    }
    else{
      digitalWrite(MYLED, HIGH);
    }

    checkBattery();
  }

  /*
  //neopixel control state
  if(millis() - previousTimeTwo >= STATE_TWO_TIME ){
    previousTimeTwo = millis();

    i++;

    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
      strip.setPixelColor(i, strip.Color(  0, 255,   0));         //  Set pixel's color (in RAM)
      strip.show();                          //  Update strip to match
      //delay(5);                           //  Pause for a moment
    }

  }
  */


}


/*
 * function to start bluetooth and set parameters
 */

 
void setupBluetooth(void){
      
    Bluefruit.begin();
    // Set max power. Accepted values are: -40, -30, -20, -16, -12, -8, -4, 0, 4
    Bluefruit.setTxPower(4);
    Bluefruit.setName("Lost60 Keyboard");

    // Configure and Start Device Information Service
    bledis.setManufacturer("Barringer Engineering LLC");
    bledis.setModel("Lost60 V1.0");
    bledis.begin();

    /// Start BLE HID
    // Note: Apple requires BLE device must have min connection interval >= 20m
    // ( The smaller the connection interval the faster we could send data).
    // However for HID and MIDI device, Apple could accept min connection interval 
    // up to 11.25 ms. Therefore BLEHidAdafruit::begin() will try to set the min and max
    // connection interval to 11.25  ms and 15 ms respectively for best performance.
    //

    blehid.begin();

    // Set callback for set LED from central
    blehid.setKeyboardLedCallback(set_keyboard_led);

    // Set connection interval (min, max) to your perferred value.
    // Note: It is already set by BLEHidAdafruit::begin() to 11.25ms - 15ms
    // min = 9*1.25=11.25 ms, max = 12*1.25= 15 ms 
    //
    // Bluefruit.setConnInterval(9, 12); 

}



/*
 * function to begin bluetooth advertising
 */
 
void startAdv(void)
{  
    // Advertising packet
    Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
    Bluefruit.Advertising.addTxPower();
    Bluefruit.Advertising.addAppearance(BLE_APPEARANCE_HID_KEYBOARD);

    // Include BLE HID service
    Bluefruit.Advertising.addService(blehid);

    // There is enough room for the dev name in the advertising packet
    Bluefruit.Advertising.addName();

    // Start Advertising
    // - Enable auto advertising if disconnected
    // - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
    // - Timeout for fast mode is 30 seconds
    // - Start(timeout) with timeout = 0 will advertise forever (until connected)
    // 
    // For recommended advertising interval
    // https://developer.apple.com/library/content/qa/qa1931/_index.html   
    //
    Bluefruit.Advertising.restartOnDisconnect(true);
    Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
    Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
    Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds
}


/**
 * Callback invoked when received Set LED from central.
 * Must be set previously with setKeyboardLedCallback()
 *
 * The LED bit map is as follows: (also defined by KEYBOARD_LED_* )
 *    Kana (4) | Compose (3) | ScrollLock (2) | CapsLock (1) | Numlock (0)
 */
 
void set_keyboard_led(uint16_t conn_handle, uint8_t led_bitmap)
{
  (void) conn_handle;
  
  // light up Red Led if any bits is set
  if ( led_bitmap )
  {
    ledOn( LED_RED );
  }
  else
  {
    ledOff( LED_RED );
  }
}


/*
 * function to check and print out battery voltage / status
 */
void checkBattery(void)
{  

  float measuredvbat = analogRead(VBATPIN);

  measuredvbat *= 2;    // we divided by 2, so multiply back
  measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
  measuredvbat /= 1024; // convert to voltage

  Serial.print("*** VBat: " ); 
  Serial.print(measuredvbat);
  Serial.println(" ***" );

}


