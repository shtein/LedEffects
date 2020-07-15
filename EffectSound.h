#ifndef __EFFECT_SOUND_H
#define __EFFECT_SOUND_H



////////////////////////////////////////////
// EffectSound - sound effect 
// 
#define MAX_BANDS 7

class SoundCapture;

typedef struct {
  uint8_t bands[MAX_BANDS]; //8 bit for each band

  uint8_t lower;            //Lower boundary
  uint8_t upper;            //Upper boundary
  uint8_t max;              //Maximum
  uint8_t min;              //Minimum
} band8_visual; 

class EffectSound: public Effect{
  public:
    EffectSound();
    ~EffectSound();

    static void initSoundCapture(SoundCapture *sc);
    
    virtual void idle();
    virtual void onCmd(const struct CtrlQueueItem &itm);
    
  protected:
    virtual void reset();
    virtual void proceed(CRGB *leds, int numLeds);    

    virtual void updateLeds(CRGB *leds, 
                            int numLeds, 
                            const band8_visual &data) = 0;

    void getBands(band8_visual &output);

  private:
    static SoundCapture *_sc;       //Single instance of sound capture
    
    static uint8_t      _lower;    //Lower boundary from 0 to _upper
    static uint8_t      _upper;    //Upper boundary from _lower to 255

    static uint8_t      _max;         //Maximum
    static uint8_t      _min;         //Minimum
};



#endif //__EFFECT_SOUND_H


