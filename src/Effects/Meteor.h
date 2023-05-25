#ifndef __METEOR_H
#define __METEOR_H


#define METEOR_SIZE_MIN 2
#define METEOR_SIZE_MAX 5

#define METEOR_SPEED_MIN 2
#define METEOR_SPEED_MAX 3


/////////////////////////
// Meteor object
class Meteor{
public:
  Meteor();
 ~Meteor();

  void setup(uint8_t vel, uint8_t size);
  void setupRandom();
  void move();

  uint16_t getTop() const;
  uint16_t getBottom() const;

  uint16_t getVisibleTop(uint16_t boundary) const;
  uint16_t getVisibleBottom(uint16_t boundary) const;

  CHSV getHSV() const;

protected:
  uint16_t _pos:10;  //current position
  uint16_t _vel:3;   //meteor velocity
  uint16_t _size:3;  //meteor size
  uint8_t  _hue;     //meteor color
};

inline Meteor::Meteor(){
  _pos  = 0;
  _vel  = 0;
  _size = 0;
  _hue  = 0;
}

inline Meteor::~Meteor(){
}

inline void Meteor::setup( uint8_t vel, uint8_t size){
  _pos  = 0;
  _vel  = vel;
  _size = size;
  _hue  = random8();
}


inline void Meteor::setupRandom(){
  setup(random8(METEOR_SPEED_MIN, METEOR_SPEED_MAX + 1), random8(METEOR_SIZE_MIN, METEOR_SIZE_MAX + 1));
}

inline void Meteor::move(){
  _pos += _vel;
}

inline uint16_t Meteor::getTop() const{
  return _pos;
}

inline uint16_t Meteor::getBottom() const{
  //Never return negative value
  return getTop() > _size ? getTop() - _size : 0;
}

inline uint16_t Meteor::getVisibleTop(uint16_t boundary) const{
  return getTop() > boundary ? boundary : getTop();
}

inline uint16_t Meteor::getVisibleBottom(uint16_t boundary) const{
  return getBottom() > boundary ? boundary : getBottom();
}

inline CHSV Meteor::getHSV() const{
  return CHSV(_hue, 0xFF, 0xFF);
}

///////////////////////////
// Effect Meteor Rain

template <const size_t MAX_METEORS = 2>
class EffectMeteorRain: public Effect{
protected:
  void reset();
  void proceed(CRGB *leds, uint16_t numLeds);

protected:

  Meteor _meteors[MAX_METEORS];
};


template <const size_t MAX_METEORS>
inline void EffectMeteorRain<MAX_METEORS>::reset(){    

  for(uint16_t i = 0; i < MAX_METEORS; i++){
    //Ret meteor position
    _meteors[i].setupRandom();
  }

  setSpeedDelay(50);
}


template <const size_t MAX_METEORS>
void EffectMeteorRain<MAX_METEORS>::proceed(CRGB *leds, uint16_t numLeds){
  //Fade it all first
  for(uint16_t i = 0; i < numLeds; i++ ){
    
    if(random(9) > 2) //Some randomization
      leds[i].fadeToBlackBy(64);
  }

  //Proceed with meteors
  uint8_t done = 0;  
  for(size_t i = 0; i < MAX_METEORS; i++){

    Meteor &meteor = _meteors[i];
    
    //Draw the meteor 
    int16_t top    = i % 2 == 0 ? meteor.getVisibleTop(numLeds) : numLeds - meteor.getVisibleBottom(numLeds);
    int16_t bottom = i % 2 == 0 ? meteor.getVisibleBottom(numLeds) : numLeds - meteor.getVisibleTop(numLeds); 

    CHSV hsv = meteor.getHSV();
    hsv.hue += bottom;
    for (int16_t j = bottom; j < top; j++){      
      hsv.hue ++;
      leds[j] += hsv;
    }

    //Move meteor
    meteor.move();

    //If meteor reaches, rerun it
    if(meteor.getBottom() > (uint16_t)numLeds){
      done ++;
    }    
  }

  //Start meteors
  if(done == MAX_METEORS){
    for(uint16_t i = 0; i < MAX_METEORS; i++){
      //Ret meteor position
      _meteors[i].setupRandom();
    }

  }
  
}



#endif // __METEOR_H
