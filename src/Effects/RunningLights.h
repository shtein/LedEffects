#ifndef __RUNNING_LIGHS
#define __RUNNING_LIGHS

////////////////////
// Running Lights Effect

#define RL_SIZE 6

class EffectRunningLights: public EffectColor{
  protected:
    void reset(){
      _ctx.step = 0;
      setSpeedDelay(50);
    }

    void proceed(CRGB *leds, uint16_t numLeds){
      CHSV hsv(_cfg.bytes[0],  _cfg.bytes[1], _cfg.bytes[2]);
      
      for(uint16_t i = 0; i < numLeds; i++) {               
        hsv.v  = sin8( ((i + _ctx.step) % RL_SIZE) * 255 / (RL_SIZE - 1) );
        leds[i] =  hsv;
      }  
      
      _ctx.step = (_ctx.step + 1) % RL_SIZE;
    }
    
};





#endif //__RUNNING_LIGHS
