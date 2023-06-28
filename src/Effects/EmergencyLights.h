#ifndef __EMERGENCYLIGHTS_H
#define __EMERGENCYLIGHTS_H

//////////////////////////////////////
// Emergency lighs

class EffectEmergencyLights: public Effect{
protected:
    void proceed(CRGB *leds, uint16_t numLeds); 
    void reset();
};


inline void EffectEmergencyLights::reset(){
  _ctx.step = 0;
  setSpeedDelay(50); 
}

#define EL_BLINKS         5               //Number of ons and offs
#define EL_CYCLES         (EL_BLINKS * 2) //Number of steps per one led set

inline void EffectEmergencyLights::proceed(CRGB *leds, uint16_t numLeds){ 

  //Black all
  fill_solid(leds, numLeds, CRGB::Black);

  uint16_t bounds[][2] = { { 0, (uint16_t)(numLeds / 2) }, 
                           { 1, (uint16_t)(numLeds / 2) }, 
                           { (uint16_t)(numLeds / 2 + 1), numLeds }, 
                           { (uint16_t)(numLeds / 2), numLeds },                       
                         };

  //Even - on, odd - off
  //EL_BLINKS ties each

  //Only even step turns it on
  if(_ctx.step % 2 == 0){   
    
    uint16_t *p  = bounds[_ctx.step / EL_CYCLES];
    CRGB clr     = CRGB::Blue;
    
    for(uint16_t i = p[0]; i < p[1]; i+= 2) {      
      leds[i] = clr;
    }
  }
  
  _ctx.step  = (_ctx.step + 1) % (EL_CYCLES * (sizeof(bounds) / sizeof(bounds[0])) );
}


#endif //__#ifndef __EMERGENCYLIGHTS_H

