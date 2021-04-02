#include <EEPROM.h>

#define NO_FAVS 6

// extern const int addr = 0;
#define ADDR 0

struct fav {
  uint8_t mode;   //determines which animation mode to use
  uint8_t brightness;
  uint16_t speed;  // speed in ms(ish) between updates
  uint8_t pattern;  //stores index for pattern
  uint8_t length;   //broadly dictates density of LEDs
  uint8_t decay;   //fade out on chase LEDs
};

extern fav favorites[NO_FAVS] = 
//mode, bri, spd, pat, len, dec
{{0, 64, 500, 0, 1, 16},    //cannon default
 {1, 128, 500, 10, 1, 16},   //Rainbow pallete slow
 {3, 128, 600, 11, 16, 16},  //Happylights twinkle
 {0, 128, 1000, 10, 16, 16}, //Rainbow chase
 {3, 128, 400, 20, 12, 16},   //Red pallette
 {3, 255, 700, 15, 16, 8}};     //fairy lights

void recallFavorite(char x) {
    mode_new = favorites[x].mode;
    brightness = favorites[x].brightness;
    speed = favorites[x].speed;
    pattern = favorites[x].pattern;
    length = favorites[x].length;
    decay = favorites[x].decay;
    currentPalette = *ActivePaletteList[pattern];
    return 0;
}

void writeFavorite(char x) {

    // const addr = ADDR;
    x = 0; //Just in case...

    favorites[x].mode = mode_new;
    favorites[x].brightness = brightness;
    favorites[x].speed = speed;
    favorites[x].pattern = pattern;
    favorites[x].length = length;
    favorites[x].decay = decay;

    EEPROM.put(ADDR, favorites[x]);
}

void readFavorite(char x) {
  //ONLY works on favorite 0 at this time!! (jankity at best!!!)
    uint16_t a = 0;
    int addr = ADDR;
    uint8_t low;
    uint8_t high;

    // a = EEPROM.read(addr);  //mode
    // Serial.print(addr);
    // Serial.print(" = ");
    // Serial.println(a);
    // addr++;
    // a = EEPROM.read(addr); //bright
    // Serial.print(addr);
    // Serial.print(" = ");
    // Serial.println(a);
    // addr++;

    // low = EEPROM.read(addr);
    // // Serial.print(addr);
    // // Serial.print(" = ");
    // // Serial.println(low);
    // addr++;
    // high = EEPROM.read(addr);
    // // Serial.print(addr);
    // // Serial.print(" = ");
    // // Serial.println(high);
    // uint16_t smush = ((low << 0) & 0xFF) + ((high << 8) & 0xFF00);
    // Serial.print("smushed together...");
    // Serial.print(" = ");
    // Serial.println(smush);
    // addr++;

    // a = EEPROM.read(addr);
    // Serial.print(addr);
    // Serial.print(" = ");
    // Serial.println(a);
    // addr++;
    // a = EEPROM.read(addr);
    // Serial.print(addr);
    // Serial.print(" = ");
    // Serial.println(a);
    // addr++;
    // a = EEPROM.read(addr);
    // Serial.print(addr);
    // Serial.print(" = ");
    // Serial.println(a);

    // a = ADDR;
/*
    a = EEPROM.read(addr);
    favorites[x].mode = a;
    addr++;

    a = EEPROM.read(addr);
    favorites[x].brightness = a;
    addr++;

    low = EEPROM.read(addr);

    addr++;
    high = EEPROM.read(addr);

    uint16_t smush = ((low << 0) & 0xFF) + ((high << 8) & 0xFF00);
    favorites[x].speed = smush;
    // Serial.println(favorites[x].speed);
    addr++;

    a = EEPROM.read(addr);
    favorites[x].pattern = a;
    addr++;

    a = EEPROM.read(addr);
    favorites[x].length = a;
    addr++;

    a = EEPROM.read(addr);
    addr++;*/

    EEPROM.get(addr, favorites[0]);
}
