#ifndef __MOODBLOBS_H
#define __MOODBLOBS_H


#define BLOB_SIZE  10
#define BLOB_PHASE _ctx.byte

class EffectMoodBlobs: public Effect{
protected:
    void reset(){
      _ctx.step  = 0;
      BLOB_PHASE = 0;
  
      setSpeedDelay(100);
    }

    void proceed(CRGB *leds, uint16_t numLeds){

      for(uint16_t i = 0; i < numLeds; i++){        
        //Calculate value
        int val = (int)sin8(-BLOB_PHASE + (i % BLOB_SIZE) * 255 / BLOB_SIZE) - 128;            
        //Calculate hue
        int index = (i + _ctx.step / 2) % numLeds;
        int hue   = ((int)sin8( index * 255 / numLeds) - 128 ) ;   
              
        //Set color
        //Everything below x axis is 0 for hue      
        leds[i].setHSV(hue, 0xFF, val < 0 ? 0 : val * 2);
      }

      //Increment blob phase
      BLOB_PHASE += 2;

      //Increment color rotation step
      _ctx.step = (_ctx.step + 1) % ( numLeds * 2 );
    }
};



#endif //__MOODBLOBS_H
