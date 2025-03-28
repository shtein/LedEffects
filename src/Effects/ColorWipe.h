#ifndef __COLORWIPE_H
#define __COLORWIPE_H

//////////////////////////////
// Color Wipe Effect

#define COLOR_WIPE_RGB _ctx.rgb

class EffectColorWipe: public Effect{
protected:
  void proceed(CRGB *leds, uint16_t numLeds){

    leds[_ctx.step % numLeds] =  COLOR_WIPE_RGB;
    _ctx.step++;

    if((uint16_t)_ctx.step == numLeds) { //wipe colors
      COLOR_WIPE_RGB = CRGB::Black;
    }
    else if ((uint16_t)_ctx.step == (2 * numLeds) ) { //set colors
      _ctx.step = 0;
      COLOR_WIPE_RGB.setHue(random8());
    }
  } 
  
  void reset(){
    _ctx.step = 0;
    COLOR_WIPE_RGB.setHue(random8());
    
    setSpeedDelay(25);
  }
};

 

#endif //__COLORWIPE_H
    
