#ifndef __JUGGLE_H
#define __JUGGLE_H


class EffectJuggle: public Effect{
  public:
     EffectJuggle(); 
    ~EffectJuggle();

    void proceed(CRGB *leds, uint16_t numLeds); 
    void reset();
};

inline EffectJuggle::EffectJuggle(){
  setSpeedDelay(20);
}

inline EffectJuggle::~EffectJuggle(){
}

inline void EffectJuggle::proceed(CRGB *leds, uint16_t numLeds){

  fadeToBlackBy(leds, numLeds, 20);

  byte hueInc = 0;
  for( int i = 0; i <  8; i++) {
    leds[beatsin16(i + 7, 0, numLeds - 1)] |= CHSV(hueInc, 200, 255);
    hueInc += 32;
  }
}

inline void EffectJuggle::reset(){  
}


#endif //__JUGGLE_H