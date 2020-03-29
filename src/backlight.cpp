// ******************************
//
// Initial Test Code for Lost60 Bluetooth Keyboard
//
// Written by Coyt Barringer - Summer 2019
//
// ******************************


//core config and dependencies in here
#include "backlight.hpp"

//Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, NEO_RGB + NEO_KHZ800);
//Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, NEO_RBG + NEO_KHZ800);
//Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, NEO_GBR + NEO_KHZ800);
//Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
//Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, NEO_BRG + NEO_KHZ800);
Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)


//
// task to control RGB underlighting
//
void backlight_task (void* arg)
{
  (void) arg;

  setupBacklighting();

  int mode = 0;    // Currently-active animation mode
  int lastMode = 0; 

  while (1)
  {
    //yield(); // yield run usb background task

    delay(10); //main delay in this forever task loop

    if(global_flag_SleepModeOne){
      //turn off underlighting
      digitalWrite(PIXEL_ENABLE, LOW);

      delay(1000); //free up rtos
    }
    else{

      //turn on underlighting
      digitalWrite(PIXEL_ENABLE, HIGH); //turn pixels on if they aren't currently

      //do this to bring underlighting to a known state upon each change of mode
      if(mode != lastMode){
        lastMode = mode;
      }

      switch(mode) {           // Start the new animation...
        case 0:
          backlightAnimationOne();
          break;
        case 1:
          backlightAnimationTwo();
          break;
        case 2:
          backlightAnimationThree();
          break;
      }
    }

  
  }

}


/*
 * function to setup backlighing on startup - run once
 */
void setupBacklighting(){

  pinMode(PIXEL_PIN, OUTPUT);

  //turn on load switch for neopixel power
  pinMode(PIXEL_ENABLE, OUTPUT);
  digitalWrite(PIXEL_ENABLE, HIGH);




  //initialize RGB "neopixels"
  strip.begin(); // Initialize NeoPixel strip object (REQUIRED)
  strip.show();  // Initialize all pixels to 'off'


  //load startup colors to neopixels
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
      //strip.setPixelColor(i, strip.Color( 255 , 0,   144)); //pink  
      //strip.setPixelColor(i, strip.Color( 31 , 117,   254)); //blue
      strip.setPixelColor(i, strip.Color( 0, 255,   0)); //green
      strip.show();                          //  Update strip to match
      delay(5);                           //  Pause for a moment
  }

}

//
// function defining animation one 
// this will flash all leds to the opposite they were
//
void backlightAnimationOne(void)
{  
  for(int i = 10; i < 255 ; i++){
    strip.setBrightness(i);
    //delay(1);
    strip.show();
    delay(5);
  }
  for(int i = 255; i > 10 ; i--){
    strip.setBrightness(i);
    //delay(1);
    strip.show();
    delay(5);
  }
}


//
// function defining animation two 
// will independently scan through every single led 
//
void backlightAnimationTwo(void)
{  

}


//
// function defining animation three 
//
void backlightAnimationThree(void)
{  
  
}


// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}


// Theater-marquee-style chasing lights. Pass in a color (32-bit value,
// a la strip.Color(r,g,b) as mentioned above), and a delay time (in ms)
// between frames.
void theaterChase(uint32_t color, int wait) {
  for(int a=0; a<10; a++) {  // Repeat 10 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
}


// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait) {
  // Hue of first pixel runs 3 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 3*65536. Adding 256 to firstPixelHue each time
  // means we'll make 3*65536/256 = 768 passes through this outer loop:
  for(long firstPixelHue = 0; firstPixelHue < 3*65536; firstPixelHue += 256) {
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}


// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for(int a=0; a<30; a++) {  // Repeat 30 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (strip.numPixels() steps):
        int      hue   = firstPixelHue + c * 65536L / strip.numPixels();
        uint32_t color = strip.gamma32(strip.ColorHSV(hue)); // hue -> RGB
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show();                // Update strip with new contents
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}