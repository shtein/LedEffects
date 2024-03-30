#include "LedEffects.h"
#include <EEPROMCfg.h>
#include <Controls.h>
#include "Effect.h"
#include "EffectsAll.h"
#include "EffectEngineCfg.h"





// Config DB structure

//  Engine Config  |   Spare  |        Offsets       |      Mode Config     |    Effects Configs   |      Mode Config     |    Effects Configs   |
// ----------------|---------------------------------|----------------------|----------------------|----------------------|----------------------|
//     7 bytes     |  4 bytes |  MAX_MODES * 1 bytes |        20 bytes      |  effects * 12 bytes  |         ...          |         ...          |
// ----------------------------------------------------------------------------------------------------------------------------------------------|

// Engine config structure

//  version - 2 bytes  |              engine - 5 bytes                     |
// -------------------------------------------------------------|----------|
// | verhigh | verlow  |  flags  | modes   |  mode   |        numLeds      |
// |  8 bit  | 8 bit   |  8 bit  |  8 bit  | 8 bit   |        16 bit       |
// ------------------------------------------------------------------------|



// Effect mode config structure

// |                                  mode 18 bytes                                |
// |-------------------------------------------------------------------------------|
// |                    name 16 byte                           | effects | effect  |
// |         |         |         |         |         |         |  8 bit  |  8 bit  |
// |-------------------------------------------------------------------------------|


// Effect config

// |                             effect - 10 bytes                                           |
// -------------------------------------------------------------------------------------------
// |   id    |  speed  |  flags  |             data 9 bytes                        |         |
// |  8 bit  |  8 bit  |  8 bit  |         |         |         |         |         |         |
// -------------------------------------------------------------------------------------------


//Sizes
#define EFFECT_ENGINE_SIZE           sizeof(EFFECT_ENGINE_CONFIG)
#define EFFECT_ENGINE_SPARE_SIZE     4 //4 spare bytes
#define INDEX_SIZE                   MODES_MAX
#define EFFECT_MODE_SIZE             sizeof(EFFECT_MODE_CONFIG)
#define EFFECT_SIZE                  sizeof(EFFECT_CONFIG)

// Offsets 
#define EFFECT_ENGINE_VERSION_OFFSET            0
#define EFFECT_ENGINE_OFFSET                    (EFFECT_ENGINE_VERSION_OFFSET + sizeof(EFFECT_ENGINE_VERSION))
#define INDEX_OFFSET                            (EFFECT_ENGINE_OFFSET + EFFECT_ENGINE_SIZE + EFFECT_ENGINE_SPARE_SIZE)
#define EFFECT_MODE_OFFSET(mode, indexValue)    (INDEX_OFFSET +  INDEX_SIZE + mode * EFFECT_MODE_SIZE + indexValue * EFFECT_SIZE) 
#define EFFECT_OFFSET(mode, indexValue, effect) (EFFECT_MODE_OFFSET(mode, indexValue) + EFFECT_MODE_SIZE + effect * EFFECT_SIZE) 

// Config db max size
#define CONFIG_SIZE 1024


//////////////////////////////////////////////
// Engine version
bool getConfigVersion(EFFECT_ENGINE_VERSION &ver){  
  EEPROMCfg ee(EFFECT_ENGINE_VERSION_OFFSET);  
  ee >> ver.verHigh >> ver.verLow;

  return true;
}

//Check version
bool checkConfigVersion(){

  //Retrive version
  EFFECT_ENGINE_VERSION ver;
  getConfigVersion(ver);

  return ver.verHigh < EFFECT_ENGINE_VERSION_HIGH ? false : true;
}


//////////////////////////////////////////////
// Engine 


bool getEngineConfigInt(EEPROMCfg &ee, EFFECT_ENGINE_CONFIG &cfg){
  ee.moveTo(EFFECT_ENGINE_OFFSET);
  ee >> cfg;

  return true;
}

bool setEngineConfigInt(EEPROMCfg &ee, const EFFECT_ENGINE_CONFIG &cfg){
  ee.moveTo(EFFECT_ENGINE_OFFSET);
  ee << cfg;

  return true;
}


bool prepareEngineConfig(uint8_t flags){
  //Erase config    
  EEPROMCfg ee(EFFECT_ENGINE_VERSION_OFFSET);
  for(size_t i = 0; i < CONFIG_SIZE; i++)
    ee.write((uint8_t)0);  

  //Write config version
  ee.moveTo(EFFECT_ENGINE_VERSION_OFFSET);  
  ee << (uint8_t)EFFECT_ENGINE_VERSION_HIGH << (uint8_t)EFFECT_ENGINE_VERSION_LOW;

  EFFECT_ENGINE_CONFIG cfgEng;
  cfgEng.flags    = flags;
  cfgEng.numLeds  = 0;
  cfgEng.numModes = 0;
  cfgEng.modeNum  = 0;

  setEngineConfigInt(ee, cfgEng);

  return true;
}



bool getEngineConfig(EFFECT_ENGINE_CONFIG &cfg){
  EEPROMCfg ee;
  
  return getEngineConfigInt(ee, cfg);
}

bool setEngineConfig(const EFFECT_ENGINE_CONFIG &cfg){
  EEPROMCfg ee;
  EFFECT_ENGINE_CONFIG cfgInt;

  //Retrieve internal configuration
  if(!getEngineConfigInt(ee, cfgInt)){
    return false;
  }

  //Convert data
  cfgInt.flags   = cfg.flags;
  cfgInt.modeNum = cfg.modeNum;
  cfgInt.numLeds = cfg.numLeds;

  return setEngineConfigInt(ee, cfgInt);
}


//////////////////////////////////////////////
// Mode

bool getModeIndexInt(EEPROMCfg &ee, uint8_t mode, uint8_t &value){
  if(mode >= MODES_MAX)
    return false;

  ee.moveTo(INDEX_OFFSET + mode);
  ee >> value;

  return true;
}

bool setModeIndexInt(EEPROMCfg &ee, uint8_t mode, uint8_t value){
  if(mode >= MODES_MAX)
    return false;

  ee.moveTo(INDEX_OFFSET + mode);
  ee << value;

  return true;
}


bool getModeConfigInt(EEPROMCfg &ee, uint8_t mode, uint8_t index, EFFECT_MODE_CONFIG &cfg){  
  //Read config after marker
  ee.moveTo( EFFECT_MODE_OFFSET(mode, index) );
  ee >>  cfg;

  return true;
}

bool setModeConfigInt(EEPROMCfg &ee, uint8_t mode, uint8_t index, const EFFECT_MODE_CONFIG &cfg){  

  //Write marker and config
  ee.moveTo(EFFECT_MODE_OFFSET(mode, index));
  ee << cfg;

  return true;
}


bool addModeConfig(const char *modeName){
  EEPROMCfg ee;

  //Ger engine config
  EFFECT_ENGINE_CONFIG cfgEng;
  if(!getEngineConfigInt(ee, cfgEng))
    return false;

  //Copy index from prev mode if it is not first mode
  uint8_t index  = 0;
  EFFECT_MODE_CONFIG cfgMode;
  if(cfgEng.numModes != 0){
    
    if(!getModeIndexInt(ee, cfgEng.numModes - 1, index)){
      return false;
    }

    //Get previouse mode config
    if(!getModeConfigInt(ee, cfgEng.numModes - 1, index, cfgMode)){
      return false;
    }

    index += cfgMode.numEffects;

    //Save index for new mode
    if(!setModeIndexInt(ee, cfgEng.numModes, index)){
      return false;
    }
  }

  
  
  //Save mode data
  strncpy_P(cfgMode.name, modeName, sizeof(cfgMode.name));
  cfgMode.effectNum  = 0;
  cfgMode.numEffects = 0;
  
  //Prepare and save mode config
  if(!setModeConfigInt(ee, cfgEng.numModes, index, cfgMode))
    return false;

  cfgEng.numModes ++;
  return setEngineConfigInt(ee, cfgEng);
}

bool getModeConfig(uint8_t mode, EFFECT_MODE_CONFIG &cfg){
  EEPROMCfg ee;
  
  uint8_t index = 0;
  if(!getModeIndexInt(ee, mode, index)){
    return false;
  } 

  //Retrieve internal configuration
  return  getModeConfigInt(ee, mode, index, cfg);
}

bool setModeConfig(uint8_t mode, const EFFECT_MODE_CONFIG &cfg){
  EEPROMCfg ee;

  uint8_t index = 0;
  if(!getModeIndexInt(ee, mode, index)){
    return false;
  }

  return setModeConfigInt(ee, mode, index, cfg);
}


/////////////////////////
//Effect
bool getEffectConfigInt(EEPROMCfg &ee, uint8_t mode, uint8_t index, uint8_t effect, EFFECT_CONFIG &cfg){

  ee.moveTo(EFFECT_OFFSET(mode, index, effect));
  ee >> cfg;


  return true;
}

bool setEffectConfigInt(EEPROMCfg &ee, uint8_t mode, uint8_t index, uint8_t effect, const EFFECT_CONFIG &cfg){

  ee.moveTo(EFFECT_OFFSET(mode, index, effect));
  ee << cfg;
  

  
  return true;
}


bool addEffectConfig(const EFFECT_CONFIG &cfg){
  EEPROMCfg ee;
  
  //Get Engine config
  EFFECT_ENGINE_CONFIG cfgEng;
  if(!getEngineConfigInt(ee, cfgEng)){
    return false;
  }

  //Check if there is mode
  if(cfgEng.numModes == 0){
    return false;
  }

  //Get index of the last mode
  uint8_t index = 0;
  if(!getModeIndexInt(ee, cfgEng.numModes - 1, index)){
    return false;
  }
  
  //Get the last mode configuiration
  EFFECT_MODE_CONFIG cfgMode;
  if(!getModeConfigInt(ee, cfgEng.numModes - 1, index, cfgMode)){
    return false;
  }

  //Add new effect 
  if(!setEffectConfigInt(ee, cfgEng.numModes - 1, index, cfgMode.numEffects, cfg)){
    return false;
  }  

  //Increase number of effects
  cfgMode.numEffects ++;
  if(setModeConfigInt(ee, cfgEng.numModes - 1, index, cfgMode)){
    return false;
  }

  //Increment index
  index++;
  if(!setModeIndexInt(ee, cfgEng.numModes - 1, index)){
    return false;
  }

  return true;
}




bool addEffectConfig(uint8_t effectId,  const EFFECT_DATA &data){
  EFFECT_CONFIG cfg;

  memset(&cfg, 0, sizeof(cfg));

  //Check ID 
  EFFECT_DESCRIPTION ed;
  if(!getEffect(effectId, ed)){
    return false;
  }

  ed.effect->reset();

  cfg.effectId    = effectId;
  cfg.speedDelay  = ed.effect->getSpeedDelay();
  cfg.data        = data;
  cfg.data.flags |= ed.flags; 

  return addEffectConfig(cfg);  
}

bool getEffectConfig(uint8_t mode, uint8_t effect, EFFECT_CONFIG &cfg){
  EEPROMCfg ee;
  
  //Get mode index
  uint8_t index = 0;
  if(!getModeIndexInt(ee, mode, index)){
    return false;
  }

  //Get Mode configuartion
  EFFECT_MODE_CONFIG cfgMode;
  if(!getModeConfigInt(ee, mode, index, cfgMode) ){
    return false;
  }

  //Check if effec number is valid
  if(effect >= cfgMode.numEffects ){
    return false;
  }

  
  return getEffectConfigInt(ee, mode, index, effect, cfg);
}

bool setEffectConfig(uint8_t mode, uint8_t effect, const EFFECT_CONFIG &cfg){
  EEPROMCfg ee;

  //Get mode index
  uint8_t index = 0;
  if(!getModeIndexInt(ee, mode, index)){
    return false;
  }

  //Get Mode configuartion
  EFFECT_MODE_CONFIG cfgMode;
  if(!getModeConfigInt(ee, mode, index, cfgMode) ){
    return false;
  }

  //Check if effec number is valid
  if(effect >= cfgMode.numEffects ){
    return false;
  }

  //Retreieve the data
  return setEffectConfigInt(ee, mode, index, effect, cfg);
}
