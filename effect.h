#ifndef __EFFECT_H
#define __EFFECT_H

///////////////////
// Basic effect

class CRGB;

//Max number of preallocated palettes
#define MAX_PAL 4

class Effect{
  public:
    Effect();
    virtual ~Effect();

    //Init
    void init(CRGB *leds, int numLeds);
    
    //Process
    void loop(CRGB *leds, int numLeds); 

    //Color
    CRGB getColor() const;
    CHSV getHSV() const;
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

    //Pallette pool for memory optimization
    static CRGBPalette16 &allocPalette(int index);

    //Private color management
    void setRandomColor();
   
 private:
   //Color for some effects or static
   CHSV _hsv;
   
   //Speed
   uint8_t  _speedDelay; //byte, i.e. range is 0 - 255, that maps to range from SPEED_DELAY_MIN to SPEED_DELAY_MAX by setSpeedDelay and getSpeedDelay

   //Preallocated palettes
   static CRGBPalette16 _pals[MAX_PAL];

};



#endif //__EFFECT_H
