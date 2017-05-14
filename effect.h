#ifndef __EFFECT_H
#define __EFFECT_H

///////////////////
// Basic effect

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
    void setHSV(const CHSV &color);

    //Speed delay
    void setSpeedDelay(int speedDelay);
    int  getSpeedDelay() const;

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
  CHSV  _hsv;
  
  //Speed
  int                 _speedDelay;
};


inline void Effect::setPixel(CRGB &led,  byte red, byte green, byte blue) {
   led.r = red;
   led.g = green;
   led.b = blue;
}

inline void Effect::setPixel(CRGB &led, const CRGB &color) {
   led = color;
}

inline void Effect::setAll(CRGB *leds, int numLeds, const CRGB &color) {
  fill_solid(leds, numLeds, color);
}

inline void Effect::setAll(CRGB *leds, int numLeds, byte red, byte green, byte blue) {  
  fill_solid(leds, numLeds, CRGB (red, green, blue));
}


#endif //__EFFECT_H
