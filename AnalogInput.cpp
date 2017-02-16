#include "precomp.h"
#include "AnalogInput.h"

Potentiometer::Potentiometer(int pin, float min, float max){
  _pin = pin;
  _min = min;
  _max = max;
}

Potentiometer::~Potentiometer(){
}

float Potentiometer::value() const{
  int val = analogRead(_pin);

  return _min + (val - POT_MIN) * (_max - _min) / (POT_MAX - POT_MIN);
}
