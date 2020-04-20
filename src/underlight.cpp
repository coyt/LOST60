// ******************************
//
// Initial Test Code for Lost60 Bluetooth Keyboard
//
// Written by Coyt Barringer - Summer 2019
//
// ******************************


//core config and dependencies in here
#include "underlight.hpp"


//
// task to control led matrix underlighting
//
void underlight_task (void* arg)
{
  (void) arg;


  //setup underlighting leds 
  setupUnderLightingLeds();

  int mode = 2;    // Currently-active animation mode
  int lastMode = 2; 

  while (1)
  {


    //yield(); // yield run usb background task


    delay(100); //main delay in this forever task loop


    
    if(global_flag_SleepModeOne){
      //turn off underlighting
      underlightOff();
      delay(1000); //free up rtos
    }
    else{
      //do this to bring underlighting to a known state upon each change of mode
      if(mode != lastMode){
        underlightOff();
        lastMode = mode;
      }

      switch(mode) {           // Start the new animation...
        case 0:
          animationOne();
          break;
        case 1:
          animationTwo();
          break;
        case 2:
          animationThree();
          break;
      }
    }

    


  }

}



//
// function to configure and set key underlighting 
//
void setupUnderLightingLeds(void)
{  

  //setup transistor I/O pins as outputs and turn all OFF
  pinMode(ULED_ROW1, OUTPUT);
  pinMode(ULED_ROW2, OUTPUT);
  pinMode(ULED_ROW3, OUTPUT);
  pinMode(ULED_ROW4, OUTPUT);
  pinMode(ULED_ROW5, OUTPUT);

  digitalWrite(ULED_ROW1, LOW);
  digitalWrite(ULED_ROW2, LOW);
  digitalWrite(ULED_ROW3, LOW);
  digitalWrite(ULED_ROW4, LOW);
  digitalWrite(ULED_ROW5, LOW);

  //setup latch and blank for led driver ic
  pinMode(ULED_LATCH, OUTPUT);
  pinMode(ULED_BLANK, OUTPUT);
  pinMode(ULED_SIN, OUTPUT);
  pinMode(ULED_CLK, OUTPUT);

  digitalWrite(ULED_LATCH, LOW);
  digitalWrite(ULED_BLANK, HIGH);
  digitalWrite(ULED_SIN, LOW);
  digitalWrite(ULED_CLK, LOW);

  delay(10);

  //16 bits control the I/O lines
  //cols 0 to 14 are wired up
  //represents LEDs
  //0,1,2,3,4,5,6,7,  8,9,10,11,12,13,14,15 //16 bits
  //1,0,0,0,0,0,0,0,  0,0,00,00,00,00,00,00 // 
  //128
  //0

  //uint8_t dataOne = 255;//128; //highbyte
  //uint8_t dataTwo = 255; //lowbyte

  //uint16_t data = 65535; //0;//65535;

  //0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
  //1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 
  //uint16_t data = 65024;//65535;

  //uint16_t data = 0b1100000000000000;

  underlightOn();

  Serial.println("*** Underlighting Enabled ***" );

}


//
// function defining animation one 
// this will flash all leds to the opposite they were
//
void animationOne(void)
{  

  //LED BLANK pin is inverted logic so remember to fix it everywhere
  if(!digitalRead(ULED_BLANK)){
    underlightOff();
  }
  else{
    underlightOn();
  }

}


//
// function defining animation two 
// will independently scan through every single led 
//
void animationTwo(void)
{  

  //make a transistor chain high
  digitalWrite(ULED_ROW1, HIGH);
  for(int i = 0; i < 14; i++){
    loadNewShiftRegisterData(0b1000000000000000 >> i);
    delay(100);
  }
  digitalWrite(ULED_ROW1, LOW);

  //make a transistor chain high
  digitalWrite(ULED_ROW2, HIGH);
  for(int i = 0; i < 14; i++){
    loadNewShiftRegisterData(0b1000000000000000 >> i);
    delay(100);
  }
  digitalWrite(ULED_ROW2, LOW);

  //make a transistor chain high
  digitalWrite(ULED_ROW3, HIGH);
  for(int i = 0; i < 14; i++){
    loadNewShiftRegisterData(0b1000000000000000 >> i);
    delay(100);
  }
  digitalWrite(ULED_ROW3, LOW);

  //make a transistor chain high
  digitalWrite(ULED_ROW4, HIGH);
  for(int i = 0; i < 14; i++){
    loadNewShiftRegisterData(0b1000000000000000 >> i);
    delay(100);
  }
  digitalWrite(ULED_ROW4, LOW);

  //make a transistor chain high
  digitalWrite(ULED_ROW5, HIGH);
  for(int i = 0; i < 14; i++){
    loadNewShiftRegisterData(0b1000000000000000 >> i);
    delay(100);
  }
  digitalWrite(ULED_ROW5, LOW);
}


//
// function defining animation three 
//
void animationThree(void)
{  
  underlightOn();
  delay(1000);
}

//
// function defining underlighting off 
//
void underlightOff(void)
{  

  loadNewShiftRegisterData(0);

  //make a transistor chain high
  digitalWrite(ULED_ROW1, LOW);
  digitalWrite(ULED_ROW2, LOW);
  digitalWrite(ULED_ROW3, LOW);
  digitalWrite(ULED_ROW4, LOW);
  digitalWrite(ULED_ROW5, LOW);

}


//
// function defining underlighting on 
//
void underlightOn(void)
{  

  loadNewShiftRegisterData(65535);

  //make a transistor chain high
  digitalWrite(ULED_ROW1, HIGH);
  digitalWrite(ULED_ROW2, HIGH);
  digitalWrite(ULED_ROW3, HIGH);
  digitalWrite(ULED_ROW4, HIGH);
  digitalWrite(ULED_ROW5, HIGH);

}


//
// function to load new values to the underlighting shift register
//
void loadNewShiftRegisterData(uint16_t localData)
{  

  //shift out highbyte
  shiftOut(ULED_SIN, ULED_CLK, LSBFIRST, localData);
  //shift out lowbyte
  shiftOut(ULED_SIN, ULED_CLK, LSBFIRST, (localData>>8));


  //toggle latch to load 16 shifted vals into output register
  delay(10);
  digitalWrite(ULED_LATCH, HIGH);
  delay(10);
  digitalWrite(ULED_LATCH, LOW);
  delay(10);
  
  //make BLANK LOW in order to make outputs enabled
  digitalWrite(ULED_BLANK, LOW);

}

