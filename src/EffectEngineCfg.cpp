#include "LedEffects.h"
#include "EffectEngineCfg.h"
#include <EEPROMCfg.h>


/////////////////////////////
// EffectConfig
EffectConfig::EffectConfig(void *buf, size_t size){
  _buf  = buf;
  _size = size;
  _pos  = 0; 
}

bool EffectConfig::read(void *p, size_t size){
  if(_pos + size > _size)
    return false;

  memcpy(p, (uint8_t *)_buf + _pos, size);
  _pos += size;

  return true;
}

bool EffectConfig::write(const void *p, size_t size){
  if(_pos + size > _size)
    return false;

  memcpy((uint8_t *)_buf + _pos, p, size);
  _pos += size;

  return true;
}



/////////////////////////
// EffectEngineCfg

// Engine config structure

//  version - 2 bytes  |      engine - 3 bytes       |       effect slots - 5 bytes, 40 effects     |         spare             |
// ------------------------------------------------------------------------------------------------------------------------------
// | verhigh | verlow  |  flags  | numModes| modeNum |        |        |        |         |         |        |        |         | 
// |  8 bit  | 8 bit   |  8 bit  |  10 bit | 6 bit   |        |        |        |         |         |        |        |         |
// ------------------------------------------------------------------------------------------------------------------------------

struct EFFECT_ENZGINE_VERSION{
  uint8_t verHigh;
  uint8_t verLow;
};

struct EFFECT_ENGINE_CONFIG{
  uint8_t  flags;
  uint16_t numModes:10;
  uint16_t modeNum:6;
  uint8_t  effects[5]; 
};


// Effect mode config structure
//                                   name - 8 byte                                 |
// ---------------------------------------------------------------------------------
// |         |         |         |         |         |         |         |         |
// |         |         |         |         |         |         |         |         |
// ---------------------------------------------------------------------------------

//             effect slots - 5 byte                 | effect |
// -------------------------------------------------------------
// |         |         |         |         |         |         |
// |         |         |         |         |         |         |
// -------------------------------------------------------------

struct EEFFECT_MODE_CONFIG {
  char    name[8];
  uint8_t effects[5];
  uint8_t effectNum;
};

#define EFFECT_ENGINE_OFFSET 0x00 
#define EFFECT_MODE_OFFSET   0x08 

// Effect config
//  effectid |  speed  |         data - 6 bytes
// ---------------------------------------------------------------------------------
// |         |         |         |         |         |         |         |         |
// |         |         |         |         |         |         |         |         |
// ---------------------------------------------------------------------------------


//Erase all data
void EffectEngineCfg::getEngineConfig(EE_CFG_EFFECT_ENGINE &cfg){
  EEPROMCfg st(EFFECT_ENGINE_OFFSET);  
  st >> cfg;
}

void EffectEngineCfg::setEngineConfig(const EE_CFG_EFFECT_ENGINE &cfg){
  EEPROMCfg st(EFFECT_ENGINE_OFFSET);
  st << cfg;
}

void EffectEngineCfg::getModeConfig(size_t mode, EE_CFG_EFFECT_MODE &cfg){
}

void EffectEngineCfg::setModeConfig(size_t mode, const EE_CFG_EFFECT_MODE &cfg){
}