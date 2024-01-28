#include "LedEffects.h"
#include "EffectEngineCfg.h"
#include <EEPROMCfg.h>

//Utility functions for internal use


void copyName(uint8_t *dst, const char *src, size_t maxLen){
  size_t nameLen = src ? min(strlen(src), maxLen) : 0;
  memcpy(dst, src, nameLen);
  memset(dst + nameLen, 0, maxLen - nameLen);
}


// Engine version structure

//  version - 2 bytes  |
// ---------------------
// | verhigh | verlow  |
// |  8 bit  | 8 bit   |
// ---------------------


// Engine config structure

// |      engine - 3 bytes       | 
// -------------------------------
// |  flags  | modes   |  mode   |
// |  8 bit  |  4 bit  | 4 bit   |
// -------------------------------


// Effect mode config structure

// |                                           mode 11 bytes                                                     |
// ---------------------------------------------------------------------------------------------------------------
//                                    name 8 bytes                                 | effects | effect  | first   |
// |         |         |         |         |         |         |         |         |  8 bit  |  8 bit  | 8 bit   |
// ---------------------------------------------------------------------------------------------------------------


// Effect config

// |                             effect - 9 bytes                                            |
// -------------------------------------------------------------------------------------------
// |   id    |  speed  |                      data 7 bytes                         |         |
// |  8 bit  |  8 bit  |         |         |         |         |         |         |         |
// -------------------------------------------------------------------------------------------

// Config db max size
#define CONFIG_SIZE 512

// Offsets 
#define EFFECT_ENGINE_VERSION_OFFSET 0
#define EFFECT_ENGINE_OFFSET         sizeof(EFFECT_ENGINE_VERSION)
#define EFFECT_ENGINE_SIZE           sizeof(EFFECT_ENGINE_CONFIG_INTERNAL)
#define EFFECT_ENGINE_SPARE          4 //4 spare bytes

#define EFFECT_MODE_OFFSET           EFFECT_ENGINE_OFFSET + EFFECT_ENGINE_SIZE + EFFECT_ENGINE_SPARE 
#define EFFECT_MODE_SIZE             sizeof(EEFFECT_MODE_CONFIG_INTERNAL)
#define EFFECT_MODE_BLOCK_SIZE       (EFFECT_MODE_SIZE * MODES_MAX)                // 10 modes * 11 bytes
#define EFFECT_MODE_SPARE            4 //4 spare bytes

#define EFFECT_OFFSET                (EFFECT_MODE_OFFSET + EFFECT_MODE_BLOCK_SIZE + EFFECT_MODE_SPARE) 
#define EFFECT_SIZE                  sizeof(EFFECT_CONFIG)
#define EFFECT_BLOCK_SIZE            (EFFECT_CONFIG * EFFECTS_MAX)



//////////////////////////////////////////////
// Engine version


bool getEngineVersion(EFFECT_ENGINE_VERSION &ver){  
  EEPROMCfg ee(EFFECT_ENGINE_VERSION_OFFSET);  
  ee >> ver.verHigh >> ver.verLow;

  return true;
}

//Check version
bool checkEngineVersion(){
  //Retrive version
  EFFECT_ENGINE_VERSION ver;
  getEngineVersion(ver);

  //Simple case - erase everything if saved version high is lower than current one
  if(ver.verHigh < EFFECT_ENGINE_VERSION_HIGH){
    eraseEngineConfig();
  }

  //Save current version
  ver.verHigh = EFFECT_ENGINE_VERSION_HIGH;
  ver.verLow  = EFFECT_ENGINE_VERSION_LOW;

  EEPROMCfg ee(EFFECT_ENGINE_VERSION_OFFSET);
  ee << ver.verHigh << ver.verLow;

  return true;
}


bool eraseEngineConfig(){
  EEPROMCfg ee;
  for(size_t i = 0; i < CONFIG_SIZE; i++)
    ee.write(0);  

  return true;
}



//////////////////////////////////////////////
// Engine 

struct EFFECT_ENGINE_CONFIG_INTERNAL{
  uint8_t  flags;
  uint8_t  numModes:4;
  uint16_t modeNum:4;
};

bool getEngineConfigInt(EEPROMCfg &ee, EFFECT_ENGINE_CONFIG_INTERNAL &cfg){
  ee.moveTo(EFFECT_ENGINE_OFFSET);
  ee >> cfg;

  return true;
}

bool setEngineConfigInt(EEPROMCfg &ee, const EFFECT_ENGINE_CONFIG_INTERNAL &cfg){
  ee.moveTo(EFFECT_ENGINE_OFFSET);
  ee << cfg;

  return true;
}

bool getEngineConfig(EFFECT_ENGINE_CONFIG &cfg){
  
  EEPROMCfg ee;
  EFFECT_ENGINE_CONFIG_INTERNAL cfgInt;
  
  //Retrieve internal configuration
  if(!getEngineConfigInt(ee, cfgInt)){
    return false;
  }

  //Convert data
  cfg.flags    = cfgInt.flags;
  cfg.numModes = cfgInt.numModes;
  cfg.modeNum  = cfgInt.modeNum;

  return true;
}

//////////////////////////////////////////////
// Mode
struct EEFFECT_MODE_CONFIG_INTERNAL {
  uint8_t name[MODE_NAME_LEN - 1];
  uint8_t numEffects;
  uint8_t effectNum;
  uint8_t effectFirst;
};

bool getModeConfigInt(EEPROMCfg &ee, uint8_t mode, EEFFECT_MODE_CONFIG_INTERNAL &cfg){

  if(mode >= MODES_MAX){
    return false;
  }

  ee.moveTo(EFFECT_MODE_OFFSET + mode * EFFECT_MODE_SIZE);
  ee >> cfg;

  return true;
}

bool setModeConfigInt(EEPROMCfg &ee, uint8_t mode, const EEFFECT_MODE_CONFIG_INTERNAL &cfg){
  if(mode >= MODES_MAX){
    return false;
  }

  ee.moveTo(EFFECT_MODE_OFFSET + mode * EFFECT_MODE_SIZE);
  ee << cfg;

  return true;
}


bool addMode(const char *modeName){
  EEPROMCfg ee;
  EFFECT_ENGINE_CONFIG_INTERNAL cfgEng;
  
  //Retrieve internal configuration
  if(!getEngineConfigInt(ee, cfgEng)){
    return false;
  }

  //Check if number of modes reached its limit
  if(cfgEng.numModes == MODES_MAX){
    return false;
  }

  //If this is not first mode, need to find out where the last effect is
  uint8_t firstEffect = 0;
  EEFFECT_MODE_CONFIG_INTERNAL cfgMode;

  if(cfgEng.numModes != 0){
    if(!getModeConfigInt(ee, cfgEng.numModes - 1, cfgMode)){
      return false;
    }

    firstEffect = cfgMode.effectFirst + cfgMode.numEffects;
    DBG_OUTLN("Add mode %d", firstEffect);
  }

  //Prepare and save mode config  
  copyName(cfgMode.name, modeName, MODE_NAME_LEN - 1);
  cfgMode.numEffects  = 0;
  cfgMode.effectNum   = 0;
  cfgMode.effectFirst = firstEffect;
  if(!setModeConfigInt(ee, cfgEng.numModes, cfgMode))
    return false;

  cfgEng.numModes ++;
  return setEngineConfigInt(ee, cfgEng);
}

bool getModeConfig(uint8_t mode, EFFECT_MODE_CONFIG &cfg){
  EEPROMCfg ee;
  EEFFECT_MODE_CONFIG_INTERNAL cfgInt;
  
  //Retrieve internal configuration
  if(!getModeConfigInt(ee, mode, cfgInt)){
    return false;
  }

  //Convert data
  memcpy(cfg.name, cfgInt.name, MODE_NAME_LEN - 1);
  cfg.name[MODE_NAME_LEN - 1] = 0;
  cfg.numEffects = cfgInt.numEffects;
  cfg.effectNum  = cfgInt.effectNum;

  return true;
}

bool setModeConfig(uint8_t mode, const EFFECT_MODE_CONFIG &cfg){
  EEPROMCfg ee;
  EEFFECT_MODE_CONFIG_INTERNAL cfgInt;
  
  //Retrieve internal configuration
  if(!getModeConfigInt(ee, mode, cfgInt)){
    return false;
  }

  //Convert data
  copyName(cfgInt.name, cfg.name, MODE_NAME_LEN - 1);
  cfgInt.effectNum  = cfg.effectNum;

  //Retrieve internal configuration
  return setModeConfigInt(ee, mode, cfgInt);
}


/////////////////////////
//Effect

bool getEffectConfigInt(EEPROMCfg &ee, uint8_t slot, EFFECT_CONFIG &cfg){

  if(slot >= EFFECTS_MAX){
    return false;
  }

  ee.moveTo(EFFECT_OFFSET + slot * EFFECT_SIZE);
  ee >> cfg;

  return true;
}

bool setEffectConfigInt(EEPROMCfg &ee, uint8_t slot, const EFFECT_CONFIG &cfg){
  if(slot >= EFFECTS_MAX){
    return false;
  }

  ee.moveTo(EFFECT_OFFSET + slot * EFFECT_SIZE);
  ee << cfg;
  
  return true;
}


bool addEffect(const EFFECT_CONFIG &cfg){
  EEPROMCfg ee;
  EFFECT_ENGINE_CONFIG_INTERNAL cfgEng;

  //Get Engine config
  if(!getEngineConfigInt(ee, cfgEng)){
    return false;
  }

  //Check if there is mode
  if(cfgEng.numModes == 0){
    return false;
  }

  //Get the last mode configuiration
  EEFFECT_MODE_CONFIG_INTERNAL cfgMode;

  if(!getModeConfigInt(ee, cfgEng.numModes - 1, cfgMode)){
    return false;
  }

  //Add new effect 
  if(!setEffectConfigInt(ee, cfgMode.effectFirst + cfgMode.numEffects, cfg)){
    return false;
  }  

  //Increase number of effects
  cfgMode.numEffects ++;
  return setModeConfigInt(ee, cfgEng.numModes - 1, cfgMode);
}




bool getEffectConfig(uint8_t mode, uint8_t effect, EFFECT_CONFIG &cfg){
  EEPROMCfg ee;
  EEFFECT_MODE_CONFIG_INTERNAL cfgMode;

  //Get Mode configuartion
  if(!getModeConfigInt(ee, mode, cfgMode) ){
    return false;
  }

  //Check if effec number is valid
  if(effect >= cfgMode.numEffects ){
    return false;
  }

  
  return getEffectConfigInt(ee, cfgMode.effectFirst + effect, cfg);
}

bool setEffectConfig(uint8_t mode, uint8_t effect, const EFFECT_CONFIG &cfg){
  EEPROMCfg ee;
  EEFFECT_MODE_CONFIG_INTERNAL cfgMode;

  //Get Mode configuartion
  if(!getModeConfigInt(ee, mode, cfgMode) ){
    return false;
  }

  //Check if effec number is valid
  if(effect >= cfgMode.numEffects ){
    return false;
  }

  //Retreieve the data
  return setEffectConfigInt(ee, cfgMode.effectFirst + effect, cfg);
}







