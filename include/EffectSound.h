#ifndef __EFFECT_SOUND_H
#define __EFFECT_SOUND_H



/////////////////////////////////////////////
// CrlBuff - simple cirtular buffer
template<typename T, const int SIZE = 10>
class CrlBuff{
  public:
    CrlBuff();

    void reset();
    void add(const T &t);
    int size() const;
    int count() const;

    const T& operator[](int index) const;
    T& operator[](int index);

  private:
    T       _data[SIZE];  //Data
    uint8_t _index;       //Current index
    uint8_t _count;       //Number of elements
};

template <typename T, const int SIZE>
CrlBuff<T, SIZE>::CrlBuff(){
  _index = 0;
  _count = 0;
}

template <typename T, const int SIZE>
void CrlBuff<T, SIZE>::reset(){
  _index = 0;
  _count = 0;
}


template <typename T, const int SIZE>
void CrlBuff<T, SIZE>::add(const T &t){
  _data[_index] = t;

  _index = (_index + 1) % SIZE;

  if(_count < SIZE)
    _count ++;
}

template <typename T, const int SIZE>
const T& CrlBuff<T, SIZE>::operator[](int index) const{
  return _data[(_index + index) % SIZE];
}

template <typename T, const int SIZE>
T& CrlBuff<T, SIZE>::operator[](int index){
  return _data[(_index + index) % SIZE];
}

template <typename T, const int SIZE>
int CrlBuff<T, SIZE>::size() const{
  return SIZE;
}

template <typename T, const int SIZE>
int CrlBuff<T, SIZE>::count() const{
  return _count;
}


/////////////////////////////////////////////
// Statistics
class Stats{
public:
  Stats();

  void reset();
  void process(uint8_t val);

  uint8_t getMax() const;
  uint8_t getMin() const;

private:
  uint8_t          _count;

  CrlBuff<uint8_t> _mins;    
  CrlBuff<uint8_t> _maxs;    
  uint8_t          _minCur;
  uint8_t          _maxCur;
};



//////////////////////////////////////////////
// Representation for sound capture visualization

#define SC_MAX_BANDS 7

#define SC_MAP_USE_MIN      0x01 //Use running min for lower boundary
#define SC_MAP_USE_MAX      0x02 //Use running max for upper boundary
#define SC_MAP_ABOVE_NOISE  0x04 //Make 0 everything below min
#define SC_MAP_SENSITIVITY  0x08 //Make 0 if max - min > sensitivity
#define SC_MAP_LOG          0x10 //Logarithmic scale


typedef struct {
  uint8_t bands[SC_MAX_BANDS]; //8 bit for each band

  uint8_t lower;            //Lower boundary
  uint8_t upper;            //Upper boundary
  uint8_t max;              //Running maximum
  uint8_t min;              //Running minimum
  uint8_t sensitivity;      //threshold between max and min
  uint8_t flags;            //default scaling flags

  uint8_t scale(uint8_t band, uint8_t flg = 0) const{

    flg = flg == 0 ? flags : flg;

    uint8_t mn  = (flg & SC_MAP_USE_MIN) ? min : lower;
    uint8_t mx  = (flg & SC_MAP_USE_MAX) ? max : upper;
    uint8_t val = ((flg & SC_MAP_ABOVE_NOISE) && (bands[band] < mn)) || ((flg & SC_MAP_SENSITIVITY) && (max - min) <= sensitivity) ? 0 : map(bands[band] > mx ? mx : bands[band], mn, mx, 0, 255);
    
    if(flg & SC_MAP_LOG){
      val = val == 0 ? 0 : log10(val) / log10(255) * 255;      
    }  

    return val;
  }

} band8_visual; 


////////////////////////////////////////////
// EffectSound - sound effect 

class SoundCapture;

class EffectSound: public Effect{
  public:
    EffectSound();
    ~EffectSound();

    static void initSoundCapture(SoundCapture *sc);
    
    virtual bool onCmd(struct CtrlQueueItemEx &itm);
    
  protected:
    virtual void reset();
    virtual void proceed(CRGB *leds, uint16_t numLeds);    

    virtual void updateLeds(CRGB *leds, 
                            uint16_t numLeds, 
                            const band8_visual &data);

    void getBands(band8_visual &output);

    //Primitives
    void drawBand(CRGB *leds,
                  uint16_t numLeds,                  
                  uint16_t center,
                  uint8_t volume,
                  uint8_t colorIndexFirst,
                  uint8_t colorIndexLast
                );

    void drawBandSymmetric(CRGB *leds,
                           uint16_t numLeds,
                           uint8_t volume,
                           uint8_t colorIndexFirst,
                           uint8_t colorIndexLast
                         ); 

    void drawBoundary(CRGB *leds, 
                      uint16_t numLeds, 
                      uint8_t bottom,
                      uint8_t lower, 
                      uint8_t upper,
                      const CRGB &clrLower,
                      const CRGB &clrUpper,
                      bool eraseBackground = true
                     );
              


  private:
    static SoundCapture *_sc;         //Single instance of sound capture
    
    static uint8_t      _lower;       //Lower boundary from 0 to _upper
    static uint8_t      _upper;       //Upper boundary from _lower to 255
    static uint8_t      _sensitivity; //max - min threshold, 0 - 255

    static Stats        _stats;      //Max, min, etc

    static uint8_t      _flags;      //Flags how to scale sound capture
};



#endif //__EFFECT_SOUND_H


