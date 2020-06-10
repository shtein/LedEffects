#ifndef __SOUND_H
#define __SOUND_H


////////////////////////////////
// EffectSoundSimple - very basic sound effect for testing

class EffectSoundSimple: public EffectSound<Effect>{

  protected:
   void updateLeds(CRGB *leds, int numLeds, const SoundCaptureData &data); 
};

void EffectSoundSimple::updateLeds(CRGB *leds, int numLeds, const SoundCaptureData &data){

  //Number of sections
  int inc = numLeds / MAX_BANDS;
    

  for(int i = 0; i < MAX_BANDS; i++){
    for(int j = 0; j < inc; j++) {
      leds[i * inc + j] = CHSV(HUE_RED, 0xFF, data.bands[i]);   
    }
  }  
}

#endif//__SOUND_H
