// If you are going to be using FastLED, you should read the basic pixel reference:
// https://github.com/FastLED/FastLED/wiki/Pixel-reference
// Some of the more obvious things you want to do are explained.

#include "FastLED.h"

// various things to set

// define the number of lights in the string(s)
#define NUM_LEDS 50
// define the arduino pin number used for led control.
#define DATA_PIN 7

// some program control variables

//Decide which code you want to run -- you probablly only want to pick 1, leaving the rest commented out.

// calibration 1
// to use, set up your stranger things display, uncomment the code, and then upload it.
// The lights will cycle through your entire string.
// Write down the letters in order displayed, including something for "not a letter"
// for example --zj-rpwiln...
// Start counting at 0.  In that example, z is light 2, j is light 3, r is light 5, etc.
// Adjust the numbers above in the array, and recomment this calibration code, and move on to calibration 2 below.

// #define CAL1  //for calibration part 1

// Calibration 2
// This calibration turns all lights on (green) if they are not in your alphabet.
// It also flashes through your alphabet from A-Z.
// This test allows you to make sure the order is right.  If something is wrong, repeat calibration 1 above.

// #define CAL2  //for calibration Part 2

// Stranger Things light show V1
// This runs the code that keeps the message area black except while displaying the message, and twinkles
// the remaining lights all the time.
// It doesn't work right for some reason... I need to fix this.

// #define ST1 // for the actual lights code

// Stranger Things V2
// This blinks all the lights like xmas lights.  When it is time to display a message, the lights go dark,
// Flash red, and then display the message.  Shortly after the message finishes, the lights return to blinking.

#define ST2 // for the V2 process

// Overall twinkle speed.
// 0 (VERY slow) to 8 (VERY fast).
#define TWINKLE_SPEED 4

// Overall twinkle density.  This allows some lights to be turned off randomly
// 0 (NONE lit) to 8 (ALL lit at once).
#define TWINKLE_DENSITY 7

// Background color for 'unlit' leds while twinkling
CRGB gBackgroundColor = CRGB::Black; // this means unlit led's are off
// Example of dim incandescent fairy light background color
// CRGB gBackgroundColor = CRGB(CRGB::FairyLight).nscale8_video(16);

// color palette for use in the blinking lights.  Fastled predefines a number of palettes:
/// Cloudy color pallete CloudColors_p
/// Lava colors LavaColors_p
/// Ocean colors, blues and whites OceanColors_p
/// Forest colors, greens ForestColors_p
/// HSV Rainbow RainbowColors_p
/// HSV Rainbow colors with alternatating stripes of black RainbowStripeColors_p
/// HSV color ramp: blue purple ping red orange yellow (and back) PartyColors_p
/// Approximate "black body radiation" palette HeatColors_p
// Replace the below with any of the other palettes above.  Feel free to search the
//  web if you want to design your own palette.

// Since there is some google-fu issues with finding this info, the color palette definitions are here:
// http://fastled.io/docs/3.1/colorpalettes_8cpp_source.html
// For example: 
// extern const TProgmemRGBPalette16 RainbowColors_p FL_PROGMEM =
//   {
//       0xFF0000, 0xD52A00, 0xAB5500, 0xAB7F00,
//       0xABAB00, 0x56D500, 0x00FF00, 0x00D52A,
//       0x00AB55, 0x0056AA, 0x0000FF, 0x2A00D5,
//       0x5500AB, 0x7F0081, 0xAB0055, 0xD5002B
//   };

// okay, here's an example of a self-created palette:
// A palette reminiscent of large 'old-school' C9-size tree lights
// in the five classic colors: red, orange, green, blue, and white.
#define C9_Red    0xB80400
#define C9_Orange 0x902C02  
#define C9_Green  0x046002  
#define C9_Blue   0x070758  
#define C9_White  0x606820
const TProgmemRGBPalette16 RetroC9_p FL_PROGMEM =
{ C9_Red,    C9_Orange, C9_Red,    C9_Orange,
  C9_Orange, C9_Red,    C9_Orange, C9_Red,
  C9_Green,  C9_Green,  C9_Green,  C9_Green,
  C9_Blue,   C9_Blue,   C9_Blue,
  C9_White
};


CRGBPalette16 gCurrentPalette = RetroC9_p;

// CRGBPalette16 gCurrentPalette = RainbowColors_p;

// define the frequency of messages.  note that the system behavior is undefined if the message
// display time exceeds the frequency.  That is if
// MESS_FREQ < (length of longest message) * MILLIS_PER_LETTER /1000
#define MESS_FREQ 30


// number of messages in the array
#define mc 3

//all messages ignore case.  Any non-letter characters are treated as a non-displaying letter
// i.e. the message board will be blank for MILLIS_PER_LETTER milliseconds.
String messages[] = {
  "read",
  "code",
  "hack",
  "make"
};


// we’ll use all 26 letters of the alphabet
// don't try to monkey with this unless you really understand the display code below... its not straightforward to add numbers or puncuation.
#define NUM_LETTERS 26

// the LED number (start counting from 0) that we light up to show our message
// this you need to adjust after runing calibration 1.
const int LETTER_LEDS[NUM_LETTERS] = {
  /*A*/ 35
  ,/*B*/ 36
  ,/*C*/ 37
  ,/*D*/ 38
  ,/*E*/ 39
  ,/*F*/ 40
  ,/*G*/ 41
  ,/*H*/ 42
  ,/*I*/ 32
  ,/*J*/ 31
  ,/*K*/ 30
  ,/*L*/ 29
  ,/*M*/ 28
  ,/*N*/ 27
  ,/*O*/ 26
  ,/*P*/ 25
  ,/*Q*/ 24
  ,/*R*/ 12
  ,/*S*/ 13
  ,/*T*/ 14
  ,/*U*/ 15
  ,/*V*/ 16
  ,/*W*/ 17
  ,/*X*/ 18
  ,/*Y*/ 19
  ,/*Z*/ 20
};

// this defines the lights you want to leave dark at all times.
// this allows you to have the beginning of the string off if needed.
#define NUM_OUT 1

const int OUT_LED[NUM_OUT] = {
  51
};

// how many colors to cycle through for the lights
#define NUM_COLORS 4

// the milliseconds to give each letter
#define MILLIS_PER_LETTER 1000

// Don't screw around past this point.  If you are not careful, you will break it.
// All of the definable parameters are above.



CRGBArray<NUM_LEDS> leds;

// the message we will display
String message;

// the time we received the message
unsigned long received;

int dstatus = 0;

int mnum;

void setup() {
  Serial.begin(57600);
  Serial.println("resetting");
  LEDS.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
  mnum = 1;
}



void loop() {

#ifdef CAL1
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Red;
    FastLED.show();
    delay(1000);
    leds[i] = CRGB::Black;
    FastLED.show();
  }
#endif


#ifdef CAL2

  fill_solid(leds, NUM_LEDS, CRGB::Green);

  for (int i = 0; i < NUM_LETTERS; i++) {
    leds[LETTER_LEDS[i]] = CRGB::Black;
  }

  for (int i = 0; i < NUM_LETTERS; i++) {
    leds[LETTER_LEDS[i]] = CRGB::Red;
    FastLED.show();
    delay(1000);
    leds[LETTER_LEDS[i]] = CRGB::Black;
    FastLED.show();
  }

  fill_solid(leds, NUM_LEDS, CRGB::Black);

#endif


#ifdef ST1


  EVERY_N_SECONDS(MESS_FREQ) {
    message = messages[mnum++];
    received = millis();
    if (mnum == mc) mnum = 0;
  }

  drawTwinkles( leds);

  for (int i = 0; i < NUM_OUT; i++) {
    leds[OUT_LED[i]] = CRGB::Black;
  }

  for (int i = 0; i < NUM_LETTERS; i++) {
    leds[LETTER_LEDS[i]] = CRGB::Black;
  }

  unsigned long elapsed = millis() - received;
  int index = elapsed / MILLIS_PER_LETTER;
  if (index < message.length()) {
    char letter = tolower(message.charAt(index));
    if (letter >= 'a' && letter <= 'z') {
      float brightness = 1 - abs((2 * (elapsed % MILLIS_PER_LETTER) / ((float)MILLIS_PER_LETTER)) - 1);
      uint8_t value = 255 * brightness;

      int letter_index = letter - 'a';
      int led = LETTER_LEDS[letter_index];
      uint8_t hue = (letter_index % NUM_COLORS * 255) / NUM_COLORS;
      leds[led] = CHSV(hue, 255, value);
      FastLED.show();
      leds[led] = CRGB::Black;

      Serial.print(letter);
      Serial.print("\t!");
      Serial.print(led);
      Serial.print("\t=");
      Serial.print(brightness);
      Serial.print("\t@");
      Serial.print(elapsed);
      Serial.println();
    } else {
      // if the letter wasn’t a-z then, we just turn off all the leds
      FastLED.show();
    }
  } else {
    // if the letter is beyond the bounds of the message, we just turn off all the leds
    FastLED.show();
  }

#endif

#ifdef ST2


  EVERY_N_SECONDS(MESS_FREQ) {
    Serial.print("mnum = ");
    Serial.print(mnum);
    Serial.print("\tmess=");
    Serial.print(messages[mnum]);
    message = messages[mnum++];
    Serial.print("\tmess=");
    Serial.print(message);
    if (mnum > mc) mnum = 1;
    dstatus = 1;
    Serial.println("\t status set to 1.");
  }

  if (dstatus == 0)
    drawTwinkles( leds);


  if (dstatus == 1) {
    Serial.println ("status = 1, Flashing");
    leds(0, NUM_LEDS) = CRGB::Black;
    FastLED.show();
    FastLED.delay(500);
    leds(0, NUM_LEDS) = CRGB::Red;
    FastLED.show();
    FastLED.delay(100);
    leds(0, NUM_LEDS) = CRGB::Black;
    FastLED.show();
    FastLED.delay(200);
    leds(0, NUM_LEDS) = CRGB::Red;
    FastLED.show();
    FastLED.delay(100);
    leds(0, NUM_LEDS) = CRGB::Black;
    FastLED.show();
    dstatus = 2;
    received = millis();
  }

  FastLED.show();

  if (dstatus == 2 ) {
    unsigned long elapsed = millis() - received;
    int index = elapsed / MILLIS_PER_LETTER;
    if (index < message.length()) {
      char letter = tolower(message.charAt(index));
      if (letter >= 'a' && letter <= 'z') {
        float brightness = 1 - abs((2 * (elapsed % MILLIS_PER_LETTER) / ((float)MILLIS_PER_LETTER)) - 1);
        uint8_t value = 255 * brightness;

        int letter_index = letter - 'a';
        int led = LETTER_LEDS[letter_index];
        uint8_t hue = (letter_index % NUM_COLORS * 255) / NUM_COLORS;
        leds[led] = CHSV(hue, 255, value);
        FastLED.show();
        leds[led] = CRGB::Black;

        Serial.print(letter);
        Serial.print("\t!");
        Serial.print(led);
        Serial.print("\t=");
        Serial.print(brightness);
        Serial.print("\t@");
        Serial.print(elapsed);
        Serial.println();
      } else {
        // if the letter wasn’t a-z then, we just turn off all the leds
        FastLED.show();
      }
    } else {
      // if the letter is beyond the bounds of the message, we just turn off all the leds
      FastLED.show();
      Serial.print("Index=");
      Serial.print(index);
      Serial.print("\tmess=");
      Serial.print(message);
      Serial.print("\tlen=");
      Serial.print(message.length());
      Serial.println();
      dstatus = 3;
    }
  }

  if (dstatus == 3) {
    leds(0, NUM_LEDS) = CRGB::Black;
    FastLED.show();
    FastLED.delay(500);
    dstatus = 0;
  }

#endif
}



/* Some say imitation is the sincerest form of flattery.
    I din't write the twinkle light code below.
    Its a modified verstion of twinklefox, available at:
    https://gist.github.com/kriegsman/756ea6dcae8e30845b5a
    I cut out a lot of features, which were cool, but
    unnecessary for what I was doing.
*/



//  This function loops over each pixel, calculates the
//  adjusted 'clock' that this pixel should use, and calls
//  "CalculateOneTwinkle" on each pixel.  It then displays
//  either the twinkle color of the background color,
//  whichever is brighter.
void drawTwinkles( CRGBSet& L)
{
  uint16_t PRNG16 = 11337;

  uint32_t clock32 = millis();

  CRGB bg;
  bg = gBackgroundColor; // just use the explicitly defined background color

  uint8_t backgroundBrightness = bg.getAverageLight();

  for ( CRGB& pixel : L) {
    PRNG16 = (uint16_t)(PRNG16 * 2053) + 1384; // next 'random' number
    uint16_t myclockoffset16 = PRNG16; // use that number as clock offset
    PRNG16 = (uint16_t)(PRNG16 * 2053) + 1384; // next 'random' number
    // use that number as clock speed adjustment factor (in 8ths, from 8/8ths to 23/8ths)
    uint8_t myspeedmultiplierQ5_3 =  ((((PRNG16 & 0xFF) >> 4) + (PRNG16 & 0x0F)) & 0x0F) + 0x08;
    uint32_t myclock30 = (uint32_t)((clock32 * myspeedmultiplierQ5_3) >> 3) + myclockoffset16;
    uint8_t  myunique8 = PRNG16 >> 8; // get 'salt' value for this pixel

    // We now have the adjusted 'clock' for this pixel, now we call
    // the function that computes what color the pixel should be based
    // on the "brightness = f( time )" idea.
    CRGB c = computeOneTwinkle( myclock30, myunique8);

    uint8_t cbright = c.getAverageLight();
    int16_t deltabright = cbright - backgroundBrightness;
    if ( deltabright >= 32 || (!bg)) {
      // If the new pixel is significantly brighter than the background color,
      // use the new color.
      pixel = c;
    } else if ( deltabright > 0 ) {
      // If the new pixel is just slightly brighter than the background color,
      // mix a blend of the new color and the background color
      pixel = blend( bg, c, deltabright * 8);
    } else {
      // if the new pixel is not at all brighter than the background color,
      // just use the background color.
      pixel = bg;
    }
  }
}


//  This function takes a time in pseudo-milliseconds,
//  figures out brightness = f( time ), and also hue = f( time )
//  The 'low digits' of the millisecond time are used as
//  input to the brightness wave function.
//  The 'high digits' are used to select a color, so that the color
//  does not change over the course of the fade-in, fade-out
//  of one cycle of the brightness wave function.
//  The 'high digits' are also used to determine whether this pixel
//  should light at all during this cycle, based on the TWINKLE_DENSITY.
CRGB computeOneTwinkle( uint32_t ms, uint8_t salt)
{
  uint16_t ticks = ms >> (8 - TWINKLE_SPEED);
  uint8_t fastcycle8 = ticks;
  uint16_t slowcycle16 = (ticks >> 8) + salt;
  slowcycle16 += sin8( slowcycle16);
  slowcycle16 =  (slowcycle16 * 2053) + 1384;
  uint8_t slowcycle8 = (slowcycle16 & 0xFF) + (slowcycle16 >> 8);

  uint8_t bright = 0;
  if ( ((slowcycle8 & 0x0E) / 2) < TWINKLE_DENSITY) {
    bright = quadwave8(fastcycle8);
  }

  uint8_t hue = slowcycle8 - salt;
  CRGB c;
  if ( bright > 0) {
    c = ColorFromPalette( gCurrentPalette, hue, bright, NOBLEND);
  } else {
    c = CRGB::Black;
  }
  return c;
}



