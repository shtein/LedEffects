#ifndef __EFFECT_H
#define __EFFECT_H

#include "EffectEngineCtx.h"

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

    //Color
    CRGB getColor() const;
    const CHSV & getHSV() const;
    void setHSV(const CHSV &hsv);

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

    //Private color management
    void setRandomColor();
   
 private:
   //Color for some effects or static
   CHSV _hsv;
   
   //Speed
   uint8_t  _speedDelay; //byte, i.e. range is 0 - 255, that maps to range from SPEED_DELAY_MIN to SPEED_DELAY_MAX by setSpeedDelay and getSpeedDelay
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
    virtual void updateColors();
    virtual void updateLeds(CRGB *leds, int numLeds);
    
    virtual bool isReadyToBlendPal() const;
    virtual bool isReadyToChangePal() const;
    virtual bool isReadyToUpdateLeds() const;

    virtual int getMaxStep() const;
    
  protected:
    static CRGBPalette16 _palCurrent;
    static CRGBPalette16 _palTarget;

  private:
    static int           _step;
};



#endif //__EFFECT_H
