#ifndef __SOUND_H
#define __SOUND_H

#include <utils.h>
#include <SoundCapture.h>


////////////////////////////////////////////
// EffectSoundVIM - sound visualization with VU meter kind of display


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

    int bandLeds = xy.height();
    int bandRows = xy.width() / SC_MAX_BANDS;

    //Draw each band according to matrix configuration
    for(int i = 0; i < SC_MAX_BANDS; i++){

      uint8_t colorIndexFirst = mapEx(i, 0, SC_MAX_BANDS, 0, 255 );
      uint8_t colorIndexLast  = mapEx(i + 1, 0, SC_MAX_BANDS, 0, 255 );    

      //Bars
      for(int j = 0; j < bandRows; j++){  
        
        int first = xy.xy(i * bandRows + j, 0);
        int last  = xy.xy(i * bandRows + j, xy.height() - 1);
        
        int index = first > last ? last : first;

        if (EFFECT_PARAM_SOUNDVIM(_cfg) == sv_Symmetric){
          drawBandSymmetric(leds + index, bandLeds, bands[i], colorIndexFirst,  colorIndexLast, i < 2 ? 1 : i < 4 ? 2 : 3 );
        }else{
          drawBand(leds + index, bandLeds, first > last ? 0 : bandLeds - 1, bands[i], colorIndexFirst,  colorIndexLast );
        }
        
      }           
    }  

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

class EffectSoundRGB: public Effect{
  public:
  virtual void reset(){    
    setSpeedDelay(10);
    _lastBeatTime = 0;
    _lastMidTime  = 0;
    _bassOffset   = 0;
  }

protected:
  
  void proceed(CRGB *leds, uint16_t numLeds){
    //Get sound data
    uint8_t bands[SC_MAX_BANDS];
    getSoundBands(bands, SC_MAX_BANDS);

    fadeToBlackBy(leds, numLeds, getSpeedDelay());

    XYDraw draw(leds, numLeds);    
    
    //Bass
    uint8_t val = (uint16_t)(bands[0] + bands[1]) / 2;
  
    if(val > _statsSound.get(ssgAverageBass).getAverage () - _statsSound.get(ssgAverageBass).getStdDev() / 2 && DELTA_MILLS(_lastBeatTime) > 100){
      //leds[random16(0, numLeds)] = CRGB::Red;
      
      
      draw.rectangle(_bassOffset, _bassOffset, draw.width() - 1 - _bassOffset, draw.height() - 1 - _bassOffset, CRGB::Red);
      
      _bassOffset = DELTA_MILLS(_lastBeatTime) > 400 ? 0 : (_bassOffset + 1) % (min(draw.width(), draw.height() ) + 1 );
      SET_MILLIS(_lastBeatTime);      
    }

    //Mid
    val = (uint16_t)(bands[2] + bands[3] + bands[4]) / 3;

    if(val > _statsSound.get(ssAverageMid).getAverage() - _statsSound.get(ssAverageMid).getStdDev() / 2 && DELTA_MILLS(_lastMidTime) > 100){
      for (int i = 0; i < draw.width(); i++) {
        uint8_t hue = random8();
        //leds[draw.xy(i, draw.height() / 2)] = CHSV(hue, 255, 255);
      }      

      SET_MILLIS(_lastMidTime);      
    }
  }
  
  protected:
    uint16_t _lastBeatTime;
    uint16_t _lastMidTime;
    uint8_t  _bassOffset;
};





#endif//__SOUND_H
