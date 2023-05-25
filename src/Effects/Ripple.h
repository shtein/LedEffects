#ifndef __EFFECT_RIPPLE
#define __EFFECT_RIPPLE


////////////////////////////////////
// Ripple object

class Ripple{
public:
  Ripple();
 ~Ripple();

  void setup(int center, uint8_t offset, uint8_t colorIndex);
  void move();

  uint16_t getCenter() const;
  uint16_t getOffset() const;
  uint8_t  getColorIndex() const;

protected:
  uint16_t _center:10;    //Drop center
  uint16_t _offset:6;     //Drop current offset
  uint8_t  _colorIndex;   //Color index, could be hue or an index in a palette
};

inline Ripple::Ripple(){
  _center     = 0;
  _offset     = 0;
  _colorIndex = 0;
}

inline Ripple::~Ripple(){
}


inline void Ripple::setup(int center, uint8_t offset, uint8_t colorIndex){
  _center        = center;
  _offset        = offset;
  _colorIndex    = colorIndex;
}

inline void Ripple::move(){
  _offset ++;
}

inline uint16_t Ripple::getCenter() const{
  return _center;  
}

inline uint16_t Ripple::getOffset() const{
  return _offset;
}

inline uint8_t Ripple::getColorIndex() const{
  return _colorIndex;
}



////////////////////////////////////
// Effect Ripple


#define RIPPLE_FADE        128
#define RIPPLE_MAX         16
#define RIPPLE_MAX_DELTA   4

template <const int MAX_RIPPLES = 5>
class EffectRipple: public Effect{

protected:
  void reset();
  void proceed(CRGB *leds, uint16_t numLeds);

protected:
  Ripple _ripples[MAX_RIPPLES];
};


template <const int MAX_RIPPLES>
inline void EffectRipple<MAX_RIPPLES>::reset(){
  setSpeedDelay(80);
}

template <const int MAX_RIPPLES>
inline void EffectRipple<MAX_RIPPLES>::proceed(CRGB *leds, uint16_t numLeds){
  
  //Fade current
  fadeToBlackBy(leds, numLeds, RIPPLE_FADE);  

  //Proceed with Ripples
  for(int i = 0; i < MAX_RIPPLES; i++){
    Ripple &ripple = _ripples[i];
  
    if(random16(RIPPLE_MAX - RIPPLE_MAX_DELTA, RIPPLE_MAX + RIPPLE_MAX_DELTA) <= ripple.getOffset()){

       //Setup ripple      
       ripple.setup(random16(i * numLeds / MAX_RIPPLES, (i + 1) * numLeds / MAX_RIPPLES), 
                    random8(0, RIPPLE_MAX_DELTA), 
                    random8()
                   );

       //Show center
       leds[ripple.getCenter()] = CHSV(ripple.getColorIndex(), 255, 255);
    }
    else{
        //Move first
        ripple.move();

        //Prepare color        
        CHSV hsv(ripple.getColorIndex(), 255, 255);
        hsv.hue        += ripple.getOffset() * 2;
        hsv.saturation -= ripple.getOffset() * 5;
        hsv.value       = cos8(map(ripple.getOffset(), 1, RIPPLE_MAX, 0, 128));

        //Light left led
        if(ripple.getCenter() >= ripple.getOffset()) 
          leds[ripple.getCenter() - ripple.getOffset()] += hsv;

        //Light right led
        if(ripple.getCenter() + ripple.getOffset() < numLeds) 
          leds[ripple.getCenter() + ripple.getOffset()] += hsv;

    }    
    
  }

}



#endif //EFFECT_RIPPLE
