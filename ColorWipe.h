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
}


inline EffectColorWipe::~EffectColorWipe(){
}

void EffectColorWipe::reset(){
  _step = 0;
  setRandomColor();
}
 
inline void EffectColorWipe::proceed(int speedDelay){
  setPixel(_step % _numLeds, _color);

  _step++;
  
  if(_step == _numLeds) { //wipe colors
    _color = 0x000000;
  }
  else if (_step == 2 * _numLeds) { //set color
    _step = 0;
    setRandomColor();
  }
}
    
