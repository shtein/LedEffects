#ifndef __EFFECT_H
#define __EFFECT_H

#include "Palette.h"
#include "EffectEngineCtx.h"
#include "EffectEngineCfg.h"

#ifdef USE_MATRIX
  #include "Matrix.h"
#endif 

#ifdef USE_SOUND
  #include "SoundUtils.h"

  class SoundCapture;
#endif

///////////////////
// Helpers
#define qsuba(x, b)  ((x > b) ? x - b : 0) 


///////////////////
// Basic effect
class Effect{
  public:

    //Draw 
    void draw(CRGB *leds, uint16_t numLeds);

    //Init
    virtual void reset() = 0;
    
    //Command processing
    virtual bool onCmd(const struct CtrlQueueItem &itm, NtfSet &ntf);
   
    //Speed delay
    void setSpeedDelay(uint8_t speedDelay);
    uint8_t getSpeedDelay() const;    

    void setConfig(const EFFECT_DATA &cfg);
    void getConfig(EFFECT_DATA &cfg);

#ifdef USE_SOUND
    //Sound
    static void initSoundCapture(SoundCapture *sc);
#endif    

  protected:
    //Process
    virtual void proceed(CRGB *leds, uint16_t numLeds) = 0;
    
#ifdef USE_SOUND
    //Sound
    void getSoundBands(uint8_t *bands, size_t numBands);
    bool onCmdSound(const struct CtrlQueueItem &itm, NtfSet &ntf);
#endif    

  protected:  

    ///////////////////
    //Structure to support real-time processing elements to save some memory
    struct EFFECT_CONTEXT {       
      int            step;        //current step

      CRGBPalette16  palCurrent;  //palette 1
      CRGBPalette16  palTarget;   //palette 2      
      
      union{                      //Reusable data items        
        CRGB     rgb;        
        uint8_t  byte;
        int8_t   ch;
        uint16_t word;
        int16_t  value;       
      };
    };
    
    static uint8_t        _speedDelay; //Speed
    static EFFECT_DATA    _cfg;        //Config
    static EFFECT_CONTEXT _ctx;        //Runtime context

#ifdef USE_SOUND
    struct EFFECT_SOUND_CONTEXT{
      uint8_t  flags;       //Flags how to scale sound capture
      uint8_t  lower;       //Lower boundary from 0 to upper
      uint8_t  upper;       //Upper boundary from lower to 255

      uint8_t   bassTicks;   //Bass beat time
      uint16_t  bassValue;   //Bass drawing context

      uint8_t  midTicks;    //Mid beat offset
      uint16_t midValue;    //Mid drawing context 

      uint8_t  trebleTicks; //Treble beat offset
      uint16_t trebleValue; //Treble drawing context
    };

    static EFFECT_SOUND_CONTEXT _ctxSound;       //Sound context    
    static SoundStats           _statsSound;     //Max, min, average
    static SoundCapture        *_sc;             //Sound capture  

#endif    
};

#ifdef USE_SOUND

#define SOUND_MIN() (_statsSound.get(SoundStatGet::ssgMin).getAverage() - _statsSound.get(SoundStatGet::ssgMin).getStdDev())
#define SOUND_MAX() (_statsSound.get(SoundStatGet::ssgMax).getAverage() + _statsSound.get(SoundStatGet::ssgMax).getStdDev())
#define SOUND_AVERAGE() _statsSound.get(SoundStatGet::ssgAverage).getAverage()
#define SOUND_STDDEV()  _statsSound.get(SoundStatGet::ssgAverage).getStdDev()

#define SCALE_SOUND(bands, numBands) \
  scaleSound(bands, numBands, _ctxSound.flags, _ctxSound.lower, _ctxSound.upper, SOUND_MIN(), SOUND_MAX(), SOUND_AVERAGE(), SOUND_STDDEV())


//Beat detection  
#define BASS_BEAT_CHECK() (_ctxSound.bassTicks * getSpeedDelay())
#define BASS_BEAT_RESET() _ctxSound.bassTicks = 0;

#define MID_BEAT_CHECK() (_ctxSound.midTicks * getSpeedDelay())
#define MID_BEAT_RESET() _ctxSound.midTicks = 0;

#define TREBLE_BEAT_CHECK() (_ctxSound.trebleTicks * getSpeedDelay()) 
#define TREBLE_BEAT_RESET() _ctxSound.trebleTicks = 0;


#endif

/////////////////////////////////////////
// Single Color Effect

class EffectColor: public Effect{
  public:
  //Command processing
    virtual bool onCmd(const struct CtrlQueueItem &itm, NtfSet &ntf);
};
  
/////////////////////////////////////////
// Effect Palette Transform - basic palette transformation
#define MAX_PAL_CHANGES 24
#define CHANGE_PAL_STEP 500


class EffectPaletteTransform: public Effect{ 
public:
  virtual bool onCmd(const struct CtrlQueueItem &itm, NtfSet &ntf);

  protected:  
    virtual void proceed(CRGB *leds, uint16_t numLeds);
    virtual void reset();    
     
    virtual int getMaxStep() const;

    CRGB getCurrentPalColor(uint8_t index, uint8_t brightness = 255, TBlendType blendType = LINEARBLEND) const;    
    void updatePal();  
};







#endif //__EFFECT_H
