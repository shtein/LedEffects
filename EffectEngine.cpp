#include "precomp.h"
#include "Effect.h"
#include "EffectEngine.h"
#include "EffectEngineCtx.h"
#include <EEPROM.h>

#define CUR_MODE _modes[_modeNum] 

EffectEngine::EffectEngine(){
  _numModes = 0;
  _modeNum     = 0;
  ::memset(_modes, 0, sizeof(_modes));

  _numLeds = 0;
  _maxLeds = 0;
  _leds    = 0;

  _millis          = 0;
  _millisToSaveCfg = 0;

  //Random number initialization
  randomSeed(analogRead(A0)); 
}

EffectEngine::~EffectEngine(){
}

void EffectEngine::addMode(Effect **effects){
  if(_numModes == MAX_MODES)
    return;

  _modes[_numModes].effects    = effects;

  //Next mode
  _numModes ++;
}

void EffectEngine::addEffect(Effect *effect){
  if(!effect)
    return;

  if(_numModes == 0)
    return;

  EFFECT_MODE *mode = & _modes[_numModes - 1]; 

  //Only add effect if there is where to add
  if(mode->effects){
    mode->effects[mode->numEffects] = effect;
    mode->numEffects ++;
  }
  
}  

void EffectEngine::init(CRGB *leds, int maxLeds) {  
 
  //Don't uncomment it if you don;t know what it is 
  //FastLED.setMaxPowerInVoltsAndMilliamps(5,1000);

  _leds    = leds;
  _numLeds = maxLeds;
  _maxLeds = maxLeds;
  
  fill_solid(_leds, _maxLeds, CRGB::Black);
  
  //Try to read from EEPROM
  readConfig();

  //Set mode
  setMode(_modeNum);

  //Light LEDs
  showStrip();
}

void EffectEngine::showStrip() {
  FastLED.show();
}


void EffectEngine::onModeChange(const struct CtrlQueueData &data){ 
  //Get new mode value    
  uint8_t mode = (uint8_t)data.translate(_modeNum, 0, _numModes - 1);
  
  //Do nothing if did not change
  if(_modeNum == mode)
    return;

  //Change mode
  setMode(mode);  

  //Safe config
  preSafeConfig();     
}

void EffectEngine::setMode(uint8_t mode){
  //Remember new mode
  _modeNum = mode;

  //Black the lights
  fill_solid(_leds, _maxLeds, CRGB::Black);

  //Refresh effect
  setEffect(CUR_MODE.effectNum);
}

void EffectEngine::setEffect(uint8_t effectNum){  
  //Set new effect number  
  CUR_MODE.effectNum = effectNum;

 //Get effect object
  Effect *curEffect = getEffect();
  
  if(curEffect != NULL){
    //Init effect
    curEffect->init(_leds, _numLeds);

    //Process right away
    _millis = 0;
  }

  DBG_OUTLN("mode %u, effect %u", _modeNum, CUR_MODE.effectNum);
}

Effect *EffectEngine::getEffect() const{
  if(_numModes == 0)
    return NULL; 

  if(!CUR_MODE.effects)
    return NULL;

  return CUR_MODE.effects[CUR_MODE.effectNum]; 
}

void EffectEngine::onNumLedsChange(const struct CtrlQueueData &data){
  
  //Get new mode value
  uint16_t numLeds = data.translate(_numLeds, 0, _maxLeds);

  //Do nothing if did not change
  if(_numLeds == numLeds)
    return;

  //Save new value
  _numLeds = numLeds;

  DBG_OUTLN("Value: %d", data.value);  
  DBG_OUTLN("New number of leds: %d", _numLeds);  

 //Black the lights
  fill_solid(_leds + _numLeds, _maxLeds - _numLeds, CRGB::Black);
}

void EffectEngine::onEffectChange(const struct CtrlQueueData &data){
  
  //Get new effect value  
  uint8_t effectNum = (uint8_t)data.translate(CUR_MODE.effectNum, 0, CUR_MODE.numEffects - 1);
  
  //Black the leds
  fill_solid(_leds, _maxLeds, CRGB::Black);
  
  //Change effect
  setEffect(effectNum);

  //Safe config
  preSafeConfig();     
}


void EffectEngine::loop(const struct CtrlQueueItem &itm){

  //Current effect
  Effect *curEffect = NULL;

  //Process command
  switch(itm.cmd){
    case EEMC_MODE:
      onModeChange(itm.data);
    break;
    case EEMC_NUMLEDS:
      onNumLedsChange(itm.data);
    break;
    case EEMC_EFFECT:
      onEffectChange(itm.data);
    break;    
    default: //Processed by current effect
      curEffect = getEffect();
      if(curEffect)
        curEffect->onCmd(itm);
    break;  
  };


  //Proceed with current effect
  bool updateLeds = (itm.cmd & EEMC_LED) != EEMC_NONE;

  //Get curretn effect if needed
  if(!curEffect)
    curEffect = getEffect();

  if(curEffect != NULL){

     //Call idle
     curEffect->idle();
      
    //Is it time to process ?
     if(_millis <= millis()){

        //Proceed
        curEffect->loop(_leds, _numLeds);
        
        //Remember when proceed next time
        _millis = millis() + curEffect->getSpeedDelay();
        
        updateLeds = true;
     }
  }
  
  //Optimization to avoid calling update leds too often
  if(updateLeds ){
     showStrip();      
  }

  //See if we need to safe config
  if(_millisToSaveCfg != 0 && _millisToSaveCfg < millis()){
    //Reset
    _millisToSaveCfg = 0;
    
    //Safe config
    writeConfig();
    DBG_OUTLN("Config saved");    
  }


}

////////////////////////
// Read/Write configuration

struct EffectEngineConfig{
  uint8_t ver;          //config version
  uint8_t numModes:4;   //total number of modes
  uint8_t modeNum:4;    //current mode
  struct EffectConfig{
    uint8_t numEffects:4;
    uint8_t effectNum:4;
  } modes[MAX_MODES];
};

#define EE_VERSION 0x02

void EffectEngine::readConfig(){
  struct EffectEngineConfig ec;

  //Read config
  EEPROM.get(0x0000, ec); 

  //Check if version is correct 
  if(ec.ver != EE_VERSION)
    return;

  //Check if number of modes match
  if(ec.numModes !=  _numModes)
    return;

  //Set current mode
  _modeNum = ec.modeNum < ec.numModes ? ec.modeNum : 0;
 
  //For each mode check consistency and set current effect
  for(uint8_t i = 0; i < _numModes; i++){
    if(ec.modes[i].numEffects != _modes[i].numEffects)
      continue;

    //Save mode effect
    _modes[i].effectNum = ec.modes[i].effectNum < ec.modes[i].numEffects? ec.modes[i].effectNum : 0;
  }

  
}

void EffectEngine::writeConfig(){
 
  struct EffectEngineConfig ec;

  //Prepare data
  ec.ver = EE_VERSION;
  ec.numModes = _numModes;
  ec.modeNum  = _modeNum;
  
  for(uint8_t i = 0; i < _numModes; i++){
    ec.modes[i].numEffects = _modes[i].numEffects;
    ec.modes[i].effectNum  = _modes[i].effectNum;
  }

  //Save config
  EEPROM.put(0x0000, ec);  
}

void EffectEngine::preSafeConfig(){
  _millisToSaveCfg = millis() + SAVE_CONFIG_TIMEOUT;
}

