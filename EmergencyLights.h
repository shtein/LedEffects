#ifndef __EMERGENCYLIGHTS_H
#define __EMERGENCYLIGHTS_H

//////////////////////////////////////
// Emergency lighs

class EffectEmergencyLights: public Effect{
  public:
    EffectEmergencyLights();
   ~EffectEmergencyLights();

    void proceed(CRGB *leds, int numLeds); 
    void reset();

  protected:
    uint8_t _step;
};


inline EffectEmergencyLights::EffectEmergencyLights(){  
  setSpeedDelay(50); 
}

inline EffectEmergencyLights::~EffectEmergencyLights(){  
}

inline void EffectEmergencyLights::reset(){
  _step = 0;
}

#define EL_BLINKS         5               //Number of ons and offs
#define EL_CYCLES         (EL_BLINKS * 2) //Number of steps per one led set

inline void EffectEmergencyLights::proceed(CRGB *leds, int numLeds){ 

  //Black all
  setAll(leds, numLeds, 0, 0, 0);

  int bounds[][2] = { { 0, numLeds/2 }, 
                      { 1, numLeds/2 }, 
                      { numLeds / 2 + 1, numLeds }, 
                      { numLeds / 2, numLeds },                       
                    };

  //Even - on, odd - off
  //EL_BLINKS ties each

  //Only even step turns it on
  if(_step % 2 == 0){   
    
    int *p   = bounds[_step / EL_CYCLES];
    CRGB clr = CRGB::Blue;
    
    for(int i = p[0]; i < p[1]; i+= 2) {      
      leds[i] = clr;
    }
  }
  
  _step  = (_step + 1) % (EL_CYCLES * (sizeof(bounds) / sizeof(bounds[0])) );
}


#endif //__#ifndef __EMERGENCYLIGHTS_H

