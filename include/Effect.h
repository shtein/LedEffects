#ifndef __EFFECT_H
#define __EFFECT_H

#include "Palette.h"
#include "EffectEngineCtx.h"
#include "EffectEngineCfg.h"
#ifdef USE_MATRIX
  #include "Matrix.h"
#endif 



///////////////////
// Helpers
#define qsuba(x, b)  ((x > b) ? x - b : 0) 

void setRandomColor(CHSV &hsv);

#ifdef NTF_ENABLED  
  void putNtfObject(NtfBase &resp, const CHSV &data);
#endif


///////////////////////
// Effect Config Flags
#define ECF_NONE          0x00
#define ECF_HSV           0x01
#define ECF_RGB           0x02
#define ECF_TRANSFORM     0x04
#define ECF_KALEYDOSCOPE  (uint8_t)0x08



///////////////////
// Basic effect
class Effect{
  public:

    //Draw 
    void draw(CRGB *leds, uint16_t numLeds);

    //Init
    virtual void reset() = 0;
    
    //Process
    virtual void proceed(CRGB *leds, uint16_t numLeds) = 0;

    //Command processing
    virtual bool onCmd(const struct CtrlQueueItem &itm, NtfSet &ntf);
   
    //Speed delay
    void setSpeedDelay(uint8_t speedDelay);
    uint8_t getSpeedDelay() const;    

    void setConfig(const EFFECT_DATA &cfg);
    void getConfig(EFFECT_DATA &cfg);

  protected:
  
    ///////////////////
    //Structure to support real-time processing elements to save some memory
    struct EFFECT_CONTEXT {       
      int            step;        //current step

      CRGBPalette16  palCurrent;  //palette 1
      CRGBPalette16  palTarget;   //palette 2      

      union{                      //Reusable data items
        CHSV     hsv;
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
};

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
