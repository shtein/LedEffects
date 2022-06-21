#include "LedEffects.h"

#include <FastLED.h>
#include "Palette.h"

//Autumn and Halloween

// Gradient palette "halloween_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/rc/tn/halloween.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 16 bytes of program space.

DEFINE_GRADIENT_PALETTE( halloween_gp ) {
    0, 173, 53,  1,
  127,   0,  0,  0,
  191, 173, 53,  1,
  255, 173, 53,  1};

  
// Gradient palette "es_autumn_20_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/es/autumn/tn/es_autumn_20.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

DEFINE_GRADIENT_PALETTE( es_autumn_20_gp ) {
    0,  24, 39,  1,
   76,  87, 53,  1,
  140, 208, 71,  3,
  244, 215,161, 75,
  255, 215,161, 75};

// Gradient palette "es_autumn_16_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/es/autumn/tn/es_autumn_16.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

DEFINE_GRADIENT_PALETTE( es_autumn_16_gp ) {
    0, 252,  1,  4,
   89, 153, 21,  6,
  153, 125, 45, 12,
  216,  91, 78, 16,
  255,  91, 78, 16};


// Gradient palette "es_autumn_02_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/es/autumn/tn/es_autumn_02.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

DEFINE_GRADIENT_PALETTE( es_autumn_02_gp ) {
    0,  86,  6,  1,
  127, 255,255,125,
  153, 255,255,125,
  242, 194, 96,  1,
  255, 194, 96,  1};

  // Gradient palette "es_autumn_01_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/es/autumn/tn/es_autumn_01.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

DEFINE_GRADIENT_PALETTE( es_autumn_01_gp ) {
    0,  30,  0,  0,
   66,  30,  3,  1,
  127,  30, 14,  1,
  229,  30, 60,  1,
  255,  30, 60,  1};


// Gradient palette "es_autumn_04_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/es/autumn/tn/es_autumn_04.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

DEFINE_GRADIENT_PALETTE( es_autumn_04_gp ) {
    0,   2,  1,  1,
  101,  27,  1,  0,
  165, 210, 22,  1,
  234, 255,166, 42,
  255, 255,166, 42};

// Gradient palette "es_autumn_03_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/es/autumn/tn/es_autumn_03.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 16 bytes of program space.

DEFINE_GRADIENT_PALETTE( es_autumn_03_gp ) {
    0, 128, 43,  1,
  127,  54,  1,  1,
  216,  36, 13,  1,
  255,  36, 13,  1};
  
                                      

//Christmas

// Gradient palette "christmattree1_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/pj/7/tn/christmattree1.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 60 bytes of program space.

DEFINE_GRADIENT_PALETTE( christmattree1_gp ) {
    0, 140, 97, 11,
   10, 255,255,255,
   25,   1, 80,  3,
   53,   1, 80,  3,
   79, 220,  1,  1,
  101, 220,  1,  1,
  109, 255,255,255,
  119, 140, 97, 11,
  124, 255,255,255,
  137, 220,  1,  1,
  165, 220,  1,  1,
  188,   1, 80,  3,
  232,   1, 80,  3,
  249, 232,235,237,
  255, 140, 97, 11};



// Gradient palette "patriot_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/rc/tn/patriot.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 16 bytes of program space.

DEFINE_GRADIENT_PALETTE( patriot_gp ) {
    0, 255,  0,  0,
  122, 255,  0,  0,
  132,   0, 16,255,
  255,   0, 16,255};


  // Gradient palette "patriot_black_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/pj/classic/tn/patriot-black.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 24 bytes of program space.

DEFINE_GRADIENT_PALETTE( patriot_black_gp ) {
    0, 227,  1,  1,
   61, 255,255,255,
  127,   0,  0,125,
  191,   0,  0,  0,
  252, 227,  1,  1,
  255, 227,  1,  1};


// Gradient palette "july4_2_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/pj/7/tn/july4-2.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 96 bytes of program space.

DEFINE_GRADIENT_PALETTE( july4_2_gp ) {
    0, 206,213,203,
    7, 242,241,237,
    7, 255,255,255,
   15,  60, 73, 88,
   25,   1,  1, 18,
   43,   2,  3, 96,
   58,   1,  4, 33,
   71,   1,  4, 33,
   86,   2,  3, 96,
   94,  45, 60, 88,
  101, 242,241,237,
  109, 242,241,237,
  117, 242,241,237,
  127,  65, 40, 30,
  140, 107,  1,  1,
  150, 121,  1,  1,
  165, 121,  1,  1,
  173,  25,  1,  1,
  183,  25,  1,  1,
  209, 107,  1,  1,
  219,  25,  1,  1,
  226, 171,138,156,
  237, 234,255,233,
  255, 234,255,233};


//RGB Palettes
// Ukraine flag colord
#define UA_FLAG_BLUE   0x0057B8
#define UA_FLAG_YELLOW 0xFFD700

const TProgmemRGBPalette16 UkraineFlag_p FL_PROGMEM ={
  UA_FLAG_BLUE, UA_FLAG_BLUE, UA_FLAG_BLUE, UA_FLAG_BLUE,
  UA_FLAG_BLUE, UA_FLAG_BLUE, UA_FLAG_BLUE, UA_FLAG_BLUE,
  UA_FLAG_YELLOW, UA_FLAG_YELLOW, UA_FLAG_YELLOW, UA_FLAG_YELLOW,
  UA_FLAG_YELLOW, UA_FLAG_YELLOW, UA_FLAG_YELLOW, UA_FLAG_YELLOW
};

const TProgmemRGBPalette16 RedGreenWhite_p FL_PROGMEM =
{  CRGB::Red, CRGB::Red, CRGB::Red, CRGB::Red, 
   CRGB::Red, CRGB::Red, CRGB::Red, CRGB::Red, 
   CRGB::Red, CRGB::Red, CRGB::Gray, CRGB::Gray, 
   CRGB::Green, CRGB::Green, CRGB::Green, CRGB::Green 
};

// A mostly (dark) green palette with red berries.
#define Holly_Green 0x00580c
#define Holly_Red   0xB00402
const TProgmemRGBPalette16 Holly_p FL_PROGMEM =
{  Holly_Green, Holly_Green, Holly_Green, Holly_Green, 
   Holly_Green, Holly_Green, Holly_Green, Holly_Green, 
   Holly_Green, Holly_Green, Holly_Green, Holly_Green, 
   Holly_Green, Holly_Green, Holly_Green, Holly_Red 
};

// A red and white striped palette
// "CRGB::Gray" is used as white to keep the brightness more uniform.
const TProgmemRGBPalette16 RedWhite_p FL_PROGMEM =
{  CRGB::Red,  CRGB::Red,  CRGB::Red,  CRGB::Red, 
   CRGB::Gray, CRGB::Gray, CRGB::Gray, CRGB::Gray,
   CRGB::Red,  CRGB::Red,  CRGB::Red,  CRGB::Red, 
   CRGB::Gray, CRGB::Gray, CRGB::Gray, CRGB::Gray 
};

// A mostly blue palette with white accents.
// "CRGB::Gray" is used as white to keep the brightness more uniform.
const TProgmemRGBPalette16 BlueWhite_p FL_PROGMEM =
{  CRGB::Blue, CRGB::Blue, CRGB::Blue, CRGB::Blue, 
   CRGB::Blue, CRGB::Blue, CRGB::Blue, CRGB::Blue, 
   CRGB::Blue, CRGB::Blue, CRGB::Blue, CRGB::Blue, 
   CRGB::Blue, CRGB::Gray, CRGB::Gray, CRGB::Gray 
};

// A pure "fairy light" palette with some brightness variations
#define HALFFAIRY ((CRGB::FairyLight & 0xFEFEFE) / 2)
#define QUARTERFAIRY ((CRGB::FairyLight & 0xFCFCFC) / 4)
const TProgmemRGBPalette16 FairyLight_p FL_PROGMEM =
{  CRGB::FairyLight, CRGB::FairyLight, CRGB::FairyLight, CRGB::FairyLight, 
   HALFFAIRY,        HALFFAIRY,        CRGB::FairyLight, CRGB::FairyLight, 
   QUARTERFAIRY,     QUARTERFAIRY,     CRGB::FairyLight, CRGB::FairyLight, 
   CRGB::FairyLight, CRGB::FairyLight, CRGB::FairyLight, CRGB::FairyLight 
};

// A palette of soft snowflakes with the occasional bright one
const TProgmemRGBPalette16 Snow_p FL_PROGMEM =
{  0x304048, 0x304048, 0x304048, 0x304048,
   0x304048, 0x304048, 0x304048, 0x304048,
   0x304048, 0x304048, 0x304048, 0x304048,
   0x304048, 0x304048, 0x304048, 0xE0F0FF 
};

// A palette reminiscent of large 'old-school' C9-size tree lights
// in the five classic colors: red, orange, green, blue, and white.
#define C9_Red    0xB80400
#define C9_Orange 0x902C02
#define C9_Green  0x046002
#define C9_Blue   0x070758
#define C9_White  0x606820
const TProgmemRGBPalette16 RetroC9_p FL_PROGMEM =
{  C9_Red,    C9_Orange, C9_Red,    C9_Orange,
   C9_Orange, C9_Red,    C9_Orange, C9_Red,
   C9_Green,  C9_Green,  C9_Green,  C9_Green,
   C9_Blue,   C9_Blue,   C9_Blue,
   C9_White
};

// A cold, icy pale blue palette
#define Ice_Blue1 0x0C1040
#define Ice_Blue2 0x182080
#define Ice_Blue3 0x5080C0
const TProgmemRGBPalette16 Ice_p FL_PROGMEM =
{
  Ice_Blue1, Ice_Blue1, Ice_Blue1, Ice_Blue1,
  Ice_Blue1, Ice_Blue1, Ice_Blue1, Ice_Blue1,
  Ice_Blue1, Ice_Blue1, Ice_Blue1, Ice_Blue1,
  Ice_Blue2, Ice_Blue2, Ice_Blue2, Ice_Blue3
};


//Pacifica palettes
const TProgmemRGBPalette16 Pacifica_1_p FL_PROGMEM = 
{ 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 
  0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x14554B, 0x28AA50 
};

const TProgmemRGBPalette16 Pacifica_2_p FL_PROGMEM = 
{ 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 
  0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x0C5F52, 0x19BE5F 
};

const TProgmemRGBPalette16 Pacifica_3_p FL_PROGMEM = 
{ 
  0x000208, 0x00030E, 0x000514, 0x00061A, 0x000820, 0x000927, 0x000B2D, 0x000C33, 
  0x000E39, 0x001040, 0x001450, 0x001860, 0x001C70, 0x002080, 0x1040BF, 0x2060FF 
};


//Valentine's Day

// Gradient palette "firstlove_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/pj/7/tn/firstlove.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 40 bytes of program space.

DEFINE_GRADIENT_PALETTE( firstlove_gp ) {
    0, 249, 63, 72,
   35, 155, 81,192,
   79, 249,108, 93,
   99, 247, 33,100,
  119, 247, 59, 80,
  153, 188, 70,106,
  186, 137, 26, 99,
  211, 249,195,212,
  242, 247, 33,100,
  255, 247, 33,100 };

// Gradient palette "roseedan_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/pj/5/tn/roseedan.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

DEFINE_GRADIENT_PALETTE( roseedan_gp ) {
    0, 220, 86, 88,
   96, 247, 16, 10,
  165, 255, 13, 79,
  221, 252,104, 67,
  255, 252,104, 67};

// Gradient palette "passionata_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/pj/classic/tn/passionata.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

DEFINE_GRADIENT_PALETTE( passionata_gp  ) {
    0, 217,  4,102,
   66, 217,  4, 61,
  122, 217,  4,  5,
  186, 161,  4,219,
  255,  78,  4,219};
