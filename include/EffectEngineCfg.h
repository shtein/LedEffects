#ifndef __EFFECT_ENGINE_CFG_H
#define __EFFECT_ENGINE_CFG_H





/////////////////////////////////////////////////
// Configuration DB in flash memory
// All functions below return true/false

//Current version
#define EFFECT_ENGINE_VERSION_HIGH   0x07
#define EFFECT_ENGINE_VERSION_LOW    0x00

// Limits - 10 modes and 40 effects
#define MODES_MAX     15  //Maximum number of modes
#define EFFECTS_MAX   60  //Maximum number of effects


//Effect Engine Version
struct EFFECT_ENGINE_VERSION{
  uint8_t verHigh;
  uint8_t verLow;
};

bool getConfigVersion(EFFECT_ENGINE_VERSION &ver);
bool checkConfigVersiong();


//Effect Engine configuration 
struct EFFECT_ENGINE_CONFIG{
  uint8_t               flags;        //Flags
  uint8_t               numModes;     //Number of modes
  uint8_t               modeNum;      //Current mode
  uint16_t              numLeds;      //Number of leds
};


bool prepareEngineConfig(uint8_t flags);
bool getEngineConfig(EFFECT_ENGINE_CONFIG &cfg);
bool setEngineConfig(const EFFECT_ENGINE_CONFIG &cfg);

//Effect Mode configuration
#define MODE_NAME_LEN   16

struct EFFECT_MODE_CONFIG{
  char    name[MODE_NAME_LEN]; //Mode name
  uint8_t numEffects;          //Number of effects
  uint8_t effectNum;           //Current effect   
};

//Mode config
bool addModeConfig(const char *modeName); //Raw: add new mode - name is PROGMEM string
bool getModeConfig(uint8_t mode, EFFECT_MODE_CONFIG &cfg);
bool setModeConfig(uint8_t mode, const EFFECT_MODE_CONFIG &cfg);

//Effect configuration

///////////////////////
// Effect Config Flags
#define ECF_NONE          0x00
#define ECF_HSV           0x01
#define ECF_RGB           0x02
#define ECF_TRANSFORM     0x04
#define ECF_KALEYDOSCOPE  0x08


#define EFFECT_PARAM_TRANSFORM(s) s.byte

struct EFFECT_DATA{
  uint8_t    flags;
  union{        
    CHSV     hsv;
    CRGB     rgb;    
    uint8_t  byte;
  }; 
  uint8_t spare[3];

  inline EFFECT_DATA() __attribute__((always_inline)) = default;

  inline EFFECT_DATA(const EFFECT_DATA &) __attribute__((always_inline)) = default;

  inline EFFECT_DATA(uint8_t f) __attribute__((always_inline)){
    flags = f;    
  }

  inline EFFECT_DATA(uint8_t f, const CHSV &h) __attribute__((always_inline)){
    flags = f | ECF_HSV;
    hsv = h;
  }

  inline EFFECT_DATA(uint8_t f, TransformPalList t) __attribute__((always_inline)){
    flags = f | ECF_TRANSFORM;
    EFFECT_PARAM_TRANSFORM((*this)) = t;
  }
};

struct EFFECT_CONFIG{  
  uint8_t     effectId;     //Effect id
  uint8_t     speedDelay;   //Speed
  EFFECT_DATA data;         //Data    
};

//Effect config
bool addEffectConfig(uint8_t effectId, const EFFECT_DATA &data);

bool getEffectConfig(uint8_t mode, uint8_t effect, EFFECT_CONFIG &cfg);
bool setEffectConfig(uint8_t mode, uint8_t effect, const EFFECT_CONFIG &cfg);


#endif //__EFFECT_ENGINE_CFG_H