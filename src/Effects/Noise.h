#ifndef __NOISE_H
#define __NOISE_H


//getPal_Default
class EffectPaletteTransformFast: public EffectPaletteTransform {
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

//getPal_Default
class EffectNoise: public EffectPaletteTransform{
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

//getPal_Plazma
class EffectPlasma: public EffectPaletteTransform {
  
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



#define CONFETTI_MAX_STEPS 100
#define LEDS_MAX 150

//getPal_Confetti
class EffectConfetti: public EffectPaletteTransform{
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

#endif //__NOISE_H
