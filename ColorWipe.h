#ifndef __COLORWIPE_H
#define __COLORWIPE_H

#endif //__COLORWIPE_H

//////////////////////////////
// Color Wipe Effect

class EffectColorWipe: public Effect{
  public:
    EffectColorWipe();
    ~EffectColorWipe();

  protected:
    void proceed(int speedDelay); 
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
 
inline void EffectColorWipe::proceed(int speedDelay){
  setPixel(_step % getNumLeds(), getColor());

  _step++;
  
  if(_step == getNumLeds()) { //wipe colors
    setHSV(CHSV(0, 0, 0));
  }
  else if (_step == 2 * getNumLeds()) { //set color
    _step = 0;
    setRandomColor();
  }
}
    
