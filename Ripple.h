#ifndef __EFFECT_RIPPLE
#define __EFFECT_RIPPLE


#define RIPPLE_FADE        128
#define RIPPLE_MAX_FADE    255
#define RIPPLE_MAX         16
#define RIPPLE_MAX_DELTA   4


////////////////////////////////////
// Ripple object
class Ripple{
public:
  Ripple();
 ~Ripple();

  void setup(int first, int last);
  void reset();
  void move();

  uint16_t getCenter() const;
  uint16_t getOffset() const;
  uint16_t getLeft() const;
  uint16_t getRight() const;

  bool isOver() const;

  CHSV getHSV() const;
  
protected:
  uint16_t _center:10; //ripple center
  uint16_t _offset:6;  //ripple current offset
  uint8_t  _hue;
};

inline Ripple::Ripple(){
  _center = 0;
  _offset = 0;
}

inline Ripple::~Ripple(){
}

inline void Ripple::setup(int first, int last){
  _center = random16(first, last);
  _offset = random16(RIPPLE_MAX - RIPPLE_MAX_DELTA, RIPPLE_MAX + 1);
  _hue    = random8();
}

inline void Ripple::move(){
  _offset --;
}


inline void Ripple::reset(){
  _offset = 0;
}

inline bool Ripple::isOver() const{
  return _offset == 0 ? true : false;
}

inline uint16_t Ripple::getCenter() const{
  return _center;  
}

inline uint16_t Ripple::getOffset() const{
  return RIPPLE_MAX - _offset;
}

inline uint16_t Ripple::getLeft() const{
  return _center - getOffset();  
}

inline uint16_t Ripple::getRight() const{
  return _center + getOffset();
}

inline CHSV Ripple::getHSV() const{
  return CHSV(_hue, 0xFF, 0xFF);
}



////////////////////////////////////
// Effect Ripple

#define MAX_RIPPLES 5

class EffectRipple: public Effect{
public:
  EffectRipple();
  ~EffectRipple();  

protected:
  void reset();
  void proceed(CRGB *leds, int numLeds);


protected:
  Ripple _ripples[MAX_RIPPLES];
};

inline EffectRipple::EffectRipple(){
  setSpeedDelay(80);
}

inline EffectRipple::~EffectRipple(){
}

inline void EffectRipple::reset(){
  for(int i = 0; i < MAX_RIPPLES; i++){
    _ripples[i].reset(); 
  }
}


inline void EffectRipple::proceed(CRGB *leds, int numLeds){

  //Fade current
  fadeToBlackBy(leds, numLeds, RIPPLE_FADE);  

  //Proceed with Ripples
  for(int i = 0; i < MAX_RIPPLES; i++){
    Ripple &ripple = _ripples[i];


    if(ripple.isOver()){
      //Setup ripple      
      ripple.setup(i * numLeds / MAX_RIPPLES, (i + 1) * numLeds / MAX_RIPPLES ); //Fix bounds here
      
      //Show center
      leds[ripple.getCenter()] = ripple.getHSV();
    }
    else{
        //Prepare color
        CHSV hsv  = ripple.getHSV();
        hsv.hue        += ripple.getOffset() * 2;
        hsv.saturation -= ripple.getOffset() * 5;
        hsv.value       =  cos8(map(ripple.getOffset(), 1, RIPPLE_MAX, 0, 128));

        //Light left led
        if((int) ripple.getLeft() >= 0) 
          leds[ripple.getLeft()] += hsv;

        //Light right led
        if((int) ripple.getRight() < numLeds) 
          leds[ripple.getRight()] += hsv;
    }
    

    ripple.move();
  }

}


#endif //EFFECT_RIPPLE
