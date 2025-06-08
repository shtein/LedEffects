#ifndef __SOUND_H
#define __SOUND_H

#include <utils.h>
#include <SoundCapture.h>


////////////////////////////////////////////
// EffectSoundVUM - sound visualization with VU meter kind of display

#define SS_FADE_FACTOR_MIN 20
#define SS_FADE_FACTOR_DELTA(fade) (getSpeedDelay() * fade)


class EffectSoundVUM: public Effect{
public:
  virtual void reset(){
    setSpeedDelay(10);
    _ctx.palCurrent = sound_bands_gp;
  }

protected:

#ifdef USE_MATRIX
  void proceed(CRGB *leds, uint16_t numLeds){
    //Get sound data

    uint16_t bands[SC_MAX_BANDS];
    getSoundBands(bands, true);

    XY xy;

    size_t bandLeds = xy.height();
    size_t bandCols = xy.width() / SC_MAX_BANDS;
    size_t firstCol = ( xy.width() % SC_MAX_BANDS ) / 2;
    

    //Draw each band according to matrix configuration
    for(size_t i = 0; i < SC_MAX_BANDS; i++){

      uint8_t colorIndexFirst = mapEx(i, 0, SC_MAX_BANDS, 0, 255 );
      uint8_t colorIndexLast  = mapEx(i + 1, 0, SC_MAX_BANDS, 0, 255 );    

      //Bars
      for(size_t j = 0; j < bandCols; j++){  
        
        size_t first = xy.xy(i  * bandCols + j + firstCol, 0);
        size_t last  = xy.xy(i  * bandCols + j + firstCol, xy.height() - 1);
        
        size_t index = first > last ? last : first;

        if (EFFECT_PARAM_SOUNDVUM(_cfg) == sv_Symmetric){
          drawBandSymmetric(leds + index, bandLeds, bands[i], colorIndexFirst,  colorIndexLast, i < 2 ? 1 : i < 4 ? 2 : 3 );
        }else{          
          drawBand(leds + index, bandLeds, first > last ? 0 : bandLeds - 1, bands[i], colorIndexFirst,  colorIndexLast );
        }
        
      }           
    }           
  }

#else
  void proceed(CRGB *leds, uint16_t numLeds){
    //Get sound data

    uint16_t bands[SC_MAX_BANDS];
    getSoundBands(bands, true);

    //Number of sections
    int bandLeds = numLeds / SC_MAX_BANDS;
        
    
    for(int i = 0; i < SC_MAX_BANDS; i++){
      drawBand( leds + i * bandLeds, 
                bandLeds, 
                //i % 2 == 0 ? bandLeds - 2 : 1,
                //bandLeds - 2,
                //1,
                bandLeds / 2,
                //0,
                bands[i],            
                mapEx(i, 0, SC_MAX_BANDS, 0, 255 ),
                mapEx(i + 1, 0, SC_MAX_BANDS, 0, 255 )
              );
    }  
  }

#endif //USE_MATRIX    

//Draw bands
void drawBand(CRGB *leds,              //Leds
              uint16_t numLeds,        //Number of Leds, 
              uint16_t center,         //Center 
              uint16_t volume,          //Volumne 
              uint8_t colorIndexFirst, //Color index for the center of the bande
              uint8_t colorIndexLast,  //Color index for the side of the bande 
              uint8_t fade = 2         //Fade step multiplier
             ){
    // Draw center
    leds[center] = (volume > 0) ? ColorFromPalette(_ctx.palCurrent, colorIndexFirst, 255, NOBLEND) : leds[center].fadeToBlackBy(SS_FADE_FACTOR_MIN);

    // Calculate parameters for bottom part and top part
    uint16_t firstStep = SOUND_UPPER_MAX / max(center + 1, numLeds - center);

    // Draw bottom to center
    for (size_t i = 0; i < center; i++) {
        uint16_t step = firstStep + (SOUND_UPPER_MAX - firstStep) * (center - i - 1) / (center + 1);
        leds[i] = (volume > step) ? ColorFromPalette(_ctx.palCurrent, colorIndexLast - i * (colorIndexLast - colorIndexFirst) / center, 255, NOBLEND) : leds[i].fadeToBlackBy(SS_FADE_FACTOR_MIN + (center - i) * SS_FADE_FACTOR_DELTA(fade) / center);
    }

    // Draw center to top
    for (size_t i = center + 1; i < numLeds; i++) {
        uint16_t step = firstStep + (SOUND_UPPER_MAX - firstStep) * (i - center - 1) / (numLeds - center);
        leds[i] = (volume > step) ? ColorFromPalette(_ctx.palCurrent, colorIndexFirst + (i - center) * (colorIndexLast - colorIndexFirst) / (numLeds - center - 1), 255, NOBLEND) : leds[i].fadeToBlackBy(SS_FADE_FACTOR_MIN + (i - center) * SS_FADE_FACTOR_DELTA(fade) / (numLeds - center - 1));
    }
  }

  void drawBandSymmetric(CRGB *leds,              //Leds
                         uint16_t numLeds,        //Number of leds
                         uint16_t volume,          //Volume
                         uint8_t colorIndexFirst, //Color index of the center of the band
                         uint8_t colorIndexLast,  //Color index of the side if the bande
                         uint8_t fade = 2        //Fade step multiplier
                        ){

    //First led is in the middle
    CRGB *first = leds + numLeds / 2 + numLeds % 2;
    //Draw first half      
    drawBand(first, 
            numLeds / 2, 
            0,
            volume,
            colorIndexFirst,
            colorIndexLast,
            fade
          );

    //Copy the rest
    int offs =  1 - numLeds % 2;
    for(size_t i = 0;  i < numLeds / 2; i++)
      first[-i - offs] = first[i];
  } 
};



/////////////////////////////////////////////////////
// EffectSoundRGB

typedef bool(*FuncRGBMode_t)(uint16_t &, uint16_t, XYDraw &, const RightTriangle8_t &);

//Bass in triangle up and down
#define BUD_START_AT_HYPOTENUSE 0x01
#define BUD_STOP_AT_CATHETUS    0x02
#define BUD_CHESS               0x04

bool mode_BassUpDown(uint16_t &value,              //Context value
                     uint16_t timeDelta,           //Time delta
                     uint8_t flags,                //Flags        
                     XYDraw &draw,                 //Drawing object
                     const RightTriangle8_t &tr    //Triangle
             ){  

  struct S{
  uint8_t dir:1;    //0 - down, 1 - up
  uint8_t y:7;      //Y coordinate
  };
  
  S *s = (S *)&value;  

  int8_t x =  tr.hypotenuseX(s->y);

  //Top line
  draw.line(flags & BUD_START_AT_HYPOTENUSE ? x : tr.cornerX(), tr.y, tr.x, tr.y, CRGB::Red);

    
  //Check for the next beat
  if( /*timeDelta >= random8(0, 201) && */ timeDelta <= 500 ){  

    //Draw next line
    if(!s->dir || !(flags & BUD_STOP_AT_CATHETUS)){            
      draw.line(x, s->y, tr.x, s->y, CRGB::Red); 
    }

    //Move up or down
    s->y += tr.topSided() ? 1 - s->dir * 2 : -1 + s->dir * 2;         

    //Change direction if it is at the end of the line          
    s->dir  = s->y == tr.cornerY() || s->y == 0 ? !s->dir : s->dir; 

    
    //Return true if it came to the top              
    return s->y == 0 && !s->dir;
  }

  return true;  
}



//Bass drawing mode 1
bool mode1_RGBBass(uint16_t &value,              //Context value
                   uint16_t timeDelta,           //Time delta
                   XYDraw &draw,                 //Drawing object
                   const RightTriangle8_t &tr    //Triangle
                  ){  
                     
  return  mode_BassUpDown(value, timeDelta, 0, draw, tr);  
}

//Bass drawing mode 2
bool mode2_RGBBass(uint16_t &value,                             //Context value
                   uint16_t timeDelta,                          //Time delta
                   XYDraw &draw,                                //Drawing object
                   const RightTriangle8_t &tr                   //Triangle
                  ){

  return mode_BassUpDown(value, timeDelta, BUD_START_AT_HYPOTENUSE | BUD_STOP_AT_CATHETUS, draw, tr);
 }

 

 //Bass drawing mode 1
bool mode1_RGBMid(uint16_t &value,              //Context value
                  uint16_t timeDelta,           //Time delta
                  XYDraw &draw,                 //Drawing object
                  const RightTriangle8_t &tr    //Triangle
 ){  

  struct S{
    uint8_t dir:1; //0 - down, 1 - up
    uint8_t y:7;   //Y coordinate
  };
  
  S *s = (S *)&value;

  //Draw vertical drop  
  draw.pixel(tr.x, tr.cornerY() + (tr.bottomSided() ? s->y : -s->y), CRGB::LawnGreen);
  s->y = (s->y + 1) % abs(tr.height());
    
  //Random dot
  int8_t x, y;
  tr.randomPointY(x, y, 1);
  draw.pixel(x, y, CRGB::Green);

  if(timeDelta <= 150){
    tr.randomPointY(x, y);
    draw.pixel(x, y, CRGB::ForestGreen);
  }


  
  return timeDelta > 300 ? true : false;  
 }


//Bass drawing mode 1
bool mode1_RGBTreble(uint16_t &value,              //Context value
                     uint16_t timeDelta,           //Time delta
                     XYDraw &draw,                 //Drawing object
                     const RightTriangle8_t &tr    //Triangle
                    ){  

  //Random dot
  
  int8_t x, y;
  tr.randomPointY(x, y);
  draw.pixel(x, y, CRGB::Blue);

  if(timeDelta <= 100){
    tr.randomPointY(x, y);
    draw.pixel(x, y, CRGB::DeepSkyBlue);
  }
  
  return false;  
}



class EffectSoundRGB: public Effect{
  public:

  virtual void reset(){    
    setSpeedDelay(10);

    _bassFunc = 0;
    _ctxSound.bassValue = 0;
    BASS_BEAT_RESET();

    _ctxSound.midValue = 0; 
    MID_BEAT_RESET();

    _ctxSound.trebleValue = 0;  
    TREBLE_BEAT_RESET();
  }

protected:

  //Bass drawing
  void proceedBass(XYDraw &draw, uint8_t value){  

    //Rectangle for drawing
    RightTriangle8_t tr(draw.width() / 2 - 1, 0, -draw.width() / 2, draw.height() / 2);

    //Fade first
    draw.fadeToBlackRightTriangle(tr.x, tr.y, tr.width(), tr.height(), getSpeedDelay() * 6);
    
    if(BASS_BEAT_CHECK() >= 150 && _sc->isBassPeak(value)){      

      //DBG_OUTLN("%d %d %d %d", value, _sc->getStats(ssgAverageBass).getAverage(), _sc->getStats(ssgAverageBass).getStdDev(), _sc->getMin());

      FuncRGBMode_t bf[] = {mode1_RGBBass, mode2_RGBBass, mode2_RGBBass};      
      
      if(bf[_bassFunc](_ctxSound.bassValue, BASS_BEAT_CHECK(), draw, tr)){
        //Reset
        _ctxSound.bassValue = 0;
        
        //Change function
        _bassFunc = random8(sizeof(bf) / sizeof(bf[0]));      
      }
      
      //Set next check time
      BASS_BEAT_RESET();
    }

    //Mirror top left to top right
    draw.mirrorRightTriangleHorizontally(tr.x, tr.y, tr.width(), tr.height(), draw.width() / 2 - 1, 1);
    //Mirror top left to bottom left
    draw.mirrorRightTriangleVertically(tr.x, tr.y, tr.width(), tr.height(), draw.height() / 2 - 1, 0, 1);
    //Mirror top right to bottom right
    draw.mirrorRightTriangleVertically(tr.x + 1, tr.y, draw.width() / 2, draw.height() / 2, draw.height() / 2 - 1, 0, 1);
  }

  void proceedMid(XYDraw &draw, uint8_t value){  
    
    //Draw in left-top horizontal right angle triangle 
    RightTriangle8_t tr(0, draw.height() / 2 - 1, draw.width() / 2 - 1, -(draw.height() / 2 - 1));    

    draw.fadeToBlackRightTriangle(tr.x, tr.y, tr.width(), tr.height(), getSpeedDelay() * 4);

    //Check for time and peak
    if(MID_BEAT_CHECK() >= 100 && _sc->isMidPeak(value)){ 

      //DBG_OUTLN("%d %d %d %d", value, _sc->getStats(ssgAverageMid).getAverage(), _sc->getStats(ssgAverageMid).getStdDev(), _sc->getMin());     

      if(mode1_RGBMid(_ctxSound.midValue, MID_BEAT_CHECK(), draw, tr))
      {
        //Reset
        _ctxSound.midValue = 0;
      }

      //Set next check time
      MID_BEAT_RESET();
    }

    draw.mirrorRightTriangleHorizontally(tr.x, tr.y, tr.width(), tr.height(), draw.width() / 2 - 1, 1);
    
  }

  void processTreble(XYDraw &draw, uint8_t value){  
    //Draw in left-botton horizontal right angle triangle 
    RightTriangle8_t tr(0, draw.height() / 2, draw.width() / 2 - 1, draw.height() / 2 - 1);    

    //Fade first
    draw.fadeToBlackRightTriangle(tr.x, tr.y, tr.width(), tr.height(), getSpeedDelay() * 2);
    
    //Check time and peak
    if(TREBLE_BEAT_CHECK() >= 50 && _sc->isTreblePeak(value)){      
                
      //DBG_OUTLN("%d %d %d ", value, _sc->getStats(ssgAverageTreble).getAverage(), _sc->getStats(ssgAverageTreble).getStdDev());     

      if(mode1_RGBTreble(_ctxSound.trebleValue, TREBLE_BEAT_CHECK(), draw, tr))
      {
        //Reset
        _ctxSound.trebleValue = 0;
        
      }
    
      //Set next check time
      TREBLE_BEAT_RESET();
    }

    draw.mirrorRightTriangleHorizontally(tr.x, tr.y, tr.width(), tr.height(), draw.width() / 2 - 1, 1);
  }
  
  void proceed(CRGB *leds, uint16_t numLeds){
    //Get sound data
    uint16_t bands[SC_MAX_BANDS];
    getSoundBands(bands, false);
    
    
    //Draw
    XYDraw draw(leds, numLeds); 

    
    //Draw bass
    proceedBass(draw, 
                ((uint16_t)bands[0] + bands[1]) / 2 //max(bands[0], bands[1]) 
              );


    //Draw mid
    proceedMid(draw, 
              ((uint16_t)bands[2] + bands[3]) / 2 //max(bands[2], bands[3])
            );


    //Draw treble
    processTreble(draw, 
                ((uint16_t)bands[4] + bands[5] + bands[6]) / 3 //max(bands[4], max(bands[5], bands[6]))  
                );              

  }
  
  protected:
    //Bass drawing
    uint8_t _bassFunc;
    
};





#endif//__SOUND_H
