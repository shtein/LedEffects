#ifndef __EFFECT_H
#define __EFFECT_H

#include "Palette.h"
#include "EffectEngineCtx.h"
#include "EffectEngineCfg.h"

///////////////////
// Helpers
#define qsuba(x, b)  ((x > b) ? x - b : 0) 


///////////////////
// Basic effect
class CRGB;
class CHSV;


class Effect{
  public:
    Effect();
    virtual ~Effect();

    //Init
    virtual void reset() = 0;
    
    //Process
    virtual void proceed(CRGB *leds, uint16_t numLeds) = 0;

    //Read/Write config
    //virtual bool config(EffectConfig &cfg, bool read);

    //Command processing
    virtual bool onCmd(struct CtrlQueueItemEx &itm);
   
    //Speed delay
    void setSpeedDelay(uint8_t speedDelay);
    uint8_t  getSpeedDelay() const;    
    
    //Color control    
    const CHSV & getHSV() const;
    void setHSV(const CHSV &hsv);
    void setRandomColor();
  
  protected:
    ///////////////////
    //Structure to support real-time processing elements to save some memory
    struct EffectContext { 
      uint8_t        speedDelay;  //speed
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
  
    static EffectContext _ctx;  
};

/////////////////////////////////////////
// Single Color Effect

class EffectColor: public Effect{
  public:
    EffectColor();
    ~EffectColor();

  //Command processing
    virtual bool onCmd(struct CtrlQueueItemEx &itm);

    //Read/Write condig
    //bool config(EffectConfig &cfg, bool read);
};

/////////////////////////////////////////
// Effect Palette Transform - basic palette transformation
#define MAX_PAL_CHANGES 24
#define CHANGE_PAL_STEP 500


//Palette change routine
typedef void (*FuncGetPalette_t) (CRGBPalette16 &);

//Default palette change routine
void FuncGetPal_Default(CRGBPalette16 &pal);

class EffectPaletteTransform: public Effect{  
  public:
    EffectPaletteTransform(FuncGetPalette_t getPal);

  protected:  
    virtual void proceed(CRGB *leds, uint16_t numLeds);
    virtual void reset();    
     
    virtual int getMaxStep() const;

    CRGB getCurrentPalColor(uint8_t index, uint8_t brightness = 255, TBlendType blendType = LINEARBLEND) const;    

  protected:
    FuncGetPalette_t _getPal;
};




//////////////////////////////////////////////////////
// Color schemes - set of transforming theme palettes 

#define BEGIN_TRANSFORM_SCHEMA_TYPE(FunctionName, Type) \
void FunctionName(CRGBPalette16 &pal){ \
  struct _ts{ \
    const Type &item; \
  } const ts[] = {


#define BEGIN_TRANSFORM_SCHEMA_GRADIENT_PALETTE(FunctionName)  BEGIN_TRANSFORM_SCHEMA_TYPE(FunctionName, TProgmemRGBGradientPalettePtr)
#define BEGIN_TRANSFORM_SCHEMA_RGB16_PALETTE(FunctionName)  BEGIN_TRANSFORM_SCHEMA_TYPE(FunctionName, TProgmemRGBPalette16)


#define END_TRANSFORM_SCHEMA() }; \
  pal = ts[random8(0, sizeof(ts) / sizeof(ts[0]))].item; \
} 

#define TRANSOFRM_PALETTE(pal) pal,

/*
Usage of transform shchema: 
BEGIN_TRANFORM_SCHEMA_XXX(YourFunctionName)
  TRANSOFRM_PALETTE(pallete1)
  TRANSOFRM_PALETTE(palette2)
  ...
  TRANSOFRM_PALETTE(paletteN)
END_TRANSFORM_SCHEMA()

It creates 
void YourFunctionName(CRGBPalette16 &pal);
*/


#endif //__EFFECT_H
