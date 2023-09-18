#ifndef __EFFECT_ENGINE_CFG_H
#define __EFFECT_ENGINE_CFG_H


struct EFFECT_DATA_CONFIG{
  uint8_t speedDelay;
  union{
    CHSV hsv;
    CRGB rgb;
  };
};

struct EFFECT_CONFIG{

};

//////////////////////////////
// Effect configuration
class EffectConfig{
  public:
    EffectConfig(void *buf, size_t size);

    bool read(void *buf, size_t size); 
    bool write(const void *buf, size_t size);

    template <typename T>
    bool read(T &t){ return read(&t, sizeof(T)); }

    template <typename T>
    bool write(const T &t){ return write(&t, sizeof(T));}

    template<typename T>
    EffectConfig & operator >> (T &t){ read(t); return *this; }

    template<typename T>
    EffectConfig & operator << (const T &t){ write(t); return *this; }
    
  private:
    void   *_buf;
    size_t  _pos;
    size_t  _size;    
};


/////////////////////////////////
// EffectEngineCfg

//Header - Effect Enging structure
struct EE_CFG_EFFECT_ENGINE{
  uint8_t flags;        //Flags
  uint8_t numModes;     //Number of modes
  uint8_t modeNum;      //Current mode 
};

//Header Effect Mode structure
struct EE_CFG_EFFECT_MODE{
  uint8_t numEffects;   //Number of effects
  uint8_t effectNum;    //Current effect
};

class EffectEngineCfg{
public:

  void getEngineConfig(EE_CFG_EFFECT_ENGINE &cfg);
  void setEngineConfig(const EE_CFG_EFFECT_ENGINE &cfg);

  void getModeConfig(size_t mode, EE_CFG_EFFECT_MODE &cfg);
  void setModeConfig(size_t mode, const EE_CFG_EFFECT_MODE &cfg);
};



#endif //__EFFECT_ENGINE_CFG_H