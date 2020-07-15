#ifndef __SOUND_H
#define __SOUND_H

#include "SoundCapture.h"


////////////////////////////////
// EffectSoundSimple - very basic sound effect for testing

class EffectSoundSimple: public EffectSound{

  protected:
   void updateLeds(CRGB *leds, int numLeds, const band8_visual &data);

  private:
    void drawBand(uint8_t volume, 
                  CRGB *leds,
                  int numLeds,
                  int center,
                  uint8_t band, 
                  uint8_t maxBands
                 );

  private:
    static CRGBPalette16 _pal;                 
};


DEFINE_GRADIENT_PALETTE( sound_bands_gp ) {
      0,  255,    0,    0,   //red
    128,    0,  255,    0,   //green
    255,    0,    0,  255    //blue
}; 

CRGBPalette16 EffectSoundSimple::_pal(sound_bands_gp);

inline void EffectSoundSimple::updateLeds(CRGB *leds, int numLeds, const band8_visual &data){

  //Number of sections
  int bandLeds = numLeds / MAX_BANDS;
    
 
 for(int i = 0; i < MAX_BANDS; i++){
  drawBand(data.bands[i], 
           leds + i * bandLeds, 
           bandLeds, 
           i % 2 == 0 ? bandLeds - 2 : 1,
           i, 
           MAX_BANDS);
 }

}


#define SS_FADE_FACTOR_MIN 10
#define SS_FADE_FACTOR_MAX 50

inline void EffectSoundSimple::drawBand(uint8_t volume, 
                                        CRGB *leds,
                                        int numLeds,
                                        int center,
                                        uint8_t band, 
                                        uint8_t maxBands) {

  uint8_t colorIndexFirst = map(band, 0, maxBands, 0, 255 );
  uint8_t colorIndexLast  = map(band + 1, 0, maxBands, 0, 255 );                                          
  uint8_t colorInc        = (colorIndexLast - colorIndexFirst) / numLeds;

  //Draw bottom

  //Number of leds from 0 to center - 1
  int bandLeds       = center + 1;
  //Volume step
  uint8_t ledInc     = 255 / bandLeds;
  //Fading
  uint8_t fadeFactor = (SS_FADE_FACTOR_MAX - SS_FADE_FACTOR_MIN) / bandLeds;
  
  for(int i = 0; i < center; i++){    

    if(volume > ledInc * (center - i))
      leds[i] = ColorFromPalette(_pal, colorIndexFirst + i * colorInc);
    else
      leds[i].fadeToBlackBy(SS_FADE_FACTOR_MIN + (i + 1) * fadeFactor);    
  }
 
  //Draw top

  //Number of leds from center to numLeds - 1
  bandLeds   = numLeds - center + 1;
  //Volume step
  ledInc     = 255 / bandLeds;
  //Fading
  fadeFactor = (SS_FADE_FACTOR_MAX - SS_FADE_FACTOR_MIN) / bandLeds;
  
  for(int i = center; i < numLeds; i++){     

     if(volume > ledInc * (i - center ))
      leds[i] = ColorFromPalette(_pal, colorIndexFirst + i * colorInc);
    else
      leds[i].fadeToBlackBy(SS_FADE_FACTOR_MIN + (i - center) * fadeFactor);
  }  
}

#endif//__SOUND_H
