#ifndef __COLORWIPE_H
#define __COLORWIPE_H

//////////////////////////////
// Color Wipe Effect
class EffectColorWipe: public Effect{
protected:
  void proceed(CRGB *leds, uint16_t numLeds){
    leds[_ctx.step % numLeds] =  _ctx.hsv;

    _ctx.step++;

    if((uint16_t)_ctx.step == numLeds) { //wipe colors
      _ctx.hsv = CHSV(0, 0, 0);
    }
    else if ((uint16_t)_ctx.step == (2 * numLeds) ) { //set color
      _ctx.step = 0;
      setRandomColor(_ctx.hsv);
    }
  } 
  
  void reset(){
    _ctx.step = 0;
    setRandomColor(_ctx.hsv);
    setSpeedDelay(25);
  }
};

 

#endif //__COLORWIPE_H
    
