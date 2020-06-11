#include <FastLED.h>
#define NUM_LEDS 600
#define DATA_PIN 6
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
CRGBArray<NUM_LEDS> leds;  //Set up FastLED array
//CRGB leds[NUM_LEDS];  //Old LED array, doesn't work with TwinkleFox

//Basic control variables setup
int mode = 0;   //determines which animation mode to use
int mode_new = 0;   //placeholder for returning to mode when serial is over
#define DEFAULT_BRIGHTNESS 64
int brightness = DEFAULT_BRIGHTNESS;
int speed = 500;  // speed in ms(ish) between updates
int pattern = 0;  //stores index for pattern
#define MAX_LEN 32 //maximum "length" parameter
int length = 1;   //broadly dictates density of LEDs
int decay = 16;   //fade out on chase LEDs
// bool rainbowchase = 1; //allows chase LEDs to be different colors
#define rainbowchase 1

uint8_t color_id = 0;  //Color palate index; TODO make controllable

//SERIAL CRAP// See serial.ino
char step = 0;
String rxChar;         // RXcHAR holds the received command.
String command = "";
String value = "";
#define TIMEOUT 15  //wait time to end serial mode in seconds
int serialtimeout = TIMEOUT;
#define SERIAL_MODE 9


CRGBPalette16 currentPalette;
TBlendType    currentBlending;

void setup() {
  delay(300); //safety delay
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  brightness );

  Serial.begin(9600);   // Open serial port (9600 bauds).
  Serial.flush();       // Clear receive buffer.
  printHelp();          // Print the command list.
  command.reserve(200);

  tfsetup();

  currentPalette = RainbowColors_p;

  while (!Serial) {
  ; // wait for serial port to connect. Needed for native USB port only
  }
  readFavorite(0);
  recallFavorite(0);
  mode = mode_new;
  Serial.println("--- Press enter to begin serial mode ---");
}

void loop() {
//"Main" loop that calls color modes based on "mode"
//Special "mode" 9 pauses LED animation and services serial
  switch (mode) {
    case 0:
    chase_mode();
    break;

    case 1:
    pallette();
    break;

    case 2:
    dTwinkleFox(0);  //Twinkle w/o BG
    break;

    case 3:
    dTwinkleFox(1); //Twinkle w/BG
    break;

    case SERIAL_MODE:  //SERIAL COMMANDS
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

/*LED chase mode
Lit leds are spread evenly across NUM_LEDs and stay a fixed distance apart
Number of leds is dictated by "length"
Leds have tails that fade out, length determined by "decay"
Each LED is a different color based on the pattern assigned, this can be switched off with "rainbowchase"
If serial is detected by "quick_serial" this loop is abandoned
*/
void chase_mode() {
  uint16_t skip = uint16_t(ceil((NUM_LEDS-1)/length));  //dictates how far apart injections are TODO make dynamic
  uint16_t skip_rainbow_inc = uint16_t(256/(length));  //if each LED is to be a different pallette color, that math is here

  uint16_t range;
  if (speed <= 0) { range = 1; }
  else            { range = NUM_LEDS; }

  for ( int i = 0; i < range; i++ ) {  //main chase animation loop

    uint16_t skip_rainbow = 0; //determines color for each active LED (sort of)

    for(uint16_t x = 0; x < NUM_LEDS; x=(x+skip)) {  //writes color to active LEDs
      leds[(i+x)%NUM_LEDS] = ColorFromPalette(currentPalette, (color_id + (skip_rainbow)), brightness, LINEARBLEND);
      if(rainbowchase) {skip_rainbow = skip_rainbow + skip_rainbow_inc; }
    }
    
    if (quick_serial()) {break;}
  
    leds.fadeToBlackBy(decay);  //This functionally creates the "tails" by darkening any inactive LED; larger decay = shorter tails
    FastLED.show();
    if (speed == 0) {FastLED.delay(100);}    //speed = 0 means LEDs stopped, delay not relevant
    else {FastLED.delay(1000/speed);}
  }
  if(!rainbowchase) {color_id = color_id + length;} //increment to the next psudo-palate  TODO not really what length is for
  return 0;
}

void pallette(void) {
    currentBlending = LINEARBLEND; //TODO make... better

    static uint16_t startIndex = 0;

    fill_palette(leds, NUM_LEDS, startIndex, length, currentPalette, brightness, LINEARBLEND);

    if (quick_serial()) {return 0;}

    FastLED.show();

    if (speed > 0) {
        startIndex = (startIndex + ceil(length/2.0));
        FastLED.delay(1000/ speed);
        }
    else {
        FastLED.delay(100);
    }
}

//EOF