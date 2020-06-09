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

//SERIAL CRAP//
char step = 0;
String rxChar;         // RXcHAR holds the received command.
String command = "";
String value = "";
//char rx = '';

//TESTING
int place = 0;
int counter = 0;

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
  command.reserve(200);

  while (!Serial) {
  ; // wait for serial port to connect. Needed for native USB port only
  }
}
bool quick_serial() {
  if (Serial.available()) {
    mode = 9;
    Serial.flush();
    return 1;
  }
  else {
    return 0;
  }
}
void getMode() {
  if (Serial.available()) {
    char rx = (char)Serial.read();
    Serial.print(rx);
    Serial.flush();
  }
/*
  switch (step) {
    
    case 0:
    command = "";
    value = "";
    step = 1;
    Serial.flush(); 
    Serial.println("Serial Restarted");
    
    
    case 1:
    if(Serial.available()) {
      char rx = (char)Serial.read();//StringUntil(' ');
      Serial.flush();
      Serial.print(rx);
      if (rx == ' ') {
        step = 2;
        Serial.print("Got the command ");
        delay(30);
      }
      else if (rx == '\n') {
        step = 0;
      }
      else {
        command += rx;
      }
    }
    break;

    case 2:
    if(Serial.available()) {
      char rx = (char)Serial.read();//StringUntil(' ');
      Serial.flush();
      Serial.println(rx);
      if (rx == '\n') {
        step = command[0];
        Serial.println("HELLO I AM HERE");
      }
      else if (char(rx) == ' ') {
        //step = 0;
      }
      else {
        value += char(rx);
        Serial.print(rx);
      }
      //Serial.print(command+value);
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
    
  }*/
  
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
    //twinkle();
    break;

    case 3:
    //twinkle(BG_ON);
    break;

    case 9:  //SERIAL COMMANDS
    getMode();
    break;
  }

}

void chase_mode() {
  int skip = int(ceil((NUM_LEDS-1)/length));  //dictates how far apart injections are TODO make dynamic
  
  for(int i = 0; i < NUM_LEDS; i++) {

    for(int x = 0; x < NUM_LEDS; x=(x+skip)) {  //writes color to all LEDs
      leds[(i+x)%NUM_LEDS] = ColorFromPalette(RainbowColors_p, color_id, brightness, LINEARBLEND);
      //color_id++;  //if increment here you yet a rainbow effect
    }
    if (quick_serial()) {break;}
  
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