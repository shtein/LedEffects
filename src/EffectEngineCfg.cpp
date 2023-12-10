#include "LedEffects.h"
#include "EffectEngineCfg.h"
#include <EEPROMCfg.h>

// Engine version structure

//  version - 2 bytes  |
// ---------------------
// | verhigh | verlow  |
// |  8 bit  | 8 bit   |
// ---------------------


// Engine config structure

// |      engine - 3 bytes       |       effect slots - 5 bytes, 40 effects     |         spare             |
// ----------------------------------------------------------------------------------------------------------
// |  flags  | numModes| modeNum |        |        |        |         |         |        |        |         | 
// |  8 bit  |  10 bit | 6 bit   |        |        |        |         |         |        |        |         |
// ----------------------------------------------------------------------------------------------------------


struct EFFECT_ENGINE_CONFIG{
  uint8_t  flags;
  uint16_t numModes:10;
  uint16_t modeNum:6;
  uint8_t  effects[5];
};


// Effect mode config structure
//                                   name - 8 byte                                 |             effect slots - 5 byte               | effect |
// --------------------------------------------------------------------------------------------------------------------------------------------
// |         |         |         |         |         |         |         |         |         |         |         |         |         |        |
// |         |         |         |         |         |         |         |         |         |         |         |         |         |        |
// --------------------------------------------------------------------------------------------------------------------------------------------

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

