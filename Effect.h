#ifndef __EFFECT_H
#define __EFFECT_H

#include "Palette.h"

///////////////////
// Helpers
#define qsuba(x, b)  ((x > b) ? x - b : 0) 

///////////////////
// Basic effect

class CRGB;
class CHSV;
class CtrlQueueItem;

class Effect{
  public:
    Effect();
    virtual ~Effect();

    //Init
    void init(CRGB *leds, int numLeds);
    
    //Process
    void loop(CRGB *leds, int numLeds); 

    //Command processing
    virtual void onCmd(const struct CtrlQueueItem &itm);
    virtual void idle();
   
    //Speed delay
    void setSpeedDelay(uint16_t speedDelay);
    uint16_t  getSpeedDelay() const;

  protected:
    virtual void reset() = 0;
    virtual void proceed(CRGB *leds, int numLeds) = 0;
  
    //Work with leds
    static void setPixel(CRGB &led, byte red, byte green, byte blue);
    static void setPixel(CRGB &led, const CRGB &color);
    static void setAll(CRGB *leds, int numLeds, byte red, byte green, byte blue);
    static void setAll(CRGB *leds, int numLeds, const CRGB &color);     
   
 private:   
   //Speed
   uint8_t  _speedDelay; //byte, i.e. range is 0 - 255, that maps to range from SPEED_DELAY_MIN to SPEED_DELAY_MAX by setSpeedDelay and getSpeedDelay
};


/////////////////////////////////////////
// Single Color Effect

class EffectColor: public Effect{
  public:
    EffectColor(const CHSV &hsv = CHSV(HUE_RED, 0xFF, 0xFF));
    ~EffectColor();

  //Command processing
    virtual void onCmd(const struct CtrlQueueItem &itm);
        
  protected:  
  //Color control
    
    const CHSV & getHSV() const;
    void setHSV(const CHSV &hsv);
    CRGB getColor() const;
    void setRandomColor();

  private:
   CHSV _hsv;
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

   virtual void proceed(CRGB *leds, int numLeds); 
   virtual void reset();

  protected:
    void onStep();
  
    virtual void updateColors();
    virtual void updateLeds(CRGB *leds, int numLeds);
    
      
    virtual bool isReadyToBlendPal() const;
    virtual bool isReadyToChangePal() const;

    virtual int getMaxStep() const;
    
  protected:
    static CRGBPalette16 _palCurrent;
    static CRGBPalette16 _palTarget;

    FuncGetPalette_t     _getPal;

  private:
    static int           _step;
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
