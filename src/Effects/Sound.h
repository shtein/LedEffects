#ifndef __SOUND_H
#define __SOUND_H

#include <utils.h>
#include <SoundCapture.h>


////////////////////////////////////////////
// EffectSoundVUM - sound visualization with VU meter kind of display


//Default palette
DEFINE_GRADIENT_PALETTE( sound_bands_gp ) {
      0,   0xBF,   0,    0,   // red
      //16,   0xFF,   0,    0,   // red
      128,    0,   0xBF,   0,   // green   
      //219,    0,   0,  0xFF,   // blue
      255,    0,   0,   0xBF,   // blue
}; 



class EffectSoundVUM: public Effect{
public:
  virtual void reset(){
    
    setSpeedDelay(10);

    _ctx.palCurrent = sound_bands_gp;
  }

protected:
    
  void proceed(CRGB *leds, uint16_t numLeds){
    //Get sound data

    uint8_t bands[SC_MAX_BANDS];
    getSoundBands(bands, SC_MAX_BANDS);
    SCALE_SOUND(bands, SC_MAX_BANDS);

#ifdef USE_MATRIX  
    XY xy;

    size_t bandLeds = xy.height();
    size_t bandRows = xy.width() / SC_MAX_BANDS;
    size_t firstRow = ( xy.width() % SC_MAX_BANDS ) / 2;
    

    //Draw each band according to matrix configuration
    for(size_t i = 0; i < SC_MAX_BANDS; i++){

      uint8_t colorIndexFirst = mapEx(i, 0, SC_MAX_BANDS, 0, 255 );
      uint8_t colorIndexLast  = mapEx(i + 1, 0, SC_MAX_BANDS, 0, 255 );    

      //Bars
      for(size_t j = 0; j < bandRows; j++){  
        
        size_t first = xy.xy(i  * bandRows + j + firstRow, 0);
        size_t last  = xy.xy(i  * bandRows + j + firstRow, xy.height() - 1);
        
        size_t index = first > last ? last : first;

        if (EFFECT_PARAM_SOUNDVUM(_cfg) == sv_Symmetric){
          drawBandSymmetric(leds + index, bandLeds, bands[i], colorIndexFirst,  colorIndexLast, i < 2 ? 1 : i < 4 ? 2 : 3 );
        }else{
          drawBand(leds + index, bandLeds, first > last ? 0 : bandLeds - 1, bands[i], colorIndexFirst,  colorIndexLast );
        }
        
      }           
    }  
    /*
    //Draw boundary
    //See if there is last column
    //Min and max
    drawBoundary(leds + bandLeds * (SC_MAX_BANDS * 2 + 1), 
                bandLeds, 
                0, 
                SOUND_MIN(), 
                SOUND_MAX(),
                CHSV(HUE_GREEN, 255, 127),
                CHSV(HUE_BLUE, 255, 127)             
              );

    //Lower and upper
    drawBoundary(leds + bandLeds * (SC_MAX_BANDS * 2 + 1), 
                  bandLeds, 
                  0, 
                  _ctxSound.lower, 
                  _ctxSound.upper,
                  (_ctxSound.flags & SC_MAP_USE_MIN) ? CRGB::Black : CRGB(CHSV(HUE_GREEN + 25, 255, 127)),
                  (_ctxSound.flags & SC_MAP_USE_MAX) ? CRGB::Black : CRGB(CHSV(HUE_BLUE + 25, 255, 127)),
                  false
                );
    */
       
#else
    //Number of sections
    int bandLeds = numLeds / SC_MAX_BANDS;
        
    
    for(int i = 0; i < SC_MAX_BANDS; i++){
      drawBand(leds + i * bandLeds, 
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
#endif //USE_MATRIX
  }

#define SS_FADE_FACTOR_MIN 20
#define SS_FADE_FACTOR_DELTA(fade) (getSpeedDelay() * fade)

//Primitives

void drawBand(CRGB *leds, uint16_t numLeds, uint16_t center, uint8_t volume, uint8_t colorIndexFirst, uint8_t colorIndexLast, uint8_t fade = 2) {
  // Draw center
  leds[center] = (volume > 0) ? ColorFromPalette(_ctx.palCurrent, colorIndexFirst, 255, NOBLEND) : leds[center].fadeToBlackBy(SS_FADE_FACTOR_MIN);

  // Calculate parameters for bottom part and top part
  uint8_t firstStep = 255 / max(center + 1, numLeds - center);

  // Draw bottom to center
  for (uint16_t i = 0; i < center; i++) {
      uint8_t step = firstStep + (255 - firstStep) * (center - i - 1) / (center + 1);
      leds[i] = (volume > step) ? ColorFromPalette(_ctx.palCurrent, colorIndexLast - i * (colorIndexLast - colorIndexFirst) / center, 255, NOBLEND) : leds[i].fadeToBlackBy(SS_FADE_FACTOR_MIN + (center - i) * SS_FADE_FACTOR_DELTA(fade) / center);
  }

  // Draw center to top
  for (uint16_t i = center + 1; i < numLeds; i++) {
      uint8_t step = firstStep + (255 - firstStep) * (i - center - 1) / (numLeds - center);
      leds[i] = (volume > step) ? ColorFromPalette(_ctx.palCurrent, colorIndexFirst + (i - center) * (colorIndexLast - colorIndexFirst) / (numLeds - center - 1), 255, NOBLEND) : leds[i].fadeToBlackBy(SS_FADE_FACTOR_MIN + (i - center) * SS_FADE_FACTOR_DELTA(fade) / (numLeds - center - 1));
  }
}

void drawBandSymmetric(CRGB *leds,
                       uint16_t numLeds,
                       uint8_t volume,
                       uint8_t colorIndexFirst,
                       uint8_t colorIndexLast,
                       uint8_t fade = 2){

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
  for(uint16_t i = 0;  i < numLeds / 2; i++)
    first[-i - offs] = first[i];
} 

  

  void drawBoundary(CRGB *leds, 
                    uint16_t numLeds, 
                    uint8_t bottom,
                    uint8_t lower, 
                    uint8_t upper,
                    const CRGB &clrLower,
                    const CRGB &clrUpper,
                    bool eraseBackground = true
                    ){

    if(eraseBackground){
      fill_solid(leds, numLeds, CRGB::Black);    
    }

    leds[mapEx(lower, 0, 255, bottom, numLeds - 1 - bottom)] += clrLower;
    leds[mapEx(upper, 0, 255, bottom, numLeds - 1 - bottom)] += clrUpper;
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
  uint8_t y:6;      //Y coordinate
  };
  S *s = (S *)&value;  

  int8_t x =  tr.hypotenuseX(s->y);

  //Top line
  draw.line(flags & BUD_START_AT_HYPOTENUSE ? x : tr.cornerX(), tr.y, tr.x, tr.y, CRGB::Red);

    
  //Check for the next beat
  if( /*timeDelta >= random8(0, 201) && */ timeDelta <= 700 ){  

    //Draw next line
    if(!s->dir || !(flags & BUD_STOP_AT_CATHETUS)){            
      draw.line(x, s->y, tr.x, s->y, CRGB::Red); 
    }

    //Move up or down
    s->y += tr.TopSided() ? 1 - s->dir * 2 : -1 + s->dir * 2;         

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
  draw.pixel(tr.x, tr.cornerY() + (tr.BottomSided() ? s->y : -s->y), CRGB::Green);
  s->y = (s->y + 1) % abs(tr.height());
    
  //Random dot
  int8_t x, y;
  tr.randomPointX(x, y, 1, 0);
  draw.pixel(x, y, CRGB::Green);

  
  return false;  
 }


//Bass drawing mode 1
bool mode1_RGBTreble(uint16_t &value,              //Context value
                     uint16_t timeDelta,           //Time delta
                     XYDraw &draw,                 //Drawing object
                     const RightTriangle8_t &tr    //Triangle
                    ){  

  //Random dot
  
  int8_t x, y;
  tr.randomPointX(x, y);
  draw.pixel(x, y, CRGB::Blue);
  
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
  void proceedBass(XYDraw &draw, uint8_t value, uint8_t threshold){  

    //Rectangle for drawing
    RightTriangle8_t tr(draw.width() / 2 - 1, 0, -draw.width() / 2, draw.height() / 2);

    //Fade first
    draw.fadeToBlackRightTriangle(tr.x, tr.y, tr.width(), tr.height(), getSpeedDelay() * 7 / 2);

    if(BASS_BEAT_CHECK() >= 150 && value >= threshold){

      FuncRGBMode_t bf[] = {mode1_RGBBass, mode2_RGBBass, mode2_RGBBass};      
      
      if(bf[_bassFunc](_ctxSound.bassValue, BASS_BEAT_CHECK(), draw, tr)){
        //Change mode
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

  void proceedMid(XYDraw &draw, uint8_t value, uint8_t threshold){  
    
    //Draw in left-top horizontal right angle triangle 
    RightTriangle8_t tr(0, draw.height() / 2 - 1, draw.width() / 2 - 1, -(draw.height() / 2 - 1));    

    draw.fadeToBlackRightTriangle(tr.x, tr.y, tr.width(), tr.height(), getSpeedDelay() * 2);

    //Check for time and peak
    if(MID_BEAT_CHECK() >= 100 && value >= threshold){

      mode1_RGBMid(_ctxSound.midValue, MID_BEAT_CHECK(), draw, tr);

      //Set next check time
      MID_BEAT_RESET();
    }

    draw.mirrorRightTriangleHorizontally(tr.x, tr.y, tr.width(), tr.height(), draw.width() / 2 - 1, 1);
    
  }

  void processTreble(XYDraw &draw, uint8_t value, uint8_t threshold){  
    //Draw in left-botton horizontal right angle triangle 
    RightTriangle8_t tr(0, draw.height() / 2, draw.width() / 2 - 1, draw.height() / 2 - 1);    

    //Fade first
    draw.fadeToBlackRightTriangle(tr.x, tr.y, tr.width(), tr.height(), getSpeedDelay() );
    
    //Check time and peak
    if(TREBLE_BEAT_CHECK() >= 50 && value >= threshold) 
    {
      mode1_RGBTreble(_ctxSound.trebleValue, TREBLE_BEAT_CHECK(), draw, tr);
    
      //Set next check time
      TREBLE_BEAT_RESET();
    }

    draw.mirrorRightTriangleHorizontally(tr.x, tr.y, tr.width(), tr.height(), draw.width() / 2 -1, 1);
  }
  
  void proceed(CRGB *leds, uint16_t numLeds){
    //Get sound data
    uint8_t bands[SC_MAX_BANDS];
    getSoundBands(bands, SC_MAX_BANDS);

    
    //Draw
    XYDraw draw(leds, numLeds); 

    //Draw bass
    proceedBass(draw, 
                ((uint16_t)bands[0] + bands[1]) / 2, 
                _statsSound.get(ssgAverageBass).getAverage () - _statsSound.get(ssgAverageBass).getStdDev() / 2
              );

              
    //Draw mid
    proceedMid(draw, 
              ((uint16_t)bands[2] + bands[3]) / 2,
              _statsSound.get(ssgAverageMid).getAverage() - _statsSound.get(ssgAverageMid).getStdDev() * 2 / 3
            );


    //Draw treble
    processTreble(draw, 
                  ((uint16_t)bands[4] + bands[5] + bands[6]) / 3,
                  _statsSound.get(ssgAverageTreble).getAverage() - _statsSound.get(ssgAverageTreble).getStdDev() * 3 / 4
                );              

  }
  
  protected:
    //Bass drawing
    uint8_t _bassFunc;
    
};





#endif//__SOUND_H
