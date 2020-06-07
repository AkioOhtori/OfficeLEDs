#include <FastLED.h>
#define NUM_LEDS 300
#define LED_PIN 6
CRGB leds[NUM_LEDS];
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define BRIGHTNESS 32
//CRGBArray<NUM_LEDS> leds;

void setup() {
    delay( 3000 ); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
    fill_solid(leds, NUM_LEDS, CRGB::Green);
    FastLED.show();

}


const TProgmemRGBPalette16 HappyLight_p FL_PROGMEM =
{  CRGB::Cyan, CRGB::Black, CRGB::Black, CRGB::Lime, 
   CRGB::Black, CRGB::Cyan, CRGB::Black, CRGB::Black, 
   CRGB::DarkViolet, CRGB::Black, CRGB::Black, CRGB::Lime, 
   CRGB::Black, CRGB::DarkViolet, CRGB::Black, CRGB::Black };

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t colorIndex = 0;
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( HappyLight_p, colorIndex, 255, LINEARBLEND);
        colorIndex += 16;  //extends/comresses pattern
    }
    FastLED.delay(1000 / 60);
    FastLED.show();

  //delay(30);
}
