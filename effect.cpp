#include "precomp.h"
#include "effect.h"

Effect::Effect(){
  _numLeds = 0;
  _leds    = NULL;
  _millis   = 0;
}

Effect::~Effect(){
}

void Effect::init(CRGB *leds, int numLeds){
  _leds    = leds;
  _numLeds = numLeds;

  setAll(CRGB::Black);
  nextProceedIn(0);

  reset();
}


void Effect::loop(int speedDelay){
  //Check timer
  if(!timeToProceed())
    return;
    
  proceed(speedDelay);

  //if next time to proceed is not set then set it by default
  if(timeToProceed())
    nextProceedIn(speedDelay);
}

void Effect::nextProceedIn(int delta){
  _millis = millis() + delta;
}

bool Effect::timeToProceed() const{ 
  return _millis <= millis() ? true: false;
}
    
void Effect::setPixel(int led, byte red, byte green, byte blue) {
   _leds[led].r = red;
   _leds[led].g = green;
   _leds[led].b = blue;
}

void Effect::setAll( byte red, byte green, byte blue) {
  CRGB color(red, green, blue);
  fill_solid(_leds, _numLeds, color);
}

void Effect::setPixel(int led, const CRGB &color) {
   _leds[led] = color;
}

void Effect::setAll(const CRGB &color) {
    fill_solid(_leds, _numLeds, color);
}


CRGB Effect::selectRandomColor() const {
  CRGB color(0xFF00FF);
  color.setHue(random(256));
   
  return color;
}


