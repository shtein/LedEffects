#ifndef __COLORWIPE_H
#define __COLORWIPE_H

//////////////////////////////
// Color Wipe Effect
class EffectColorWipe: public EffectColor{
  public:
    EffectColorWipe();
    ~EffectColorWipe();

  protected:
    void proceed(CRGB *leds, int numLeds); 
    void reset();
  
};


inline EffectColorWipe::EffectColorWipe(){
  _ctx.step = 0;
  setSpeedDelay(25);
}


inline EffectColorWipe::~EffectColorWipe(){
}

void EffectColorWipe::reset(){
  _ctx.step = 0;
  setRandomColor();
}
 
inline void EffectColorWipe::proceed(CRGB *leds, int numLeds){
  setPixel(leds[_ctx.step % numLeds], getColor());

  _ctx.step++;
  
  if(_ctx.step == numLeds) { //wipe colors
    setHSV(CHSV(0, 0, 0));
  }
  else if (_ctx.step == 2 * numLeds) { //set color
    _ctx.step = 0;
    setRandomColor();
  }
}

#endif //__COLORWIPE_H
    
