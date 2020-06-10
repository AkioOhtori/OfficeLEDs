#include <FastLED.h>
#define NUM_LEDS 600
#define DATA_PIN 6
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGBArray<NUM_LEDS> leds;
//CRGB leds[NUM_LEDS];

int mode = 0;
int mode_new = 0;
#define DEFAULT_BRIGHTNESS 64
int brightness = DEFAULT_BRIGHTNESS;
int speed = 100;
int pattern = 0;
#define MAX_LEN 32
int length = 1;
int decay = 16;
bool rainbowchase = 1;

uint8_t color_id = 0;  //Color palate index; TODO make controllable

//SERIAL CRAP//
char step = 0;
String rxChar;         // RXcHAR holds the received command.
String command = "";
String value = "";
#define TIMEOUT 15  //wait time to end serial mode in seconds
int serialtimeout = TIMEOUT;

void setup() {
  delay(300); //safety delay
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  brightness );

  Serial.begin(9600);   // Open serial port (9600 bauds).
  Serial.flush();       // Clear receive buffer.
  printHelp();          // Print the command list.
  command.reserve(200);
  
  tfsetup();

  while (!Serial) {
  ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("--- Press enter to begin serial mode ---");
}

void loop() {
 
  switch (mode) {
    case 0:
    chase_mode();
    break;

    case 1:
    pallette();
    break;

    case 2:
    twinkleFox(0);
    break;

    case 3:
    twinkleFox(1);
    break;

    case 4:
    //solid
    break;

    case 9:  //SERIAL COMMANDS
    getMode();
    break;

    default:
    Serial.println();
    Serial.print("Invalid mode '");
    Serial.print(mode);
    Serial.println("', switching to 0. T_T");
    mode = 0;
  }

}

void chase_mode() {
  int skip = int(ceil((NUM_LEDS-1)/length));  //dictates how far apart injections are TODO make dynamic
  int skip_rainbow_inc = int(256/length);  //if each LED is to be a different pallette color, that math is here

  for(int i = 0; i < NUM_LEDS; i++) {
    int skip_rainbow = 0;
    for(int x = 0; x < NUM_LEDS; x=(x+skip)) {  //writes color to all LEDs
      leds[(i+x)%NUM_LEDS] = ColorFromPalette(RainbowColors_p, (color_id + (skip_rainbow)), brightness, LINEARBLEND);
      if(rainbowchase) {skip_rainbow = skip_rainbow + skip_rainbow_inc;}
    }
    
    if (quick_serial()) {break;}
  
    leds.fadeToBlackBy(decay);
    FastLED.show();
    if (speed == 0) {FastLED.delay(1000);}
    else {FastLED.delay(1000/speed);}
  }
  if(!rainbowchase) {color_id = color_id + pattern;} //increment to the next psudo-palate
  return 0;
}

//EOF