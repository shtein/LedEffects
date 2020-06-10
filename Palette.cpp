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
