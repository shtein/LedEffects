#ifndef __SOUND_H
#define __SOUND_H

#include "SoundFHT.h"

//////////////////////
// Basic saund reactive effect
class EffectSound: public EffectSoundFHT<Effect>{
  public:
    void proceed(CRGB *leds, int numLeds);
};


inline void EffectSound::proceed(CRGB *leds, int numLeds){
  //Magic to translate spector to color
  const uint8_t *out = CFHT::_instance.getLogOut();
  
  for (int i = 0; i < numLeds; i++){                         
    int tmp = qsuba(out[2 * i + 2], 32);            
    if (tmp > (leds[i].r + leds[i].g + leds[i].b))            
        leds[i] = CHSV(tmp * 10, 255, tmp * 10); 
        
    leds[i].nscale8(224);                                     
  }
}




#endif//__SOUND_H
