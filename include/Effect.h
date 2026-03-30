#ifndef __EFFECT_H
#define __EFFECT_H

#include "Palette.h"
#include "EffectEngineCtx.h"
#include "EffectEngineCfg.h"

#ifdef USE_MATRIX
  #include "Matrix.h"
#endif 

#ifdef USE_SOUND
  #include <SoundCapture.h>
#endif


///////////////////
// Basic effect
class Effect{
  public:

    //Draw 
    void draw(CRGB *leds, uint16_t numLeds);

    //Init
    virtual void reset() = 0;
    
    //Command processing
    virtual bool onCmd(const struct CtrlQueueItem &itm);
   
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
    void getSoundBands(sc_band_t &bands, bool scale);
    void getSound();
    bool onCmdSound(const struct CtrlQueueItem &itm);

    uint16_t beatCheckBass(uint16_t delta, uint8_t sensForBanAg = BASS_PEAK_SENS_BUS, uint8_t sensForAvg = BASS_PEAK_SENS_AVG) const;
    void beatResetBass() { _ctxSound.bassTicks = 0; }

    uint16_t beatCheckMid(uint16_t delta, uint8_t sensForBanAg = MID_PEAK_SENS_BUS, uint8_t sensForAvg = MID_PEAK_SENS_AVG) const;
    void beatResetMid() { _ctxSound.midTicks = 0; }

    uint16_t beatCheckTreble(uint16_t delta, uint8_t sensForBanAg = TREBLE_PEAK_SENS_BUS, uint8_t sensForAvg = TREBLE_PEAK_SENS_AVG) const;
    void beatResetTreble() { _ctxSound.trebleTicks = 0; }
#endif    

  protected:  

    ///////////////////
    //Structure to support real-time processing elements to save some memory
    struct EFFECT_CONTEXT {       
      int            step;        //current step

      CRGBPalette16  palCurrent;  //palette 1
      CRGBPalette16  palTarget;   //palette 2      
      
      //Reusable data items                      
      CRGB  rgb;  
      union {                      
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
      uint8_t   flags;       //Flags how to scale sound capture
      uint16_t  lower;       //Lower boundary from 0 to upper
      uint16_t  upper;       //Upper boundary from lower to 255

      uint8_t   bassTicks;   //Bass beat time
      uint16_t  bassValue;   //Bass drawing context

      uint8_t  midTicks;    //Mid beat offset
      uint16_t midValue;    //Mid drawing context 

      uint8_t  trebleTicks; //Treble beat offset
      uint16_t trebleValue; //Treble drawing context
    };

    static EFFECT_SOUND_CONTEXT _ctxSound;       //Sound context    
    static SoundCapture        *_sc;             //Sound capture  

#endif    
};

#ifdef USE_SOUND

#define SOUND_FADE_10(fade) (fade * getSpeedDelay() / 10)
#define BASS_PEAK_CHECK_TIME   150
#define MID_PEAK_CHECK_TIME    100
#define TREBLE_PEAK_CHECK_TIME 50

#endif

/////////////////////////////////////////
// Single Color Effect

class EffectColor: public Effect{
  public:
  //Command processing
    virtual bool onCmd(const struct CtrlQueueItem &itm);
};
  
/////////////////////////////////////////
// Effect Palette Transform - basic palette transformation
#define MAX_PAL_CHANGES 24
#define CHANGE_PAL_STEP 500


class EffectPaletteTransform: public Effect{ 
public:
  virtual bool onCmd(const struct CtrlQueueItem &itm);

  protected:  
    virtual void proceed(CRGB *leds, uint16_t numLeds);
    virtual void reset();    
     
    virtual int getMaxStep() const;

    CRGB getCurrentPalColor(uint8_t index, uint8_t brightness = 255, TBlendType blendType = LINEARBLEND) const;    
    void updatePal();  
};







#endif //__EFFECT_H
