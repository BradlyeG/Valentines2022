//The sketch for a valentine's day card made in 2022.

#include <Adafruit_NeoPixel.h>
#include <Ramp.h>

#define PIN 6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(3, PIN, NEO_GRB + NEO_KHZ800);
ramp bInterpOne, bInterpTwo, bInterpThree; // Each LED needs its own ramp.
int ledOne = 3; // Pin mapping. Using PWM (or analog) pins for this.
int ledTwo = 10;
int ledThree = 9;

void setup() {
  strip.begin(); //This needs to be here when using the Neopixels.
  strip.setBrightness(50);
  strip.show(); // Initialize all pixels to 'off'
  
  bInterpOne.setGrain(random(1.0)); //The set grain function adds a bit of random noise. Think about how old film looked a bit "grainy". That's random noise.
  bInterpTwo.setGrain(random(1.0));
  bInterpThree.setGrain(random(1.0));
  
  bInterpOne.go(0.0); //Start these ramps at origin. Think of ramps as functions, like y=mx+b. You can calculate y at any arbitrary x, but we're starting at zero.
  bInterpTwo.go(0.0);
  bInterpThree.go(0.0);

/*The "Go" function can be used to start at an arbitrary point, or we can supply additional arguments to build our function.
 * Consider the purpose of these ramps. They represent the brightness of the LEDs, and we want them to twinkle like animated stars.
 * To do this, we're going to set a domain, starting at 0 and ending at 175. Because we're using PWM (this applies to analog as well)
 * the maximum voltage, or duty cycle in digital terms, is a scale from 0 - 255. From an artistic perspective, we want these LEDs
 * to sit in the background of the design and not stand out from the Neopixels, so we don't go to full brightness even though we could.
 * We also need to define a period for our ramp. The period represents how long we want to spend ramping up to our maximum value.
 * "BACK_INOUT" is the ramp, or interpolation, mode. This specific piece represents the function that is run, starting at 0 and ending
 * at 175, which will take 2400 ms. See https://github.com/siteswapjuggler/RAMP for the full list of interpolation modes. Note that 
 * there is no specific techicnal reason that ramp mode was chosen. After experimenting with different modes, the specific chosen mode
 * met the art direction criteria. That is subjective.
 * Lastly, the "FORTHANDBACK" parameter is the looping mode. Once we get 175, we want to think about getting back to zero. See the above
 * link for the full list of looping modes.
 */

  bInterpOne.go(175, 2400, BACK_INOUT, FORTHANDBACK);
  bInterpTwo.go(175, 2400, BACK_INOUT, FORTHANDBACK);
  bInterpThree.go(175, 2400, BACK_INOUT, FORTHANDBACK);
  
  pinMode(ledOne, OUTPUT); //Set the pins we need to output
  pinMode(ledTwo, OUTPUT);
  pinMode(ledThree, OUTPUT);
}

void loop() {
  // Most of the code is based on Adafruit's FLORA onboard NeoPixel example, located here: https://learn.adafruit.com/getting-started-with-flora/blink-onboard-neopixel .
  rainbowCycle(.75);
  //Fading the LED
  analogWrite(ledOne, bInterpOne.update()); //We use the "Update" function to capture the instantaneous value of our ramp when it is run.
  delay(random(25.0)); //We want a short delay to slow down the frequency of our update, again for artistic purposes. Use random to give a more interesting-looking effect.
  analogWrite(ledTwo, bInterpOne.update());
  delay(random(25.0));
  analogWrite(ledThree, bInterpOne.update());
  delay(random(25.0));
}

// Fill the dots one after the other with a color.
//This function didn't provide the look wanted by the author, but is left anyways.
void colorWipe(int32_t c, int8_t wait) {
  for(int16_t i=3; i>-1; i--) { //Hardcoded values are generally against best practice. They are also easy and the author does not anticipate any updates or scaling to this code.
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
// Note the difference from the original code. Using i>-1 and --i flips the direction of the effect, and makes sure the 1st, or pixel number 0, in our chain (array) is lit.
void rainbowCycle(float wait) {
  int16_t i, j;

  for(j=0; j<256; j++) { // 5 cycles of all colors on wheel
    for(i=strip.numPixels(); i>-1; --i) {
      strip.setPixelColor(i, Wheel(((i * 512 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}
