#include <FastLED.h>
#define NUM_LEDS 600
#define DATA_PIN 6
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
CRGBArray<NUM_LEDS> leds;  //Set up FastLED array
//CRGB leds[NUM_LEDS];  //Old LED array, doesn't work with TwinkleFox

//SERIAL CRAP// See serial.ino
char step = 0;
String rxChar;         // RXcHAR holds the received command.
String command = "";
String value = "";
#define TIMEOUT 15  //wait time to end serial mode in seconds
int serialtimeout = TIMEOUT;

void setup() {
  delay(300); //safety delay
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  128 );

  Serial.begin(9600);   // Open serial port (9600 bauds).
  Serial.flush();       // Clear receive buffer.

  while (!Serial) {
  ; // wait for serial port to connect. Needed for native USB port only
  }
}

void loop() {
    for ( int i = 0; i < 16; i++) {
        static int ind = 0;
        int temp = (ColorFromPalette( RainbowColors_p, ind, 255, LINEARBLEND));
        Serial.println((temp, HEX));
        ind = ind +8;
    }
    delay(10000);
}