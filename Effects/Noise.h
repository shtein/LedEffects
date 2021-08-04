#ifndef __NOISE_H
#define __NOISE_H


////////////////////////////
// Effect EffectPaletteTransformFast - simple transition between palettes
class EffectPaletteTransformFast: public EffectPaletteTransform{
  public:
    EffectPaletteTransformFast(FuncGetPalette_t getPal = &FuncGetPal_Default);
    ~EffectPaletteTransformFast();

  protected:
    void updateLeds(CRGB *leds, int numLeds);
    int getMaxStep() const;
};

inline EffectPaletteTransformFast::EffectPaletteTransformFast(FuncGetPalette_t getPal): EffectPaletteTransform(getPal){
}

inline EffectPaletteTransformFast::~EffectPaletteTransformFast(){
}


inline void EffectPaletteTransformFast::updateLeds(CRGB *leds, int numLeds){  
  //Do full range for current palette
  for(int i = 0; i < numLeds; i++){       
    leds[i]  = getCurrentPalColor((uint8_t)map(i, 0, numLeds - 1, 0, 255));
  }
}



inline int EffectPaletteTransformFast::getMaxStep() const{
  return  MAX_PAL_CHANGES * 3;
}


////////////////////////////
//EffectNoise
class EffectNoise: public EffectPaletteTransform{
  public:
     EffectNoise(FuncGetPalette_t getPal = &FuncGetPal_Default);
    ~EffectNoise();

  protected:
    void updateLeds(CRGB *leds, int numLeds);   
     
  protected:
    int _dist;
};

inline EffectNoise::EffectNoise(FuncGetPalette_t getPal): EffectPaletteTransform(getPal){

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
    leds[i]  = getCurrentPalColor((uint8_t)( inoise8(i * XSCALE, _dist + i * YSCALE) % 255 ));
  }

  
  //Prepare for the next move
  _dist += beatsin8(10, 1, 4);                                               
}


////////////////////////////////////////
// EffectPlasma

inline void FuncGetPal_Plazma(CRGBPalette16 &pal){

  uint8_t clr = random8();
  pal =  CRGBPalette16(CHSV(clr + random8(32), 192, random8(128,255)), 
                       CHSV(clr + random8(32), 255, random8(128,255)), 
                       CHSV(clr + random8(32), 192, random8(128,255)), 
                       CHSV(clr + random8(32), 255, random8(128,255))
                      );
}

class EffectPlasma: public EffectPaletteTransform{
  public: 
    EffectPlasma(FuncGetPalette_t getPal = &FuncGetPal_Plazma);
    ~EffectPlasma();

  protected:
    void updateLeds(CRGB *leds, int numLeds);
    int getMaxStep() const;
};

inline EffectPlasma::EffectPlasma(FuncGetPalette_t getPal): EffectPaletteTransform(getPal){
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
    
    leds[i] = getCurrentPalColor(clrIndex, clrBrightness);
  }  
}

#define PLASMA_MAX_STEPS 100

inline int EffectPlasma::getMaxStep() const{
  return PLASMA_MAX_STEPS;
}


//////////////////////////////
// Effect Confetti


BEGIN_TRANSFORM_SCHEMA_RGB16_PALETTE(Confetti)
  TRANSOFRM_PALETTE(OceanColors_p)
  TRANSOFRM_PALETTE(LavaColors_p)
  TRANSOFRM_PALETTE(ForestColors_p)
  TRANSOFRM_PALETTE(CloudColors_p)
END_TRANSFORM_SCHEMA()


class EffectConfetti: public EffectPaletteTransform{
  public:
    EffectConfetti(FuncGetPalette_t getPal = &Confetti);
    ~EffectConfetti();

  protected:
    void updateLeds(CRGB *leds, int numLeds);

    int getMaxStep() const;    
};


inline EffectConfetti::EffectConfetti(FuncGetPalette_t getPal): EffectPaletteTransform(getPal) {
  setSpeedDelay(20);
}

inline EffectConfetti::~EffectConfetti(){  
}


#define CONFETTI_MAX_STEPS 100

inline int EffectConfetti::getMaxStep() const{
  return CONFETTI_MAX_STEPS;
}


#define LEDS_MAX 150

inline void EffectConfetti::updateLeds(CRGB *leds, int numLeds){
  //Fade all
  fadeToBlackBy(leds, numLeds, 8);                     

  //Once per each LEDS_MAX leds
  int cnt = numLeds / LEDS_MAX + 1;
  for(int i = 0; i < cnt; i++){
    int ledIndex = random16(cnt * LEDS_MAX);

    if(ledIndex < numLeds)
      leds[ledIndex] = getCurrentPalColor(random8());
  } 
  
}


////////////////////////////////////////
// Palettes for different type of transformation


/////////////////////
//Christmas
BEGIN_TRANSFORM_SCHEMA_GRADIENT_PALETTE(TransformChristmas)
  TRANSOFRM_PALETTE(christmattree1_gp)
END_TRANSFORM_SCHEMA()

////////////////////
// Autumn
BEGIN_TRANSFORM_SCHEMA_GRADIENT_PALETTE(TransformAutunm)
  TRANSOFRM_PALETTE(es_autumn_01_gp)
  TRANSOFRM_PALETTE(es_autumn_03_gp)
END_TRANSFORM_SCHEMA()

///////////////////
//Halloween - better autumn than autumn
BEGIN_TRANSFORM_SCHEMA_GRADIENT_PALETTE(TransformHalloween)
  TRANSOFRM_PALETTE(halloween_gp)
END_TRANSFORM_SCHEMA()


///////////////////
//Patriot - US 4th of July
BEGIN_TRANSFORM_SCHEMA_GRADIENT_PALETTE(July4th)
  TRANSOFRM_PALETTE(july4_2_gp)
END_TRANSFORM_SCHEMA()


///////////////////
//Valentines day
BEGIN_TRANSFORM_SCHEMA_GRADIENT_PALETTE(ValentinesDay)
  TRANSOFRM_PALETTE(firstlove_gp)
  TRANSOFRM_PALETTE(roseedan_gp)
  TRANSOFRM_PALETTE(passionata_gp)
END_TRANSFORM_SCHEMA()


#endif //__NOISE_H
