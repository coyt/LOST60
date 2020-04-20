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
#include "audio.hpp"




//Core Object Declarations - these are externed in the main header file so others such as scan.cpp can see them
BLEDis bledis; // device information
BLEHidAdafruit blehid;
//BLEBas  blebas;  // battery


//global variables
unsigned long previousTime = millis();
uint8_t connection_count = 0;


// Set BATTERY_CAPACITY to the design capacity of your battery.
const unsigned int batteryCapacity = BATTERY_CAPACITY; // e.g. 850mAh battery

//function declarations
void set_keyboard_led(uint16_t conn_hdl, uint8_t led_bitmap);
void startAdv(void);
void setupBluetooth(void);
void checkBattery(void);
void setupBQ27441(void);
void printBatteryStats();


//setup - this code runs once at startup
void setup() 
{
  //begin uart
  Serial.begin(115200);

  //uncomment line below to force code to wait for uart
  //while ( !Serial ) delay(10);   // for nrf52840 with native usb

  //startup message
  Serial.println("*** Lost60 Bluetooth Keyboard Starting Up... ***");

  //board debug led
  pinMode(MYLED, OUTPUT);


  //select board version
  //setup for version one
  #if defined(LOST60_VER_ONE)

  
      //setup bluetooth
      setupBluetooth();

      //Set up and start advertising
      startAdv();

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

  //setup for version two
  #elif defined(LOST60_VER_TWO)

      //setup battery monitoring
      setupBQ27441();

      //setup bluetooth
      setupBluetooth();

      //Set up and start advertising
      startAdv();

      //**********
      //setup other FreeRTOS tasks
      //**********
 
      // Create a task for underlighting (under key leds)
      xTaskCreate( underlight_task, "underlight", LOOP_STACK_SZ, NULL, TASK_PRIO_LOW, NULL);

      // Create a task for backlighting (rgb neopixels on bottom)
      xTaskCreate( backlight_task, "backlight", LOOP_STACK_SZ, NULL, TASK_PRIO_LOW, NULL);

      // Create a task for audio - the two speakers on the board!
      //xTaskCreate( audio_task, "audio", LOOP_STACK_SZ, NULL, TASK_PRIO_LOW, NULL);

      // Create a task for keyboard matrix scanning
      xTaskCreate( scan_task, "scan", LOOP_STACK_SZ, NULL, TASK_PRIO_LOW, NULL);

      //xTaskCreate( hid_task, "hid", 128, NULL, configMAX_PRIORITIES-2, NULL);

  //run hello world
  #else

      //No separate freeRTOS tasks need to be run for hello world. 

  #endif
    
}

//this code loops forever
void loop() 
{

  // delay allows BLE SoC to put this thread to sleep - make sure to add delay like this to all other thread loops
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

  #if defined(LOST60_VER_ONE)
    checkBattery();
  #else
    printBatteryStats();
  #endif

    
  }


}


//
// function to start bluetooth and set parameters
//
void setupBluetooth(void){

    // Config the peripheral connection with maximum bandwidth 
    // more SRAM required by SoftDevice
    // Note: All config***() function must be called before begin()
    //Bluefruit.configPrphBandwidth(BANDWIDTH_MAX);
        

    // Initialize Bluefruit with max concurrent connections as Peripheral = 2, Central = 0
    Bluefruit.begin(MAX_PRPH_CONNECTION, 0);
    //Bluefruit.begin();
    // Set max power. Accepted values are: -40, -30, -20, -16, -12, -8, -4, 0, 4
    Bluefruit.setTxPower(4);
    Bluefruit.setName(BLUETOOTH_NAME);


    // Configure and Start Device Information Service
    bledis.setManufacturer(BLUETOOTH_MANUFACTURER);
    bledis.setModel(BLUETOOTH_MODEL);
    bledis.begin();


    // Start BLE Battery Service
    //blebas.begin();
    //blebas.write(100);


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

    // Secondary Scan Response packet (optional)
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
  //if ( led_bitmap )
  //{
  //  ledOn( LED_RED );
 // }
  //else
 // {
  //  ledOff( LED_RED );
  //}
}


/*
 * function to check and print out battery voltage / status
 */

void checkBattery(void)
{  

  #if defined(LOST60_VER_ONE)
    float measuredvbat = analogRead(VBATPIN);
  #else
    float measuredvbat = 3.3; //TODO FIX THIS //analogRead(VBATPIN);
  #endif

  measuredvbat *= 2;    // we divided by 2, so multiply back
  measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
  measuredvbat /= 1024; // convert to voltage

  Serial.print("*** VBat: " ); 
  Serial.print(measuredvbat);
  Serial.println(" ***" );

}

#if defined(LOST60_VER_ONE)
  //do nothing
#else
  void setupBQ27441(void)
  {
    // Use lipo.begin() to initialize the BQ27441-G1A and confirm that it's
    // connected and communicating.
    if (!lipo.begin()) // begin() will return true if communication is successful
    {
    // If communication fails, print an error message and loop forever.
      Serial.println("Error: Unable to communicate with BQ27441.");
      Serial.println("  Check wiring and try again.");
      Serial.println("  (Battery must be plugged into Battery Babysitter!)");
      //while (1) ; //why would u do this!?
    }
    Serial.println("Connected to BQ27441!");
    
    // Uset lipo.setCapacity(BATTERY_CAPACITY) to set the design capacity
    // of your battery.
    lipo.setCapacity(batteryCapacity);
  }


  void printBatteryStats()
  {
    // Read battery stats from the BQ27441-G1A
    unsigned int soc = lipo.soc();  // Read state-of-charge (%)
    unsigned int volts = lipo.voltage(); // Read battery voltage (mV)
    int current = lipo.current(AVG); // Read average current (mA)
    unsigned int fullCapacity = lipo.capacity(FULL); // Read full capacity (mAh)
    unsigned int capacity = lipo.capacity(REMAIN); // Read remaining capacity (mAh)
    int power = lipo.power(); // Read average power draw (mW)
    int health = lipo.soh(); // Read state-of-health (%)

    // Now print out those values:
    String toPrint = String(soc) + "% | ";
    toPrint += String(volts) + " mV | ";
    toPrint += String(current) + " mA | ";
    toPrint += String(capacity) + " / ";
    toPrint += String(fullCapacity) + " mAh | ";
    toPrint += String(power) + " mW | ";
    toPrint += String(health) + "%";
    
    Serial.println(toPrint);
  }

#endif







