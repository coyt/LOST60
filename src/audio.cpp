// ******************************
//
// Initial Test Code for Lost60 Bluetooth Keyboard
//
// Written by Coyt Barringer - Summer 2019
//
// ******************************


//core config and dependencies in here
#include "audio.hpp"
#include "RotaryEncoder.h"

//synthEngine mixer(20E3);
//const char song[] PROGMEM = "::e,8f,8g,8a,8b,8c1,8d1,8e1";  //the C major scale. This format is known as RingTone Transfer Language or RTTL(It was used by Nokia's phone company).
//const char song2[] PROGMEM = ":d=4:c,d,e,f";
//const char song3[] PROGMEM = ":o=6,d=2:16a,16b,8a,4b";
//const char song4[] PROGMEM = ":o=5,d=2:32g,32a,32b,32c";
//MusicWithoutDelay instrument(song);          //d=4 means that every note without a number in front of the letter is assumed to be a quarter note.
//MusicWithoutDelay instrument2(song2);
//MusicWithoutDelay instrument3(song3);
//MusicWithoutDelay instrument4(song4);

void encoder_callback(int step);

volatile bool rotaryEncoderRight = false;
volatile bool rotaryEncoderLeft = false;

int speakerOneFrequency = 500;
int speakerOneLastFrequency = 500;

//HardwarePWM speakerOne(NRF_PWM1);
//HardwarePWM speakerTwo(NRF_PWM2);


//
// task to control audio features
//
void audio_task (void* arg)
{
  (void) arg;

  setupAudio();


  while (1)
  {
    //yield(); // yield run usb background task
    delay(10); //main delay in this forever task loop

    //instrument.update();
    //instrument2.update();

    if(rotaryEncoderRight){
      rotaryEncoderRight = false;
      speakerOneFrequency ++;
    }

    if(rotaryEncoderLeft){
      rotaryEncoderLeft = false;
      speakerOneFrequency --;
    }

    if(speakerOneFrequency != speakerOneLastFrequency){
      //tone(SPEAKER_A, speakerOneFrequency);
      Serial.print("Freq: ");
      Serial.println(speakerOneFrequency);
      speakerOneLastFrequency = speakerOneFrequency;

    }


  }

}


/*
 * function to setup audio on startup - run once
 */
void setupAudio(){

    #if defined(LOST60_VER_ONE)
      //we do no audio setup
    #else
      //setup speaker pins
      pinMode(SPEAKER_A, OUTPUT);
      pinMode(SPEAKER_B, OUTPUT);

      digitalWrite(SPEAKER_A, LOW);
      digitalWrite(SPEAKER_B, LOW);

      // Initialize Encoder
      RotaryEncoder.begin(ROTARY_A, ROTARY_B);

      // Set callback
      RotaryEncoder.setCallback(encoder_callback);

      // Start encoder
      RotaryEncoder.start();

      //tone(SPEAKER_A, speakerOneFrequency);

    #endif



    //tone(SPEAKER_A, 500);
    //tone(SPEAKER_B, 1000);

  //instrument.begin(SPEAKER_A, TRIANGLE, ENVELOPE0, 0);
  //instrument.setVolume(127);
  //instrument2.begin(SPEAKER_B, TRIANGLE, ENVELOPE0, 0);
  //instrument3.begin(TRIANGLE, ENVELOPE0, 0);
  //instrument4.begin(TRIANGLE, ENVELOPE0, 0);



}


void encoder_callback(int step)
{
  if ( step > 0 )
  {
    rotaryEncoderRight = true;
     //Serial.println("Right");
  }else
  {
    rotaryEncoderLeft = true;
    //Serial.println("Left");
  }  
}