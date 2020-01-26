#ifndef __EFFECT_H
#define __EFFECT_H

#include "EffectEngineCtx.h"
#include "Palette.h"

///////////////////
// Basic effect


class CRGB;
class CHSV;

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

class EffectPaletteTransform: public Effect{
  public: 
    EffectPaletteTransform();
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

  private:
    static int           _step;
};


//////////////////////////////////////////////////////
// Color schemes - set of transforming theme palettes 
#define BEGIN_TRANFORM_SCHEMA(className) \
template<class T> \
class className: public T{ \
  protected: \
    virtual void updateColors() { \
        const TProgmemRGBGradientPalettePtr ts[] = {

#define END_TRANSFORM_SCHEMA() }; \
         T::_palTarget = ts[random(0, sizeof(ts) / sizeof(ts[0]) + 1)]; \
    } \
};

#define TRANSOFRM_PALETTE(pal) pal, \

/*
Usage of transform shchema: 

BEGIN_TRANFORM_SCHEMA(YourTemplateName)
  TRANSOFRM_PALETTE(pallete1)
  TRANSOFRM_PALETTE(palette2)
  ...
  TRANSOFRM_PALETTE(paletteN)
END_TRANSFORM_SCHEMA()

Instancinate
YourTemplateName<Class_Derived_from_EffectPaletteTransform>

*/

#endif //__EFFECT_H
