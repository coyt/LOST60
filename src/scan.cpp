// ******************************
//
// Initial Test Code for Lost60 Bluetooth Keyboard
//
// Written by Coyt Barringer - Summer 2019
//
// ******************************


//core config and dependencies in here
#include "scan.hpp"

/* 
uint8_t colCount = sizeof(cols);
uint8_t rowCount = sizeof(rows);

bool keyPressedPreviously = false;

//variables for 
#define KEYBOARD_SLEEP_TIME 60000 //60s 
bool usbHIDEnabled = false;
bool bleHIDEnabled = false;
bool keyboardActive = false;
char keyboardLayer = 'A';

//
// task to control keyboard matrix scanning
//
void scan_task (void* arg)
{
  (void) arg;

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


  while (1)
  {
    //yield(); // yield run usb background task

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
            tud_hid_keyboard_keycode(modifier, keycode);

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
      tud_hid_keyboard_keycode(modifier, keycode);
    }


    // Send All-zero report to indicate there is no keys pressed
    // Most of the time, it is, though we don't need to send zero report
    // every loop(), only a key is pressed in previous loop() 
    if ( !anyKeyPressed && keyPressedPreviously )
    {
      keyPressedPreviously = false;
      tud_hid_keyboard_key_release();
    }  
    

    // Poll interval
    delay(10);

    }

}

*/

//tud_hid_keyboard_keycode(modifier, keycode);
//tud_hid_keyboard_key_release();



/* 
//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+


// Must match with ID declared by HID Report Descriptor, better to be in header file
enum
{
  REPORT_ID_KEYBOARD = 1,
  REPORT_ID_MOUSE
};

void hid_task(void* params)
{
  (void) params;

  while (1)
  {
    // Poll every 10ms
    vTaskDelay(pdMS_TO_TICKS(10));

    uint32_t const btn = board_button_read();

    // Remote wakeup
    if ( tud_suspended() && btn )
    {
      // Wake up host if we are in suspend mode
      // and REMOTE_WAKEUP feature is enabled by host
      tud_remote_wakeup();
    }

    //------------- Mouse -------------
    if ( tud_hid_ready() )
    {
      if ( btn )
      {
        int8_t const delta = 5;

        // no button, right + down, no scroll pan
        tud_hid_mouse_report(REPORT_ID_MOUSE, 0x00, delta, delta, 0, 0);

        // delay a bit before attempt to send keyboard report
        vTaskDelay(pdMS_TO_TICKS(2));
      }
    }

    //------------- Keyboard -------------
    if ( tud_hid_ready() )
    {
      // use to avoid send multiple consecutive zero report for keyboard
      static bool has_key = false;

      if ( btn )
      {
        uint8_t keycode[6] = { 0 };
        keycode[0] = HID_KEY_A;

        tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode);

        has_key = true;
      }else
      {
        // send empty key report if previously has key pressed
        if (has_key) tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
        has_key = false;
      }
    }
  }
}
*/