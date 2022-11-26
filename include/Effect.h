#ifndef __EFFECT_H
#define __EFFECT_H

#include "Palette.h"
#include "EffectEngineCtx.h"

///////////////////
// Helpers
#define qsuba(x, b)  ((x > b) ? x - b : 0) 


///////////////////
// Basic effect

class CRGB;
class CHSV;
class EEPROMCfg;

class Effect{
  public:
    Effect();
    virtual ~Effect();

    //Init
    void init(CRGB *leds, uint16_t numLeds);
    
    //Process
    void loop(CRGB *leds, uint16_t numLeds); 

    //Read/Write condig
    virtual bool config(EEPROMCfg &cfg, bool read);

    //Command processing
    virtual bool onCmd(struct CtrlQueueItemEx &itm);
   
    //Speed delay
    void setSpeedDelay(uint8_t speedDelay);
    uint8_t  getSpeedDelay() const;    

  protected:
    virtual void reset() = 0;
    virtual void proceed(CRGB *leds, uint16_t numLeds) = 0;

  
    //Work with leds
    static void setPixel(CRGB &led, byte red, byte green, byte blue);
    static void setPixel(CRGB &led, const CRGB &color);
    static void setAll(CRGB *leds, uint16_t numLeds, byte red, byte green, byte blue);
    static void setAll(CRGB *leds, uint16_t numLeds, const CRGB &color);  

 protected:
    ///////////////////
    //Structure to support real-time processing elements to save some memory
    struct EffectContext { 
      int            step;        //current step
      CRGBPalette16  palCurrent;  //palette 1
      CRGBPalette16  palTarget;   //palette 2
      uint8_t        speedDelay;  //speed
      union{
        CHSV     hsv;
        CRGB     rgb;
        uint8_t  byte;
        uint16_t word;
        int8_t   ch;
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
    bool config(EEPROMCfg &cfg, bool read);

  protected:  
  //Color control
    
    const CHSV & getHSV() const;
    void setHSV(const CHSV &hsv);
    CRGB getColor() const;
    void setRandomColor();
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
   ~EffectPaletteTransform();

   virtual void proceed(CRGB *leds, uint16_t numLeds);
   virtual void reset();

  protected:
    void onStep();
    CRGB getCurrentPalColor(uint8_t index, uint8_t brightness = 255, TBlendType blendType = LINEARBLEND) const;
  
    virtual void updateColors();
    virtual void updateLeds(CRGB *leds, uint16_t numLeds);
    
    
    virtual bool isReadyToBlendPal() const;
    virtual bool isReadyToChangePal() const;

    virtual int getMaxStep() const;
    
  protected:    
    FuncGetPalette_t     _getPal;
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
  pal = ts[random(0, sizeof(ts) / sizeof(ts[0]) + 1)].item; \
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
