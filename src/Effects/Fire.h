#ifndef __FIRE_H
#define __FIRE_H

/////////////////////////////////////////
// Effect Fire
class EffectFire: public Effect{
protected:
  void proceed(CRGB *leds, uint16_t numLeds); 
  void reset();
};

inline void EffectFire::reset(){  
  setSpeedDelay(20);
  _ctx.word = 0;
  _ctx.palCurrent = HeatColors_p;

#ifdef USE_SOUND
  
  _ctxSound.bassValue = 0;
  BASS_BEAT_RESET();

  _ctxSound.midValue = 0; 
  MID_BEAT_RESET();

  _ctxSound.trebleValue = 0;  
  TREBLE_BEAT_RESET();

#endif //USE_SOUND

}


#ifdef USE_MATRIX
  #define MATRIX_FIRE_X_SCALE      64
  #define MATRIX_FIRE_Y_SCALE      64
  #define MATRIX_FIRE_SPEED        40
  #define MATRIX_FIRE_OVERLAY      150
  #define MATRIX_FIRE_B_ADJ        10    //Brightness adjustment
#else 
  #define FIRE_X_SCALE 20 
  #define FIRE_Y_SCALE 3
#endif //USE_MATRIX

inline void EffectFire::proceed(CRGB *leds, uint16_t numLeds){  

#ifdef USE_MATRIX

  _ctx.word += MATRIX_FIRE_SPEED;

  XYDraw xy(leds, numLeds);

  uint8_t xScale = MATRIX_FIRE_X_SCALE;
  uint8_t yScale = MATRIX_FIRE_Y_SCALE;


#ifdef USE_SOUND
    //Get sound data
    uint16_t bands[SC_MAX_BANDS];
    getSoundBands(bands, false);
    

    bool bassBeat   = BASS_BEAT_CHECK() >= 150 && _sc->isBassPeak();
    bool midBeat    = MID_BEAT_CHECK() >= 100 && _sc->isMidPeak();
    bool trebleBeat = TREBLE_BEAT_CHECK() >= 50 && _sc->isTreblePeak();

    if(bassBeat || midBeat || trebleBeat){ 
      //Increase speed on mid beat
      _ctxSound.midValue =  qadd8(_ctxSound.midValue, 160);    
    }

    if(bassBeat){ 
      BASS_BEAT_RESET();
    }

    if(midBeat){ 
      MID_BEAT_RESET();
    } 

    if(trebleBeat){
      TREBLE_BEAT_RESET();
    }     

   _ctxSound.midValue = qsub8(_ctxSound.midValue, 8);

   xScale -= _ctxSound.midValue >> 4;
   yScale -= _ctxSound.midValue >> 4;

#endif //USE_SOUND


  for(int16_t x = 0; x < xy.width(); x++){
    for(int16_t y = 0; y < xy.height(); y++){

      int16_t raw = inoise8(x * xScale, (y * yScale) - _ctx.word) - (y * (255 / xy.height()));
      uint8_t colorIndex = (raw <= 0) ? 0 : (raw > 255 ? 255 : raw);
      uint8_t brightness = (raw <= 0) ? 0 : (uint8_t)(255 - (raw / MATRIX_FIRE_B_ADJ));

      nblend(xy(x, (xy.height() - y - 1)), ColorFromPalette(_ctx.palCurrent, colorIndex, brightness), MATRIX_FIRE_OVERLAY);        

    }
  }


#else  
  
  for(uint16_t i = 0; i < numLeds ; i++) {        
    leds[i] = ColorFromPalette(_ctx.palCurrent, 
                              min( (i % 50) * (inoise8((i % 50) * FIRE_X_SCALE, millis() * FIRE_Y_SCALE * numLeds / 255))  >> 6, 255),
                              255,
                              LINEARBLEND ); 
  }

#endif //USE_MATRIX

}


#endif //__FIRE_H

