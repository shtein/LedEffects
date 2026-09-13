#ifndef __JUGGLE_H
#define __JUGGLE_H


#define BASE_SPEED 8

class EffectJuggle: public Effect{
protected:
  void reset(){
    setSpeedDelay(20);
  }

  void proceed(CRGB *leds, uint16_t numLeds){      
    byte hueInc = 0;

#ifdef USE_MATRIX
    XYDraw xy(leds, numLeds);

    fadeToBlackBy(leds, numLeds, 120);

    for(int i = 0; i < xy.width(); i++) {
      
      uint8_t x = beatsin8(BASE_SPEED + i * 2, 0, xy.width() - 1);
      uint8_t y = beatsin8(BASE_SPEED * 2 + i * 2, 0, xy.height() - 1);

      xy(x, y) |= CHSV(hueInc, 255, 255);      
      
      hueInc += 8;
    }

#else    
    fadeToBlackBy(leds, numLeds, 20);

    for( int i = 0; i <  8; i++) {
      leds[beatsin16(i + 7, 0, numLeds - 1)] |= CHSV(hueInc, 200, 255);
      hueInc += 32;
    }

    #endif
  }

  
};




#endif //__JUGGLE_H