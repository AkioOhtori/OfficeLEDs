
struct fav {
  uint8_t mode;   //determines which animation mode to use
  uint8_t brightness;
  uint16_t speed;  // speed in ms(ish) between updates
  uint8_t pattern;  //stores index for pattern
  uint8_t length;   //broadly dictates density of LEDs
  uint8_t decay;   //fade out on chase LEDs
};

extern fav favorites[5] = 
//mode, bri, spd, pat, len, dec
{{1, 128, 50, 10, 1, 16},   //Rainbow pallete slow
 {3, 128, 500, 11, 2, 16},  //Happylights twinkle
 {2, 128, 250, 11, 4, 16}, //Happy w/o BG
 {1, 64, 100, 0, 1, 16},   //Red pallette
 {0, 64, 25, 0, 3, 8}};     //three red tails

void createFavorites(void) {
    // f0 = {1, 128, 100, 11, 8, 16};
    // favorites[0] = {1, 128, 100, 11, 8, 16};
    brightness = favorites[0].brightness;
}