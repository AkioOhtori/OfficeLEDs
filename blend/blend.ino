/* End to end blending
By: Andrew Tuline
Date: Oct 25, 2015
*/

#include "FastLED.h"

#define LED_PIN     6
//#define CK_PIN   11
#define COLOR_ORDER GBR
#define CHIPSET WS2812B    
#define NUM_LEDS    300

CRGB leds[NUM_LEDS];

int max_bright = 32;

CRGB endclr;
CRGB midclr;

void setup() {
  Serial.begin (57600);

  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness( max_bright );
}

void loop(){ 
  uint8_t speed = beatsin8(6,0,255);
  endclr = blend(CRGB::Aqua, CRGB::Orange, speed);
  midclr = blend(CRGB::Orange, CRGB::Aqua, speed);
  cycle();
  FastLED.show();
}

void cycle() {
  fill_gradient_RGB(leds, 0, endclr, NUM_LEDS/2, midclr);
  fill_gradient_RGB(leds, NUM_LEDS/2+1, midclr, NUM_LEDS, endclr);
}
