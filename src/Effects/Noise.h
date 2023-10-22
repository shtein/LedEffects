#ifndef __NOISE_H
#define __NOISE_H



class EffectPaletteTransformFast: public EffectPaletteTransform {
  public:
    EffectPaletteTransformFast(FuncGetPalette_t getPal = FuncGetPal_Default):
      EffectPaletteTransform(getPal){}

  protected:
    void proceed(CRGB *leds, uint16_t numLeds){
      //Call parrent
      EffectPaletteTransform::proceed(leds, numLeds);

      //Do full range for current palette
      for(uint16_t i = 0; i < numLeds; i++){       
        leds[i]  = getCurrentPalColor((uint8_t)map(i, 0, numLeds - 1, 0, 255));
      }
    }    

    int getMaxStep() const{
      return MAX_PAL_CHANGES * 3;
    }
};


////////////////////////////
//EffectNoise
#define NOISE_DIST Effect::_ctx.value

#define XSCALE          30
#define YSCALE          30

class EffectNoise: public EffectPaletteTransform{
  public:
    EffectNoise(FuncGetPalette_t getPal = FuncGetPal_Default):
      EffectPaletteTransform(getPal){}

  protected:
    void proceed(CRGB *leds, uint16_t numLeds){
      //Call parrent
      EffectPaletteTransform::proceed(leds, numLeds);

      //Do default actions
      for(uint16_t i = 0; i < numLeds; i++){       
        leds[i]  = getCurrentPalColor((uint8_t)( inoise8(i * XSCALE, NOISE_DIST + i * YSCALE) % 255 ));
      }

  
      //Prepare for the next move
      NOISE_DIST += beatsin8(10, 1, 4);                                               
    }

    void reset(){
      EffectPaletteTransform::reset();

      //Init distortion
      NOISE_DIST = random16(millis());
    }
};


////////////////////////////////////////
// EffectPlasma
#define PLASMA_MAX_STEPS 100

inline void FuncGetPal_Plazma(CRGBPalette16 &pal){

  uint8_t clr = random8();
  pal =  CRGBPalette16(CHSV(clr + random8(32), 192, random8(128,255)), 
                       CHSV(clr + random8(32), 255, random8(128,255)), 
                       CHSV(clr + random8(32), 192, random8(128,255)), 
                       CHSV(clr + random8(32), 255, random8(128,255))
                      );
}

class EffectPlasma: public EffectPaletteTransform {
  public:  
    EffectPlasma(FuncGetPalette_t getPal = FuncGetPal_Plazma):
      EffectPaletteTransform(getPal){}


  protected:
    void proceed(CRGB *leds, uint16_t numLeds){
      EffectPaletteTransform::proceed(leds, numLeds);
        
      int phase1 = beatsin8(6,-64, 64);
      int phase2 = beatsin8(7,-64, 64);

      for(uint16_t i = 0; i < numLeds; i++){
        int clrIndex      = cubicwave8( (i * 23) + phase1) / 2 + cos8( (i * 15) + phase2) / 2;
        int clrBrightness = qsuba(clrIndex, beatsin8(7, 0, 96));
        
        leds[i] = getCurrentPalColor(clrIndex, clrBrightness);
      }  
    }

    void reset(){
      EffectPaletteTransform::reset();
      Effect::setSpeedDelay(50);  
    }

    int getMaxStep() const{
      return PLASMA_MAX_STEPS;
    }
};


//////////////////////////////
// Effect Confetti


BEGIN_TRANSFORM_SCHEMA_RGB16_PALETTE(FuncGetPal_Confetti)
  TRANSOFRM_PALETTE(OceanColors_p)
  TRANSOFRM_PALETTE(LavaColors_p)
  TRANSOFRM_PALETTE(ForestColors_p)
  TRANSOFRM_PALETTE(CloudColors_p)
END_TRANSFORM_SCHEMA()

#define CONFETTI_MAX_STEPS 100
#define LEDS_MAX 150

class EffectConfetti: public EffectPaletteTransform{
  public:
    EffectConfetti(FuncGetPalette_t getPal = FuncGetPal_Confetti):
      EffectPaletteTransform(getPal){}


  protected:
    void proceed(CRGB *leds, uint16_t numLeds){  
      EffectPaletteTransform::proceed(leds, numLeds);
      //Fade all
      fadeToBlackBy(leds, numLeds, 8);                     

      //Once per each LEDS_MAX leds
      int cnt = numLeds / LEDS_MAX + 1;
      for(int i = 0; i < cnt; i++){
        uint16_t ledIndex = random16(cnt * LEDS_MAX);

        if(ledIndex < numLeds)
          leds[ledIndex] = getCurrentPalColor(random8());
      } 

    }

    void reset(){
      EffectPaletteTransform::reset();
      Effect::setSpeedDelay(20);
    }

    int getMaxStep() const{
      return CONFETTI_MAX_STEPS;
    }

};

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


///////////////////
//Ukraine Flag
BEGIN_TRANSFORM_SCHEMA_RGB16_PALETTE(UkraineFlag)
  TRANSOFRM_PALETTE(UkraineFlag_p)
END_TRANSFORM_SCHEMA()

///////////////////
//Israel Flag
BEGIN_TRANSFORM_SCHEMA_RGB16_PALETTE(IsraelFlag)
  TRANSOFRM_PALETTE(IsraelFlag_p)
END_TRANSFORM_SCHEMA()

///////////////////
//Portugal Flag
BEGIN_TRANSFORM_SCHEMA_RGB16_PALETTE(PortugalFlag)
  TRANSOFRM_PALETTE(PortugalFlag_p)
END_TRANSFORM_SCHEMA()


#endif //__NOISE_H
