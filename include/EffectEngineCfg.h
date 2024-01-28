#ifndef __EFFECT_ENGINE_CFG_H
#define __EFFECT_ENGINE_CFG_H

/////////////////////////////////////////////////
// Configuration DB in flash memory
// All functions below return either true/false

//Current version
#define EFFECT_ENGINE_VERSION_HIGH   0x07
#define EFFECT_ENGINE_VERSION_LOW    0x00

// Limits - 10 modes and 40 effects
#define MODES_MAX     10  //Maximum number of modes
#define EFFECTS_MAX   40  //Maximum number of effects


//Effect Engine Version
struct EFFECT_ENGINE_VERSION{
  uint8_t verHigh;
  uint8_t verLow;
};

bool getEngineVersion(EFFECT_ENGINE_VERSION &ver);
bool checkEngineVersion();
bool eraseEngineConfig();

//Effect Engine configuration 
struct EFFECT_ENGINE_CONFIG{
  uint8_t               flags;        //Flags
  uint8_t               numModes;     //Number of modes
  uint8_t               modeNum;      //Current mode
};

bool getEngineConfig(EFFECT_ENGINE_CONFIG &cfg);
bool setEngineConfig(const EFFECT_ENGINE_CONFIG &cfg);



//Effect Mode configuration
#define MODE_NAME_LEN   9

struct EFFECT_MODE_CONFIG{
  char    name[MODE_NAME_LEN]; //Mode name
  uint8_t numEffects;          //Number of effects
  uint8_t effectNum;           //Current effect 
};

//Mode config
bool addMode(const char *modeName);
bool getModeConfig(uint8_t mode, EFFECT_MODE_CONFIG &cfg);
bool setModeConfig(uint8_t mode, const EFFECT_MODE_CONFIG &cfg);

//Effect configuration
#define EFFECT_DATA_LEN 7

struct EFFECT_CONFIG{  
  uint8_t effect;                 //Effect id
  uint8_t speedDelay;             //Speed
  uint8_t data[EFFECT_DATA_LEN];  //Data
};

//Effect config
bool addEffect(const EFFECT_CONFIG &cfg);
bool getEffectConfig(uint8_t mode, uint8_t effect, EFFECT_CONFIG &cfg);
bool setEffectConfig(uint8_t mode, uint8_t effect, const EFFECT_CONFIG &cfg);





#endif //__EFFECT_ENGINE_CFG_H