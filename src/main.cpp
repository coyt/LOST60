// ******************************
//
// Initial Test Code for Lost60 Bluetooth Keyboard
//
// Written by Coyt Barringer - Summer 2019
// 
//
// ******************************

//core config and dependencies in here
#include "Lost60.hpp"
#include "underlight.hpp"
#include "backlight.hpp"
#include "scan.hpp"
#include "power.hpp"


//Core Object Declarations - these are externed in the main header file so others such as scan.cpp can see them
BLEDis bledis;
BLEHidAdafruit blehid;


//global variables
unsigned long previousTime = millis();


//function declarations
void set_keyboard_led(uint16_t conn_hdl, uint8_t led_bitmap);
void startAdv(void);
void setupBluetooth(void);
void checkBattery(void);


//setup - this code runs once at startup
void setup() 
{
  //begin uart
  Serial.begin(115200);

  //uncomment line below to force code to wait for uart
  //while ( !Serial ) delay(10);   // for nrf52840 with native usb

  //startup message
  Serial.println("*** Lost60 Bluetooth Keyboard Starting Up... ***");

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
  xTaskCreate( scan_task, "scan", LOOP_STACK_SZ, NULL, TASK_PRIO_LOW, NULL);

  //xTaskCreate( hid_task, "hid", 128, NULL, configMAX_PRIORITIES-2, NULL);

}


//this code loops forever
void loop() 
{

  // delay allows BLE SoC to put this thread to sleep
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


}


//
// function to start bluetooth and set parameters
//
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



//
// function to begin bluetooth advertising
//
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


