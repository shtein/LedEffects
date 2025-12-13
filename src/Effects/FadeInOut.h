#ifndef __FADEINOUT_H
#define __FADEINOUT_H

//////////////////////////////////////////
// Fade in and out effect

#define FIO_HUE _ctx.byte
#define FIO_VAL _ctx.step

class EffectFadeInOut: public Effect{
  protected:
    
    void reset(){
      FIO_HUE = 0;      
      FIO_VAL = 0;
      setSpeedDelay(50); 
    }

    void proceed(CRGB *leds, uint16_t numLeds){
      //See if we need to change color
      if( _ctx.step == 0){      

        //Init hue and value
        FIO_HUE = random8();
        FIO_VAL = 0xFF;
      }
      else{
        //Change value
        FIO_VAL -= 5;                    
      }

      //Set color
      CRGB rgb;
      rgb.setHSV(FIO_HUE, 0xFF, triwave8(FIO_VAL));
      fill_solid(leds, numLeds, rgb);
    }
};






#endif //__FADEINOUT_H
