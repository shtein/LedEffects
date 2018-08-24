#ifndef __METEOR_H
#define __METEOR_H

/////////////////////////
// Meteor object
class Meteor{
public:
  Meteor();
 ~Meteor();

  void setup(uint8_t vel, uint8_t size);
  void move();

  uint16_t getTop() const;
  uint16_t getBottom() const;

  uint16_t getVisibleTop(uint16_t boundary) const;
  uint16_t getVisibleBottom(uint16_t boundary) const;

protected:
  uint16_t _pos:24;  //current position
  uint16_t _vel:3;   //meteor velocity
  uint16_t _size:5;  //meteor size
};

inline Meteor::Meteor(){
  _pos  = 0;
  _vel  = 0;
  _size = 0;
}

inline Meteor::~Meteor(){
}

inline void Meteor::setup( uint8_t vel, uint8_t size){
  _pos  = 0;
  _vel  = vel;
  _size = size;
}

inline void Meteor::move(){
  _pos += _vel;
}

uint16_t Meteor::getTop() const{
  return _pos;
}

uint16_t Meteor::getBottom() const{
  //Never return negative value
  return getTop() > _size ? getTop() - _size: 0;
}

uint16_t Meteor::getVisibleTop(uint16_t boundary) const{
  return getTop() > boundary ? boundary : getTop();
}

uint16_t Meteor::getVisibleBottom(uint16_t boundary) const{
  return getBottom() > boundary ? boundary : getBottom();
}


///////////////////////////
// Effect Meteor Rain

#define METEORS_NUMBER 2

#define METEOR_SIZE_MIN 2
#define METEOR_SIZE_MAX 5

#define METEOR_SPEED_MIN 1
#define METEOR_SPEED_MAX 3


class EffectMeteorRain: public Effect{
public:
  EffectMeteorRain();
 ~EffectMeteorRain();

protected:
  void reset();
  void proceed(CRGB *leds, int numLeds);

protected:
  Meteor _meteors[METEORS_NUMBER];
};


inline EffectMeteorRain::EffectMeteorRain(){
  setSpeedDelay(50);
  setHSV(CHSV(HUE_GREEN, 0xFF, 0xFF));
}

inline EffectMeteorRain::~EffectMeteorRain(){
}


inline void EffectMeteorRain::reset(){    

  for(int16_t i = 0; i < METEORS_NUMBER; i++){
    //Ret meteor position
    _meteors[i].setup(random8(METEOR_SPEED_MIN, METEOR_SPEED_MAX), random8(METEOR_SIZE_MIN, METEOR_SIZE_MAX));
  }

  setRandomColor();
}



void EffectMeteorRain::proceed(CRGB *leds, int numLeds){
  //Fade it all first
  for(int16_t i = 0; i < numLeds; i++ ){
    
    if(random(9) > 3) //Some randomization
      leds[i].fadeToBlackBy(64);
  }

  //Proceed with meteors
  uint8_t done = 0;  
  for(int16_t i = 0; i < METEORS_NUMBER; i++){

    Meteor &meteor = _meteors[i];
    
    //Draw the meteor 
    int16_t top    = i % 2 == 0 ? meteor.getVisibleTop(numLeds) : numLeds - meteor.getVisibleBottom(numLeds);
    int16_t bottom = i % 2 == 0 ? meteor.getVisibleBottom(numLeds) : numLeds - meteor.getVisibleTop(numLeds); 

    CHSV hsv = getHSV();
    hsv.hue = i * 128;
    for (int16_t j = bottom; j < top; j++){
      
      hsv.hue += j;
      leds[j] += hsv;
    }

    //Move meteor
    meteor.move();

    //If meteor reaches, rerun it
    if(meteor.getBottom() > numLeds){
      done ++;
    }    
  }

  //Start meteors
  if(done == METEORS_NUMBER){
    reset();
  }
  
}



#endif // __METEOR_H
