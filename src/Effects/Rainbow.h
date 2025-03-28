#ifndef __RAINBOW_H
#define __RAINBOW_H


#define RAINBOW_HUE _ctx.byte

//////////////////////////////
// Rainbow effect 

class EffectRainbow: public Effect{
  protected:

    void reset(){
      RAINBOW_HUE = 0;
      setSpeedDelay(75);
    }

    void proceed(CRGB *leds, uint16_t numLeds){
      //Get current colors  
      uint8_t delta = numLeds >= 255 ? 1 : 255 / numLeds;

      //Move forward and save
      RAINBOW_HUE = (RAINBOW_HUE + delta) & 0xFF;
       
      //Set colors - same as fill_rainbow
      uint8_t hue = RAINBOW_HUE;
      
      for(uint16_t i = 0; i < numLeds; i++){ 
        leds[i].setHue(hue);
        hue = (hue + delta) & 0xFF;
      }
    } 
};




/////////////////////////////////////
// Theater Chase Rainbow Effect
class EffectTheaterChaseRainbow: public Effect{  
  protected:
    void reset(){
      _ctx.step = 0;  
      RAINBOW_HUE = 0;
      setSpeedDelay(100);  
    }

    void proceed(CRGB *leds, uint16_t numLeds){
      //Turn every third pixell off
      for(uint16_t i = 0; i < numLeds; i = i + 3){
          leds[(i + _ctx.step) % numLeds] = CRGB::Black;          
      }

      uint8_t delta = numLeds >= 255 ? 1 : 255 / numLeds;
      
      //Move forward
      _ctx.step  = (_ctx.step + 1) % 3;
      if (_ctx.step  == 0){ // Next cycle    
        
        //Move to the next color
        RAINBOW_HUE = (RAINBOW_HUE + delta) & 0xFF;
      }

      uint8_t hue = RAINBOW_HUE;
      //Turn every third pixell on
      for(uint16_t i = 0; i < numLeds; i = i + 3){ 
        leds[(i + _ctx.step ) % numLeds].setHue(hue);
        hue = (hue + delta) & 0xFF;    
      }
    }
};



/////////////////////////
// EffectRainbowMove

class EffectRainbowMove: public Effect{
  protected:
    void proceed(CRGB *leds, uint16_t numLeds); 
    void reset();
};


inline void EffectRainbowMove::reset(){
  setSpeedDelay(50);
}

void EffectRainbowMove::proceed(CRGB *leds, uint16_t numLeds){
  uint8_t beatA = beatsin8(17, 0, 255);                        
  uint8_t beatB = beatsin8(13, 0, 255);
  
  fill_rainbow(leds, numLeds, (beatA + beatB) / 2, 8);
}



#endif //__RAINBOW_H
