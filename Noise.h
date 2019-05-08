#ifndef __NOISE_H
#define __NOISE_H


////////////////////////////
// Effect EffectPaletteTransformFast - simple transition between palettes
class EffectPaletteTransformFast: public EffectPaletteTransform{
  public:
    EffectPaletteTransformFast();
    ~EffectPaletteTransformFast();

  protected:
    void updateLeds(CRGB *leds, int numLeds);
    int getMaxStep() const;
};

inline EffectPaletteTransformFast::EffectPaletteTransformFast(){
}

inline EffectPaletteTransformFast::~EffectPaletteTransformFast(){
}


inline void EffectPaletteTransformFast::updateLeds(CRGB *leds, int numLeds){  
  //Do full range for current palette
  for(int i = 0; i < numLeds; i++){       
    leds[i]  = ColorFromPalette(_palCurrent, 
                                 (uint8_t)map(i, 0, numLeds - 1, 0, 255), 
                                 255, 
                                 LINEARBLEND 
                               );
  }
}



inline int EffectPaletteTransformFast::getMaxStep() const{
  return  MAX_PAL_CHANGES * 3;
}


////////////////////////////
//EffectNoise
class EffectNoise: public EffectPaletteTransform{
  public:
     EffectNoise(); 
    ~EffectNoise();

  protected:
    void updateLeds(CRGB *leds, int numLeds);   
     
  protected:
    int _dist;
};

inline EffectNoise::EffectNoise(){

  //Init distortion
  _dist = random16(millis());
}

inline EffectNoise::~EffectNoise(){ 
}


#define XSCALE          30
#define YSCALE          30

inline void EffectNoise::updateLeds(CRGB *leds, int numLeds){
  
  //Do default actions
  for(int i = 0; i < numLeds; i++){       
    leds[i]  = ColorFromPalette(_palCurrent, 
                                 (uint8_t)( inoise8(i * XSCALE, _dist + i * YSCALE) % 255 ),
                                 255, 
                                 LINEARBLEND 
                               );
  }

  
  //Prepare for the next move
  _dist += beatsin8(10, 1, 4);                                               
}


////////////////////////////////////////
// EffectPlasma
class EffectPlasma: public EffectPaletteTransform{
  public: 
    EffectPlasma();
    ~EffectPlasma();

  protected:
    void updateColors();
    void updateLeds(CRGB *leds, int numLeds);
    int getMaxStep() const;
};

inline EffectPlasma::EffectPlasma(){
  setSpeedDelay(50);  
}

inline EffectPlasma::~EffectPlasma(){
}

inline void EffectPlasma::updateLeds(CRGB *leds, int numLeds){
  int phase1 = beatsin8(6,-64, 64);
  int phase2 = beatsin8(7,-64, 64);

  for(int i = 0; i < numLeds; i++){
    int clrIndex      = cubicwave8( (i * 23) + phase1) / 2 + cos8( (i * 15) + phase2) / 2;
    int clrBrightness = qsuba(clrIndex, beatsin8(7, 0, 96));
    
    leds[i] = ColorFromPalette(_palCurrent, clrIndex, clrBrightness, LINEARBLEND);
  }  
}

inline void EffectPlasma::updateColors(){

  uint8_t clr = random8();
  _palTarget =  CRGBPalette16(CHSV(clr + random8(32), 192, random8(128,255)), 
                              CHSV(clr + random8(32), 255, random8(128,255)), 
                              CHSV(clr + random8(32), 192, random8(128,255)), 
                              CHSV(clr + random8(32), 255, random8(128,255))
                             );
}

#define PLASMA_MAX_STEPS 100

inline int EffectPlasma::getMaxStep() const{
  return PLASMA_MAX_STEPS;
}


//////////////////////////////
// Effect Confetti
class EffectConfetti: public EffectPaletteTransform{
  public:
    EffectConfetti();
    ~EffectConfetti();

  protected:
    void updateLeds(CRGB *leds, int numLeds);
    void updateColors(); 

    int getMaxStep() const;    
};

inline EffectConfetti::EffectConfetti(){  
  setSpeedDelay(20);
}

inline EffectConfetti::~EffectConfetti(){  
}


#define CONFETTI_MAX_STEPS 100

inline int EffectConfetti::getMaxStep() const{
  return CONFETTI_MAX_STEPS;
}

inline void EffectConfetti::updateLeds(CRGB *leds, int numLeds){
  
  fadeToBlackBy(leds, numLeds, 8);                     
  leds[random16(numLeds)] = ColorFromPalette(_palCurrent, random8(255), 255, LINEARBLEND);
}

inline void EffectConfetti::updateColors(){
   struct { const TProgmemRGBPalette16 &pal;
   } ts[] = { OceanColors_p, LavaColors_p, ForestColors_p, CloudColors_p }; 
   
  _palTarget = ts[random(0, sizeof(ts) / sizeof(ts[0]) + 1)].pal;
}



////////////////////////////////////////
// Palettes for different type of transformation

/////////////////////
//Christmas
BEGIN_TRANFORM_SCHEMA(TransformChristmas)
  TRANSOFRM_PALETTE(christmattree1_gp)
END_TRANSFORM_SCHEMA()

////////////////////
// Autumn
BEGIN_TRANFORM_SCHEMA(TransformAutunm)
  TRANSOFRM_PALETTE(es_autumn_01_gp)
  TRANSOFRM_PALETTE(es_autumn_03_gp)
END_TRANSFORM_SCHEMA()

///////////////////
//Halloween - better autumn than autumn
BEGIN_TRANFORM_SCHEMA(TransformHalloween)
  TRANSOFRM_PALETTE(halloween_gp)
END_TRANSFORM_SCHEMA()


#endif //__NOISE_H
