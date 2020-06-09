/*Serial_LED_02.ino  Arduining 4 May 2015
Controlling the LED in pin 13 with the Serial Monitor.
--- Command list: ---
mode
brightness
length
speed
pattern
favorite

Example using the switch statement.
*/
char mode = 0;
#define LED 13          // Pin 13 is connected to the LED
String rxChar;         // RXcHAR holds the received command.
String command = "";
String value = "";

//=== function to print the command list:  ===========================
void printHelp(void){
  Serial.println("--- Command list: ---");
  //Serial.println("? -> Print this HELP");  
  //Serial.println("a -> LED On  \"activate\"");
  //Serial.println("d -> LED Off \"deactivate\"");
  //Serial.println("s -> LED     \"status\"");  
  }
  
//---------------- setup ---------------------------------------------
void setup(){
  Serial.begin(9600);   // Open serial port (9600 bauds).
  Serial.flush();       // Clear receive buffer.
  printHelp();          // Print the command list.

  while (!Serial) {
  ; // wait for serial port to connect. Needed for native USB port only
  }
}

//--------------- loop ----------------------------------------------- 
void loop(){

if (Serial.available()) {
  char rx = (char)Serial.read();
  Serial.print(rx);
  Serial.flush();
}
  
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