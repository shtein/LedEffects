#ifndef __SOUND_H
#define __SOUND_H

#include <utils.h>
#include <SoundCapture.h>

#include "Matrix.h"


//////////////////////////////////////////
// Sound Matrix
class EffectSoundMatrix: public EffectSound{
public:
  EffectSoundMatrix();

protected:
  void updateLeds(CRGB *leds, int numLeds, const band8_visual &data);
  virtual void drawMatrixBar(CRGB *leds, 
                             int numLeds,
                             int center, 
                             uint8_t volume,                                         
                             uint8_t colorIndexFirst, 
                             uint8_t colorIndexLast
                           ) = 0;
};

inline EffectSoundMatrix::EffectSoundMatrix(){
}

inline void EffectSoundMatrix::updateLeds(CRGB *leds, int numLeds, const band8_visual &data){
  //Number of sections
  //Last two sections are for tuning
  XY xy;

  int bandLeds = xy.height();
  int bandRows = xy.width() / SC_MAX_BANDS;

  //Draw each band according to matrix configuration
  for(int i = 0; i < SC_MAX_BANDS; i++){

    uint8_t colorIndexFirst = map(i, 0, SC_MAX_BANDS, 0, 255 );
    uint8_t colorIndexLast  = map(i + 1, 0, SC_MAX_BANDS, 0, 255 );    

    //Bars
    for(int j = 0; j < bandRows; j++){  
      
      int first = xy.xy(i * bandRows + j, 0);
      int last  = xy.xy(i * bandRows + j, xy.height() - 1);
      
      int index = first > last ? last : first;

      drawMatrixBar(leds + index,
                    bandLeds,
                    first > last ? 0 : bandLeds - 1,
                    data.scale(i),                                         
                    colorIndexFirst, 
                    colorIndexLast
                  );
    }     
    
  }  

  //Draw boundary
  //See if there is last column
  //Min and max
  drawBoundary(leds + bandLeds * (SC_MAX_BANDS * 2 + 1), 
              bandLeds, 
              0, 
              data.min, 
              data.max,
              CHSV(HUE_GREEN, 255, 127),
              CHSV(HUE_BLUE, 255, 127)             
            );

  //Lower and upper
  drawBoundary(leds + bandLeds * (SC_MAX_BANDS * 2 + 1), 
                bandLeds, 
                0, 
                data.lower, 
                data.upper,
                (data.flags & SC_MAP_USE_MIN) ? CRGB::Black : CRGB(CHSV(HUE_GREEN + 25, 255, 127)),
                (data.flags & SC_MAP_USE_MAX) ? CRGB::Black : CRGB(CHSV(HUE_BLUE + 25, 255, 127)),
                false
              );
}

///////////////////////////////////
// EffectSoundMatrixBar
class EffectSoundMatrixColumn: public EffectSoundMatrix{
  protected:
    void drawMatrixBar(CRGB *leds, 
                       int numLeds,  
                       int center,
                       uint8_t volume,                                         
                       uint8_t colorIndexFirst, 
                       uint8_t colorIndexLast
                      );
};

inline void EffectSoundMatrixColumn::drawMatrixBar( CRGB *leds, 
                                                    int numLeds,
                                                    int center, 
                                                    uint8_t volume,                                         
                                                    uint8_t colorIndexFirst, 
                                                    uint8_t colorIndexLast 
                                                  ){                                                        

  drawBand(leds, 
           numLeds, 
           center,
           volume, 
           colorIndexFirst,
           colorIndexLast            
          );

}

class EffectSoundMatrixSymmetric: public EffectSoundMatrix{
  protected:
    void drawMatrixBar(CRGB *leds, 
                       int numLeds,
                       int center, 
                       uint8_t volume,                                         
                       uint8_t colorIndexFirst, 
                       uint8_t colorIndexLast
                      );
};


inline void EffectSoundMatrixSymmetric::drawMatrixBar(CRGB *leds,
                                                      int numLeds,
                                                      int /*center*/,
                                                      uint8_t volume,                                         
                                                      uint8_t colorIndexFirst, 
                                                      uint8_t colorIndexLast 
                                                     ){

  drawBandSymmetric(leds,
                    numLeds,
                    volume, 
                    colorIndexFirst,
                    colorIndexLast
                   );  
}


/////////////////////////////////////////////////////
// EffectSoundRGB

class EffectSoundRGB: public EffectSound{
  public:
    void updateLeds(CRGB *leds, int numLeds, const band8_visual &data);
  
};


void EffectSoundRGB::updateLeds(CRGB *leds, int numLeds, const band8_visual &data){
  XYDraw draw(leds, numLeds);

  
  fadeToBlackBy(leds, numLeds, 64);    

  //int16_t r = mapEx( max(data.scale(0), data.scale(1) ), 0, 255, 0, draw.width()/2 );
  //int16_t r = mapEx( max(max(data.scale(2), data.scale(3)),  data.scale(4)), 0, 255, 0, draw.width()/2 );
  int16_t r = mapEx( (data.scale(2) + data.scale(3) + data.scale(4)) / 3, 0, 255, 0, draw.width()/2 );
  //int16_t r = mapEx( max(data.scale(5), data.scale(6) ), 0, 255, 0, draw.width()/2 );

  draw.fillCircle(draw.width()/2, draw.height()/2, r, CRGB::Blue);
  

  //uint16_t h = draw.height() / 3;
  //uint16_t w = draw.height() / 3;

  

}



#endif//__SOUND_H
