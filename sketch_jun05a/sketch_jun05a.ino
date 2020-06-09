#include <FastLED.h>
#define NUM_LEDS 600
#define DATA_PIN 6
//CRGB leds[NUM_LEDS];
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGBArray<NUM_LEDS> leds;

int mode = 0;
int brightness = 64;
int speed = 100;
int pattern = 5;
int length = 1;
int decay = 8;

char step = 0;
#define LED 13          // Pin 13 is connected to the LED
String rxChar;         // RXcHAR holds the received command.
String command = "";
String value = "";

/*
#define BRIGHTNESS 64
#define RATELIMIT 2 //TODO make variable
#define CYCLES NUM_LEDS//*RATELIMIT
#define UPDATES_PER_SECOND 100  //TODO make variable, dynamic
*/
//#define OFFSET 150
uint8_t color_id = 0;  //Color palate index; TODO make controllable
//uint8_t len = 1;  //Defines the number of lit LEDs active at once; TODO make controlable
//=== function to print the command list:  ===========================
void printHelp(void) {
  Serial.println("--- Command list: ---");
  //Serial.println("? -> Print this HELP");  
  //Serial.println("a -> LED On  \"activate\"");
  //Serial.println("d -> LED Off \"deactivate\"");
  //Serial.println("s -> LED     \"status\"");  
}

void setup() {
  delay(300); //safety delay
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  brightness );
  printf("boop");

  Serial.begin(9600);   // Open serial port (9600 bauds).
  Serial.flush();       // Clear receive buffer.
  printHelp();          // Print the command list.

  while (!Serial) {
  ; // wait for serial port to connect. Needed for native USB port only
  }
}
void getMode() {
  switch (step) {
    
    case 0:
    command = "";
    value = "";
    step = 1;
    Serial.flush(); 
    Serial.println("Serial Restarted");
    case 1:

    if(Serial.available()) {
      command = Serial.readStringUntil(' ');
      Serial.flush(); 
      step = 2;
      Serial.print("Got the command ");
      //Serial.print(command);
    }
    else {
      Serial.println("Step 2");
    }
    break;

    case 2:
    if(Serial.available()) {
      value = Serial.readStringUntil('/n');
      Serial.flush(); 
      step = command[0];
      //Serial.print(command+value);
    }
    else {
      Serial.println("Step 3");
    }
    break;

    case 'm':  //MODE
    //change the mode, yo
    step = 0;
    break;

    case 'b':  //BRIGHTNESS
    //change the brightness, yo
    //Serial.println(value);
    brightness = value.toInt();//std::stoi(value);
    Serial.print("Brightness set to ");
    Serial.println(brightness);
    step = 0;
    break;

    case 's': //SPEED
    case 'S':
    int s = value.toInt();
    if (s <= 0) {  //TODO stopped?
      s = 1;
    }
    speed = s;
    Serial.print("Speed set to ");
    Serial.println(speed);
    step = 0;
    break;

    case 'l': //length
    int l = value.toInt();
    if (l <= 1) {
      l = 1;
    }
    else if (l > 32) {
      l = 32;
    }
    length = l;
    Serial.print("Length set to ");
    Serial.println(length);
    step = 0;
    break;

    case 'p': //PATTERN
    step = 0;
    break;

    case 'f': //favorites
    step = 0;
    break;

    case 'd': //decay/ tail length
    step = 0;
    break;
        
    default:                           
      Serial.print("'");
      Serial.print(command);
      Serial.println("' is not a command!");
      step = 0;
    
  }
  
}

void loop() {
  getMode();
  
  if(1) {
    chase_mode();
  }
  else {
    pallette();
  }
}

void chase_mode() {
  int skip = int(ceil((NUM_LEDS-1)/length));  //dictates how far apart injections are TODO make dynamic
  for(int i = 0; i < NUM_LEDS; i++) {

    for(int x = 0; x < NUM_LEDS; x=(x+skip)) {  //writes color to all LEDs
      leds[(i+x)%NUM_LEDS] = ColorFromPalette(RainbowColors_p, color_id, brightness, LINEARBLEND);
      //color_id++;  //if increment here you yet a rainbow effect
      getMode();
    }
    
    leds.fadeToBlackBy(decay);
    FastLED.show();
    FastLED.delay(1000/speed);
  }
  color_id = color_id + 16; //increment to the next palate TODO make controllabe
  return 0;
}

void pallette() {
  return 0;
}

/*TODO
multistring wrap
multistring
multimode
controllability */