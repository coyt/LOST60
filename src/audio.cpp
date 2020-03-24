// ******************************
//
// Initial Test Code for Lost60 Bluetooth Keyboard
//
// Written by Coyt Barringer - Summer 2019
//
// ******************************


//core config and dependencies in here
#include "audio.hpp"

synthEngine mixer(20E3);
const char song[] PROGMEM = "::e,8f,8g,8a,8b,8c1,8d1,8e1";  //the C major scale. This format is known as RingTone Transfer Language or RTTL(It was used by Nokia's phone company).
const char song2[] PROGMEM = ":d=4:c,d,e,f";
const char song3[] PROGMEM = ":o=6,d=2:16a,16b,8a,4b";
const char song4[] PROGMEM = ":o=5,d=2:32g,32a,32b,32c";
MusicWithoutDelay instrument(song);          //d=4 means that every note without a number in front of the letter is assumed to be a quarter note.
MusicWithoutDelay instrument2(song2);
MusicWithoutDelay instrument3(song3);
MusicWithoutDelay instrument4(song4);

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
  }

}


/*
 * function to setup audio on startup - run once
 */
void setupAudio(){

    //setup speaker pins
    pinMode(SPEAKER_A, OUTPUT);
    pinMode(SPEAKER_B, OUTPUT);

    digitalWrite(SPEAKER_A, LOW);
    digitalWrite(SPEAKER_B, LOW);

    //tone(SPEAKER_A, 500);
    //tone(SPEAKER_B, 1000);

  //instrument.begin(SPEAKER_A, TRIANGLE, ENVELOPE0, 0);
  //instrument.setVolume(127);
  //instrument2.begin(SPEAKER_B, TRIANGLE, ENVELOPE0, 0);
  //instrument3.begin(TRIANGLE, ENVELOPE0, 0);
  //instrument4.begin(TRIANGLE, ENVELOPE0, 0);



}