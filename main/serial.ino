

bool quick_serial(void) {
  if (Serial.available()) {
    delay(100);
    Serial.readStringUntil('\n');
    mode_new = mode;
    mode = SERIAL_MODE;
    Serial.flush();
    Serial.println("Serial Mode Active! ");
    return 1;
  }
  else {
    return 0;
  }
}

void printHelp(void) {
    Serial.println("----------- COMMANDS -----------");
    Serial.println("'help' -> Prints this message");
    Serial.println("'mode 0-9' -> Set LED animation mode");
    Serial.println("'brightness 0 - 255' -> Set LED brightness");
    Serial.println("'pattern 0 - 24(ish)' -> Set LED pattern/color");
    Serial.println("'speed 0 - 100' -> Set LED animation speed (Higher is faster)");
    Serial.println("'decay 0 - 256' -> Set duration of LED fade out (Higher is faster");
    Serial.println("'length 1 - 32' -> Set LED pattern length");
    Serial.println("Use 'mode help' and/or 'pattern help' for more info");
    Serial.println("Use 'exit' to exit serial and 'off' (or 'on') to toggle LEDs");
}
void printPatterns(void) {
    Serial.println("----------- PATTERNS -----------");
    Serial.println("0 through 9 = Red through Pink");
    Serial.println("10 - Rainbow");
    Serial.println("11 - Happy Lights!");
    Serial.println("12 - Party!");
    Serial.println("13 - Retro XMas");
    Serial.println("14 - Forest Colors");
    Serial.println("15 - Fairy Lights (creamy white)");
    Serial.println("16 - Cool White Snow");
    Serial.println("17 - Oceal (blue, white, teal)");
    Serial.println("18 - Icey Blues");
    Serial.println("19 - Blue Clouds");
    Serial.println("20 - Orange and Red Fire");
    Serial.println("21 - Lava (fire+white)");
    Serial.println("22 - Green and White");
    Serial.println("23 - Holly");
    Serial.println("24 - Red and White");
    Serial.println("----------- PATTERNS -----------");
}

void printModes(void) {
    Serial.println("----------- MODES -----------");
    Serial.println("0 - LED Chase");
    Serial.println("1 - Pallette Fill");
    Serial.println("2 - Domesticated Twinkle Fox");
    Serial.println("3 - Twinkle Fox w/ Auto Background");
    Serial.println("----------- MODES -----------");
}

/* ~~~~~ SERIAL CONTROL OF MODES ~~~~ */
void getMode(void) {

uint16_t temp;

  switch (step) {
    
    case 0:  //Resets parameters and gets ready to accept a new command
    command = "";
    value = "";
    step = 1;
    serialtimeout = TIMEOUT;
    Serial.flush(); 
    Serial.print("Command: ");
    break;
    
    case 1:  //Checks for first command, separated from second by a " "
      if(Serial.available()) {
        serialtimeout = TIMEOUT;  //we've recieved a command, reset timeout
        char rx = (char)Serial.read(); //temp character storage
        Serial.flush();
        
        if (rx == ' ') {  //if it was a space, move on to storing the 2nd command
          Serial.print(" ");
          step = 2;
        }
        else if (rx == '\n') {  //if it was a newline, just roll with it
          value = "0";
          step = command[0];
          Serial.println(" - I'll give it a try!");
        }
        else {  //otherwise print what you're recieved (echo)
          command += rx;
          Serial.print(rx);
        }
      }
      else if (serialtimeout > 0) {  //otherwise decrement the timeout
        serialtimeout--;
        delay(1000);
      }
      else {  //if timeout has expired, exit serial mode
        Serial.println("Serial mode timed out!");
        step = 'e';
      }
    break;

    case 2:
    if(Serial.available()) {          //Checks for 2nd command, separated from second by a newline
      serialtimeout = TIMEOUT;        //we've recieved a command, reset timeout
      char rx = (char)Serial.read();  //Read the serial buffer
      Serial.flush();

      if ((rx == '\n') || (rx == ' ')) {  //if new char is a newline or space, we're done!
        step = command[0];            //all 1st letters are unique, so just use that for the case
        Serial.println(" - I'll give it a try!");
      }
      else {
        value += char(rx);            //otherwise, print and carry on
        Serial.print(rx);
      }
    }
    else if (serialtimeout > 0) {  //service timeout
      serialtimeout--;
      delay(1000);
    }
    else {    //timeout
      Serial.println("Serial mode timed out!!");
      step = 'e';
    }
    break;
    
    case 'm':  //MODE - Changes the operational mode
    if (value[0] == 'h')  //if the argument is "help" print that
    {
        printModes();
    }
    else {
        temp = value.toInt();
        if (temp == SERIAL_MODE) {  //if mode is set to serial mode we'd be trapped(ish)
            Serial.println("Are you TRYING to break this!?");
            temp = mode_new;    //so basically ignore that
        }
        mode_new = temp; //otherwise store it as the "new" mode (serviced on exit)
        Serial.print("Mode set to: ");
        Serial.println(mode_new);
    }
    step = 0;
    break;

    case 'b':  //BRIGHTNESS
    //change the brightness, yo
    temp = value.toInt();  //conver the brightness
    if (temp >= 255) {    //must be in range 0 - 255
        temp = 255;
    }
    else if (temp < 0) {
        temp = 0;
    }
    brightness = temp;
    Serial.print("Brightness set to: ");
    Serial.println(brightness);
    step = 0;
    break;

    case 'o': //turn LEDs off/on
    if (brightness != 0) {
      brightness = 0;
      Serial.println("LEDs turned off!");
      step = 'e';  //if turning off, exit
    }
    else {
      brightness = DEFAULT_BRIGHTNESS;
      Serial.println("LEDs turned back on at default brightness.");
      step = 0;
    }
    break;

    case 's': //SPEED - Must be between 0 (stopped) and 1000
    temp = value.toInt();
    if (temp < 0) { 
        temp = 0;
    }
    else if (temp > 1000) {
        temp = 1000;
    }
    speed = temp;
    Serial.print("Speed set to: ");
    Serial.println(speed);
    step = 0;
    break;

    case 'l': //length - determines a lot of stuff 0 - MAX_LEN
    temp = value.toInt();
    if (temp <= 1) {
      temp = 1;
    }
    else if (temp > MAX_LEN) {
      temp = MAX_LEN;
      Serial.print("Max length is ");
      Serial.println(MAX_LEN);
    }
    length = temp;
    Serial.print("Length set to: ");
    Serial.println(length);
    step = 0;
    break;

    case 'p': //PATTERN - index to reference the patterns
    if (value[0] == 'h') //if help value is given, print help
    {
        printPatterns();
    }
    else {  //otherwise make sure the value is in range
        uint8_t numberOfPalettes = sizeof(ActivePaletteList) / sizeof(ActivePaletteList[0]);
        temp = value.toInt();
        if (temp < 0) {
            temp = 0;
        }
        else if (temp >= numberOfPalettes) {
            temp = numberOfPalettes;
        }
        pattern = temp;
        currentPalette = *ActivePaletteList[pattern];
        Serial.print("Pattern set to: #");
        Serial.println(pattern);
    }
    step = 0;
    break;

    case 'f': //favorites
    temp = value.toInt();

    if (temp > 5){//NO_FAVS) {
      step = 9;
      break;
    }
    else {
      mode_new = favorites[temp].mode;
      brightness = favorites[temp].brightness;
      speed = favorites[temp].speed;
      pattern = favorites[temp].pattern;
      length = favorites[temp].length;
      decay = favorites[temp].decay;

      Serial.print("Fav set to: #");
      Serial.println(temp);

      step = 0;
    }
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
    Serial.print("Tail set to: ");
    Serial.println(decay);
    step = 0;
    break;

    case 'h': //HELP!!
    step = 0;
    printHelp();
    break;
    
    case 'e': //exit!
    case 'x':
    Serial.println("Exiting Serial mode!");
    Serial.println("-~- Press enter to enter serial mode -~-");
    step = 0;
    mode = mode_new;
    break;

    default:                           
    Serial.println();
    Serial.print("'");
    Serial.print(command);
    Serial.println("' is not a valid command! T_T");
    step = 0;
    serialtimeout = TIMEOUT;
    break;
  }
  
}