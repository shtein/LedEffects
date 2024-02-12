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

#ifndef RIPPLE_COUNT
  #define RIPPLE_COUNT 5
#endif // RIPPLE_COUNT

class EffectRipple: public Effect{
protected:
  void reset(){
      setSpeedDelay(80);
  }
  void proceed(CRGB *leds, uint16_t numLeds){
    
    //Fade current
    fadeToBlackBy(leds, numLeds, RIPPLE_FADE);  

    //Proceed with Ripples
    for(int i = 0; i < RIPPLE_COUNT; i++){
      Ripple &ripple = _ripples[i];
    
      if(random16(RIPPLE_MAX - RIPPLE_MAX_DELTA, RIPPLE_MAX + RIPPLE_MAX_DELTA) <= ripple.getOffset()){

        //Setup ripple      
        ripple.setup(random16(i * numLeds / RIPPLE_COUNT, (i + 1) * numLeds / RIPPLE_COUNT), 
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

protected:
  Ripple _ripples[RIPPLE_COUNT];
};






#endif //EFFECT_RIPPLE
