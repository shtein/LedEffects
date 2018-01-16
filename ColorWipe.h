#ifndef __COLORWIPE_H
#define __COLORWIPE_H

//////////////////////////////
// Color Wipe Effect

class EffectColorWipe: public Effect{
  public:
    EffectColorWipe();
    ~EffectColorWipe();

  protected:
    void proceed(CRGB *leds, int numLeds); 
    void reset();
   
  protected:
    int  _step; //cycle
};


inline EffectColorWipe::EffectColorWipe(){
  _step = 0;
  setSpeedDelay(25);
}


inline EffectColorWipe::~EffectColorWipe(){
}

void EffectColorWipe::reset(){
  _step = 0;
  setRandomColor();
}
 
inline void EffectColorWipe::proceed(CRGB *leds, int numLeds){
  setPixel(leds[_step % numLeds], getColor());

  _step++;
  
  if(_step == numLeds) { //wipe colors
    setHSV(CHSV(0, 0, 0));
  }
  else if (_step == 2 * numLeds) { //set color
    _step = 0;
    setRandomColor();
  }


}

#endif //__COLORWIPE_H
    
