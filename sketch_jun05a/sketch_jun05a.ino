#include <FastLED.h>
#define NUM_LEDS 600
#define DATA_PIN 6
//CRGB leds[NUM_LEDS];
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGBArray<NUM_LEDS> leds;

int mode = 0;
int mode_new = 0;
int brightness = 64;
int speed = 100;
int pattern = 16;
int length = 8;
int decay = 8;
bool rainbowchase = 1;

//SERIAL CRAP//
char step = 0;
String rxChar;         // RXcHAR holds the received command.
String command = "";
String value = "";
#define TIMEOUT 15  //wait time to end serial mode in seconds
int serialtimeout = TIMEOUT;
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
  Serial.println("--- COMMANDS ---");
  Serial.println("'help' -> Prints this message");
  Serial.println("'mode 0-9' -> Set LED animation mode");
  Serial.println("'brightness 0 - 255' -> Set LED brightness");
  Serial.println("'pattern ?? - ??' -> Set LED pattern/color");
  Serial.println("'speed 0 - 100' -> Set LED animation speed");
  Serial.println("'decay 0 - 256' -> Set duration of LED fade out");
  Serial.println("'length 1 - 32' -> Set LED pattern length");
  Serial.println("Use 'exit' to exit serial and 'off' to turn off LEDs");
  //Serial.println("? -> Print this HELP");  
  //Serial.println("a -> LED On  \"activate\"");
  //Serial.println("d -> LED Off \"deactivate\"");
  //Serial.println("s -> LED     \"status\"");  
}

void setup() {
  delay(300); //safety delay
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  brightness );

  Serial.begin(9600);   // Open serial port (9600 bauds).
  Serial.flush();       // Clear receive buffer.
  printHelp();          // Print the command list.
  command.reserve(200);

  while (!Serial) {
  ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("--- Press enter to begin serial mode ---");
}
bool quick_serial() {
  if (Serial.available()) {
    delay(100);
    Serial.readStringUntil('\n');
    mode_new = mode;
    mode = 9;
    Serial.flush();
    return 1;
  }
  else {
    return 0;
  }
}


void getMode() {

int temp;

  switch (step) {
    
    case 0:
    command = "";
    value = "";
    step = 1;
    Serial.flush(); 
    Serial.print("Serial Mode Active! Command: ");
    break;
    
    case 1:
      if(Serial.available()) {
        serialtimeout = TIMEOUT;
        char rx = (char)Serial.read();
        Serial.flush();
        
        if (rx == ' ') {
          Serial.print(" ");
          step = 2;
        }
        else if (rx == '\n') {
          value = "0";
          step = command[0];
          Serial.println(" - I'll give it a try!");
        }
        else {
          command += rx;
          Serial.print(rx);
        }
      }
      else if (serialtimeout > 0) {
        serialtimeout--;
        delay(1000);
      }
      else {
        serialtimeout = TIMEOUT;
        mode = mode_new;
        Serial.println();
        Serial.println("Serial Mode timed out! Resuming...");
        step = 0;
        return 0;
      }
    break;

    case 2:
    if(Serial.available()) {
      serialtimeout = TIMEOUT;
      char rx = (char)Serial.read();
      Serial.flush();

      if (rx == '\n') {
        step = command[0];
        Serial.println(" - I'll give it a try!");
      }
      else if (char(rx) == ' ') {
        step = 9;//char('e');
      }
      else {
        value += char(rx);
        Serial.print(rx);
      }
    }
    else if (serialtimeout > 0) {
      serialtimeout--;
      delay(1000);
    }
    else {
      Serial.println();
      Serial.println("Serial Mode timed out!");
      step = 'e';
    }
    break;
    
    case 'm':  //MODE
    //change the mode, yo
    temp = value.toInt();
    mode_new = temp; //TODO validation
    Serial.println();
    Serial.print("Mode set to: ");
    Serial.println(mode_new);
    step = 0;
    break;

    case 'b':  //BRIGHTNESS
    //change the brightness, yo
    //Serial.println(value);
    brightness = value.toInt();//std::stoi(value);
    Serial.println();
    Serial.print("Brightness set to: ");
    Serial.println(brightness);
    step = 0;
    break;

    case 'o': //turn LEDs off
    brightness = 0;
    Serial.println();
    Serial.print("LEDs turned off!");
    step = 'e';
    break;

    case 's': //SPEED
    temp = value.toInt();
    if (temp <= 0) {  //TODO stopped?
      temp = 1;
    }
    speed = temp;
    Serial.print("Speed set to: ");
    Serial.println(speed);
    step = 0;
    break;

    case 'l': //length
    temp = value.toInt();
    if (temp <= 1) {
      temp = 1;
    }
    else if (temp > 32) {
      temp = 32;
    }
    length = temp;
    Serial.print("Length set to: ");
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
    temp = value.toInt();
    if (temp > 254) {
      temp = 255;
    }
    else if (temp < 0) {
      temp = 0;
    }
    decay = temp;
    Serial.println();
    Serial.print("Tail set to: ");
    Serial.println(decay);
    step = 0;
    break;

    case 'h': //HELP!!
    step = 0;
    printHelp();
    break;
    
    case 'e': //exit!
    Serial.println("Exiting Serial mode!");
    Serial.println();
    step = 0;
    mode = mode_new;
    break;

    default:                           
    Serial.println();
    Serial.print("'");
    Serial.print(command);
    Serial.println("' is not a valid command!");
    step = 0;
    serialtimeout = TIMEOUT;
    break;
  }
  
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
  int skip_rainbow_inc = int(256/length);

  for(int i = 0; i < NUM_LEDS; i++) {
    int skip_rainbow = 0;
    for(int x = 0; x < NUM_LEDS; x=(x+skip)) {  //writes color to all LEDs
      leds[(i+x)%NUM_LEDS] = ColorFromPalette(RainbowColors_p, (color_id + (skip_rainbow)), brightness, LINEARBLEND);
      //color_id++;  //if increment here you yet a rainbow effect
      if(rainbowchase) {skip_rainbow = skip_rainbow + skip_rainbow_inc;}
    }
    
    if (quick_serial()) {break;}
  
    leds.fadeToBlackBy(decay);
    FastLED.show();
    FastLED.delay(1000/speed);
  }
  if(!rainbowchase) {color_id = color_id + pattern;} //increment to the next psudo-palate
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