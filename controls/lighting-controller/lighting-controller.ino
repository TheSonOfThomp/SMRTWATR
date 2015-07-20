//Code includes samples written by Mark Kriegsman and Andrew Tuline.  Thanks so much for all your help guys!

#include <FastLED.h>

const int BUFFER_LEN = 6;
char instr_buff[BUFFER_LEN];
char prev_state;

//---LED SETUP STUFF
#define COLOR_ORDER GBR

#define NUM_LEDS 42       // Change to reflect the number of LEDs you have
//fire
#define FRAMES_PER_SECOND 60
#define COOLING  55
#define SPARKING 120

CRGB leds[NUM_LEDS];      //naming our LED array

int ledMode = 0;  //FIRST ACTIVE MODE

CRGBPalette16 currentPalette;
TBlendType    currentBlending;
CRGBPalette16 gPal;


// MODE VARIABLES -- Change these numbers to create new interesting modes

int BRIGHTNESS = 255;    //0-255.  Lower number saves battery life, higher number is screamingly bright
int SATURATION = 255;    //0 is white (no color) and 255 is fully saturated with color
int HUE = 0;             //0-255, around the color wheel
int STEPS = 4;           //Wider or narrower bands of color
int SPEEDO = 10;         //The speed of the animation

// SIN WAVE SETUP

#define qsubd(x, b)  ((x>b)?wavebright:0)                   // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)                          // Analog Unsigned subtraction macro. if result <0, then => 0


// SINWAVE VARIABLES -- change these numbers to modify Sinwave mode
uint8_t wavebright = 255;                                     // You can change the brightness of the waves/bars rolling across the screen.
uint8_t thishue = 30;                                          // You can change the starting hue value for the first wave.
uint8_t thisrot = 1;                                          // You can change how quickly the hue rotates for this wave. 0= color stays the same
uint8_t allsat = 255;                                         // I like 'em fully saturated with colour.
bool thisdir = 0;                                             // You can change direction.
int8_t thisspeed = 4;                                         // You can change the speed, and use negative values.
uint8_t allfreq = 25;                                         // You can change the frequency, thus overall width of bars.
int thisphase = 0;                                            // Phase change value gets calculated.
uint8_t thiscutoff = 192;                                     // You can change the cutoff value to display this wave. Lower value = longer wave.
int loopdelay = 4;                                           // You can change the delay. Also you can change the allspeed variable above.
uint8_t bgclr = 0;                                            // A rotating background colour.
uint8_t bgbright = 55;                                        // Background color brightness

//RIPPLE VARIABLES

uint8_t colour;                                               // Ripple colour is randomized.
int center = 0;                                               // Center of the current ripple.
int step = -1;                                                // -1 is the initializing step.
uint8_t myfade = 255;                                         // Starting brightness.
#define maxsteps 16                                           // Case statement wouldn't allow a variable.


//------------------SETUP------------------
void setup()
{
  delay( 2000 ); // power-up safety delay
  Serial.begin(9600);
  FastLED.addLeds<DOTSTAR, 11, 13, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<DOTSTAR, 0, 20, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  currentBlending = BLEND;
  gPal = HeatColors_p;


}

#define NUM_MODES 3
//------------------MAIN LOOP------------------
void loop() {
  if (Serial.peek() == 'q') {
        if (Serial.available()>=BUFFER_LEN) {
            Serial.readBytes(instr_buff, BUFFER_LEN);
            Serial.println(instr_buff);
            if (instr_buff[1] != prev_state) {
              ledMode = instr_buff[1];
              prev_state = instr_buff[1];
            }
        }
    } else {
        Serial.read();
    }

  switch (ledMode) {

    case '1':
      SPEEDO = 10;
      BRIGHTNESS = 255;
      Rainbow();
      break;              //Rainbow -- Change STEPS and SPEED to modify
    case '2':
      confetti();
      break;
    case '3':
      SPEEDO = 15;
      BRIGHTNESS = 255;
      sinwave_1();
      break;               //Ripple -- Change
    case '4':
      juggle();
      break;
    case '5':
      // Add entropy to random number generator; we use a lot of it.
      random16_add_entropy( random());
      static uint8_t hue = 0;
      hue++;
      gPal = CRGBPalette16( CRGB::Black, CHSV(hue, 255, 192), CHSV(hue, 128, 255), CRGB::White);
      Fire2012WithPalette(); // run simulation frame, using palette colors
      break;
    case '6':
      bpm();
      break;
    case '7':
      cylon();
    case '8':
      BRIGHTNESS = 255;
      SPEEDO = 100;
      STEPS = 4;
      Rainbow();
      break;
  }

}

// SOLID COLOR -------------------------------------
void Solid()
{
  fill_solid(leds, NUM_LEDS, CHSV(HUE, SATURATION, BRIGHTNESS));
  FastLED.show();
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  FastLED.show(); // display this frame
  FastLED.delay(100);
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, HUE+(i*2), beat-HUE+(i*10));
  }
}

//CONFETTI()

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( HUE + random8(64), 200, 255);
  FastLED.show();  
  FastLED.delay(1000/100); 
}

//JUGGLE
void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  FastLED.show();  
  FastLED.delay(1000/100); 
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
  
}
//FIRE 2012 With Palette

void Fire2012WithPalette()
{
  // Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];
  FastLED.show(); // display this frame
  FastLED.delay(70);
  // Step 1.  Cool down every cell a little
  for ( int i = 0; i < NUM_LEDS; i++) {
    heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for ( int k = NUM_LEDS - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
  }

  // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
  if ( random8() < SPARKING ) {
    int y = random8(7);
    heat[y] = qadd8( heat[y], random8(160, 255) );
  }

  // Step 4.  Map from heat cells to LED colors
  for ( int j = 0; j < NUM_LEDS; j++) {
    // Scale the heat value from 0-255 down to 0-240
    // for best results with color palettes.
    byte colorindex = scale8( heat[j], 240);
    leds[j] = ColorFromPalette( gPal, colorindex);
  }
}

// RAINBOW --------------------------------------------------
void Rainbow()
{
  FastLED.setBrightness(  BRIGHTNESS );
  currentPalette = RainbowColors_p;

  static uint8_t startIndex = 0;
  startIndex = startIndex + 1;

  FillLEDsFromPaletteColors( startIndex);

  FastLED.show();
  FastLED.delay(SPEEDO);
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{

  for ( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex, BRIGHTNESS, currentBlending);
    colorIndex += STEPS;
  }
}
//SIN WAVE
void sinwave_1() {
    one_sin();
    show_at_max_brightness_for_power();
    delay_at_max_brightness_for_power(loopdelay*2.5);
    //Serial.println(LEDS.getFPS());
}

void one_sin() {                                                                // This is the heart of this program. Sure is short.
    if (thisdir == 0) thisphase+=thisspeed;
    else thisphase-=thisspeed;            // You can change direction and speed individually.
    thishue = thishue + thisrot;                                                // Hue rotation is fun for thiswave.
    for (int k=0; k<NUM_LEDS; k++) {
        int thisbright = qsubd(cubicwave8((k*allfreq)+thisphase), thiscutoff);      // qsub sets a minimum value called thiscutoff. If < thiscutoff, then bright = 0. Otherwise, bright = 128 (as defined in qsub)..
        leds[k] = CHSV(bgclr, 255, bgbright);
        leds[k] += CHSV(thishue, allsat, thisbright);                               // Assigning hues and brightness to the led array.
    }
    bgclr++;
} 

//CYLON

void cylon(){
  // First slide the led in one direction
  static uint8_t hue = 80;
      for (int i = 0; i < NUM_LEDS; i++) {
        // Set the i'th led to red
        leds[i] = CRGB::White;
        // Show the leds
        FastLED.show();
        // now that we've shown the leds, reset the i'th led to black
        leds[i] = CHSV(hue, 128, 255);
        // Wait a little bit before we loop around and do it again
        delay(30);
      }

      // Now go in the other direction.
      for (int i = NUM_LEDS - 1; i >= 0; i--) {
        // Set the i'th led to red
        leds[i] = CRGB::White;
        // Show the leds
        FastLED.show();
        // now that we've shown the leds, reset the i'th led to black
        leds[i] = CHSV(hue, 255, 192);
        // Wait a little bit before we loop around and do it again
        delay(30);
      }
}
//RIPPLE --------------------------------------------------------------------------------
void ripple() {
  HUE = 140;
  HUE++;
  if (HUE > 220) {
    HUE = 140;   // constrain BG hue to blues and purples
  }
  for (int i = 0; i < NUM_LEDS; i++) leds[i] = CHSV(HUE++, 255, 50);  // Rotate background colour.

  switch (step) {

    case -1:                                                          // Initialize ripple variables.
      center = random(NUM_LEDS);
      colour = random16(0, 256);
      step = 0;
      break;

    case 0:
      leds[center] = CHSV(colour, 255, 255);                          // Display the first pixel of the ripple.
      step ++;
      break;

    case maxsteps:                                                    // At the end of the ripples.
      step = -10;
      break;

    default:                                                          // Middle of the ripples.
      leds[wrap(center + step)] += CHSV(colour, 255, myfade / step * 2); // Display the next pixels in the range for one side.
      leds[wrap(center - step)] += CHSV(colour, 255, myfade / step * 2); // Display the next pixels in the range for the other side.
      step ++;                                                      // Next step.
      break;
  } // switch step

  show_at_max_brightness_for_power();
  delay_at_max_brightness_for_power(SPEEDO * 2.5);
} // ripple()


int wrap(int step) {
  if (step < 0) return NUM_LEDS + step;
  if (step > NUM_LEDS - 1) return step - NUM_LEDS;
  return step;
} // wrap()


