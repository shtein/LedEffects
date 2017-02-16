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
    void loop(int speedDelay);
    

  protected:
    virtual void reset() = 0;
    virtual void proceed(int speedDelay) = 0;
  
    //Utility functions
    void setPixel(int led, byte red, byte green, byte blue);
    void setPixel(int led, const CRGB &color);
    void setAll(byte red, byte green, byte blue);
    void setAll(const CRGB &color);
    CRGB selectRandomColor() const;

    //Proceeding related
    void nextProceedIn(int delta);
    bool timeToProceed() const;
    
 protected:
  //Data
  int   _numLeds;
  CRGB *_leds;

  //Next check point
  unsigned long _millis;
};

#endif //__EFFECT_H
