#ifndef __EFFECT_H
#define __EFFECT_H

///////////////////
// Basic effect

class Effect{
  public:
    Effect();
    virtual ~Effect();

    //Two functions to proceed: init and proceed
    void init(CRGB *leds, int numLeds);
    void loop();

    //Color
    CRGB getColor() const;
    void setColor(const CRGB &color);

    //Speed delay
    void setSpeedDelay(int speedDelay);
    int  getSpeedDelay() const;

  protected:
    virtual void reset() = 0;
    virtual void proceed(int speedDelay) = 0;
  
    //Utility functions
    void setPixel(int led, byte red, byte green, byte blue);
    void setPixel(int led, const CRGB &color);
    void setAll(byte red, byte green, byte blue);
    void setAll(const CRGB &color);
    
    void setRandomColor();

    //Proceeding related
    void nextProceedIn(int delta);
    bool timeToProceed() const;
    
 protected:
  //Data
  int   _numLeds;
  CRGB *_leds;

  //Color for some effects or static
  CRGB  _color;

  //Next check point
  unsigned long _millis;
  int           _speedDelay;
};

#endif //__EFFECT_H
