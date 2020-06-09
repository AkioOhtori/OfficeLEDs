/*Serial_LED_02.ino  Arduining 4 May 2015
Controlling the LED in pin 13 with the Serial Monitor.
--- Command list: ---
? -> Print this HELP 
a -> LED On  "activate"
d -> LED Off "deactivate"
s -> LED     "status" 

Example using the switch statement.
*/
char mode = 'a';
#define LED 13          // Pin 13 is connected to the LED
char rxChar[256];         // RXcHAR holds the received command.

//=== function to print the command list:  ===========================
void printHelp(void){
  Serial.println("--- Command list: ---");
  Serial.println("? -> Print this HELP");  
  Serial.println("a -> LED On  \"activate\"");
  Serial.println("d -> LED Off \"deactivate\"");
  Serial.println("s -> LED     \"status\"");  
  }
  
//---------------- setup ---------------------------------------------
void setup(){
  Serial.begin(9600);   // Open serial port (9600 bauds).
  pinMode(LED, OUTPUT); // Sets pin 13 as OUTPUT.
  Serial.flush();       // Clear receive buffer.
  printHelp();          // Print the command list.
  while (!Serial) {
  ; // wait for serial port to connect. Needed for native USB port only
  }
}

//--------------- loop ----------------------------------------------- 
void loop(){
  if (Serial.available() >0){          // Check receive buffer.
    rxChar = Serial.readStringUntil(' ');            // Save character received.
    Serial.println(rxChar);
    Serial.flush();                    // Clear receive buffer.
  
  }
  if (rxChar == "switch") {
    Serial.println("I got a command to switch modes!");
    mode = 'A';
  }
/*
  switch (mode) {
    
    case 'a':
    case 'A':                          // If received 'a' or 'A':
	if (digitalRead(LED) == LOW){        // If LED is Off:
          digitalWrite(LED,HIGH);      // Turn On the LED.
          Serial.println("LED turned On");
	}
        else Serial.println("LED already On!");
        break;

    case 'd':
    case 'D':                          // If received 'd' or 'D':
	if (digitalRead(LED) == HIGH){       // If LED is On:
          digitalWrite(LED,LOW);       // Turn Off the LED.
          Serial.println("LED turned Off");
	}
        else Serial.println("LED already Off!");
        break;
        
    case 's':
    case 'S':                          // If received  's' or 'S':
	if (digitalRead(LED) == HIGH)        // Read LED status.
          Serial.println("LED status: On");
        else Serial.println("LED status: Off");
        break;
        
    case '?':                          // If received a ?:
        printHelp();                   // print the command list.
        break;
        
    default:                           
      Serial.print("'");
      //Serial.print((char)rxChar);
      Serial.println("' is not a command!");
    
  }*/
  
}


String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
// End of the Sketch.