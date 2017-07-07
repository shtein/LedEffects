#include "precomp.h"
#include "effect.h"


Effect::Effect(){
  _speedDelay = 25;
}

Effect::~Effect(){
}


void Effect::init(CRGB *leds, int numLeds){  

  setAll(leds, numLeds, CRGB::Black);
  
  reset();
}


void Effect::loop(CRGB *leds, int numLeds){  
  
  //proceed
  proceed(leds, numLeds);
 
}


CRGB Effect::getColor() const{
  return _hsv;
}

CHSV Effect::getHSV() const{
  return _hsv;
}

void Effect::setHSV(const CHSV &hsv){
  _hsv   = hsv;
}

    
void Effect::setRandomColor(){
  setHSV(CHSV(random(256), 0xFF, 0xFF));
}

void Effect::setSpeedDelay(int speedDelay){
  _speedDelay = speedDelay;
}

int Effect::getSpeedDelay() const{
  return _speedDelay;
}
