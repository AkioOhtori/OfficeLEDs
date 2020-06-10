

bool quick_serial(void) {
  if (Serial.available()) {
    delay(100);
    Serial.readStringUntil('\n');
    mode_new = mode;
    mode = 9;
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
  Serial.println("'pattern ?? - ??' -> Set LED pattern/color");
  Serial.println("'speed 0 - 100' -> Set LED animation speed (Higher is faster)");
  Serial.println("'decay 0 - 256' -> Set duration of LED fade out (Higher is faster");
  Serial.println("'length 1 - 32' -> Set LED pattern length");
  Serial.println("Use 'exit' to exit serial and 'off' (or 'on') to toggle LEDs");
}

/* ~~~~~ SERIAL CONTROL OF MODES ~~~~ */
void getMode(void) {

int temp;

  switch (step) {
    
    case 0:
    command = "";
    value = "";
    step = 1;
    Serial.flush(); 
    Serial.print("Command: ");
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
        Serial.println("Serial Mode timed out!");
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
    temp = value.toInt();//std::stoi(value);
    if (temp >= 255) {
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
    }
    else {
      brightness = DEFAULT_BRIGHTNESS;
      Serial.println("LEDs turned back on at default brightness.");
    }
    step = 0;
    break;

    case 's': //SPEED
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