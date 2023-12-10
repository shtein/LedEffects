#ifndef __EFFECT_ENGINE_CFG_H
#define __EFFECT_ENGINE_CFG_H

/////////////////////////////////
// EffectEngineCfg

//Effect Engine Version
struct EFFECT_ENGINE_VERSION{
  uint8_t verHigh;
  uint8_t verLow;
};

//Effect Enging configuration 
struct EE_CFG_EFFECT_ENGINE{
  uint8_t               flags;        //Flags
  uint8_t               numModes;     //Number of modes
  uint8_t               modeNum;      //Current mode
};

//Effect Mode configuration
struct EE_CFG_EFFECT_MODE{
  uint8_t numEffects;   //Number of effects
  uint8_t effectNum;    //Current effect
};

//Effect configuration
struct EE_CFG_EFFECT{  
  uint8_t effect;
  uint8_t speedDelay;
};


//Check version
bool checkVersion(EFFECT_ENGINE_VERSION);

//Engine config
void getEngineConfig(EE_CFG_EFFECT_ENGINE &cfg);
void setEngineConfig(const EE_CFG_EFFECT_ENGINE &cfg);

//Mode config
void getModeConfig(uint8_t mode, EE_CFG_EFFECT_MODE &cfg);
void setModeConfig(uint8_t mode, const EE_CFG_EFFECT_MODE &cfg);

//Effect config
void getEffectConfig(uint8_t mode, uint8_t effect, EE_CFG_EFFECT_MODE &cfg);
void setEffectConfig(uint8_t mode, uint8_t effect, const EE_CFG_EFFECT_MODE &cfg);



#endif //__EFFECT_ENGINE_CFG_H