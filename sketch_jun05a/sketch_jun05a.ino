#include <FastLED.h>
#define NUM_LEDS 300
#define DATA_PIN 6
//CRGB leds[NUM_LEDS];
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGBArray<NUM_LEDS> leds;
#define BRIGHTNESS 64
#define RATELIMIT 4 //TODO make variable
#define CYCLES NUM_LEDS*RATELIMIT
#define UPDATES_PER_SECOND 20  //TODO make variable, dynamic

//#define OFFSET 150
uint8_t color_id = 0;  //Color palate index; TODO make controllable
uint8_t len = 16;  //Defines the number of lit LEDs active at once; TODO make controlable
int skip = int(ceil((NUM_LEDS-1)/len));  //dictates how far apart injections are TODO make dynamic

void setup() {
  delay(300); //safety delay
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
}

void loop() {
  if(1) {
    chase_mode();
  }
}

void chase_mode() {
  for(int i = 0; i < CYCLES; i++) {
    int n = int((i/RATELIMIT));  //advances LEDs (range = 0-NUM_LEDS)
    for(int x = 0; x < NUM_LEDS; x=(x+skip)) {  //writes color to all LEDs
      leds[(n+x)%NUM_LEDS] = ColorFromPalette(RainbowColors_p, color_id, BRIGHTNESS, LINEARBLEND);
      //color_id++;  //if increment here you yet a rainbow effect
    }
    leds.fadeToBlackBy(8);
    FastLED.show();
  }
  color_id = color_id + 16; //increment to the next palate TODO make controllabe
}

/*TODO
multistring wrap
multistring
multimode
controllability */