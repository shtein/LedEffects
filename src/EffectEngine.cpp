#include "LedEffects.h"
#include <Fastled.h>
#include <Controls.h>
#include <CtrlSerial.h>
#include <EEPROMCfg.h>

#include "Effect.h"
#include "EffectEngine.h"
#include "EffectEngineCtx.h"

#define CUR_MODE _modes[_modeNum]  

EffectEngine::EffectEngine(uint8_t flags){
  _flags    = flags;
  
  _numModes = 0;
  _modeNum  = 0;
  ::memset(_modes, 0, sizeof(_modes));

  _numLeds = MAX_LEDS;

  _millis          = 0;
  _millisToSaveCfg = 0;

  //FastLed randmization seeding Random number initialization  
  random16_set_seed(analogRead(0)); 
}

CRGB *EffectEngine::getLeds() const{
  return (CRGB *)_leds;
}

EffectEngine::~EffectEngine(){
}

void EffectEngine::addMode(const __FlashStringHelper *modeName, EFFECT_EFFECT *effects){
  if(_numModes == MAX_MODES)
    return;

  _modes[_numModes].effects   = effects;
#ifdef NTF_ENABLED  
  _modes[_numModes].modeName  = modeName; 
#endif  

  //Next mode
  _numModes ++;
}

void EffectEngine::addEffect(const __FlashStringHelper *effectName, Effect *effect){
  if(!effect)
    return;

  if(_numModes == 0)
    return;

  EFFECT_MODE *mode = & _modes[_numModes - 1]; 
  
  //Only add effect if there is where to add
  if(mode->effects){
    mode->effects[mode->numEffects].effect     = effect;
#ifdef NTF_ENABLED
    mode->effects[mode->numEffects].effectName = effectName;
#endif    

    mode->numEffects ++;
  }
} 

void EffectEngine::init() {  
 
  //Don't uncomment it if you don;t know what it is 
  //FastLED.setMaxPowerInVoltsAndMilliamps(5,1000);

  fill_solid(_leds, MAX_LEDS, CRGB::Black);
  
  //Try to read from EEPROM
  readConfig();

  //Check startup flags
  if(_flags & EFF_RANDOM_START_MODE){
    _modeNum           = random8(_numModes); //random mode
    CUR_MODE.effectNum = 0;                  //first effect  
  }

  if(_flags & EFF_RANDOM_START_EFFECT){
    CUR_MODE.effectNum = random8(CUR_MODE.numEffects); //random effect
  }

  //Set mode
  setMode(_modeNum);

  //Light LEDs
  FastLED.show();
}



void EffectEngine::setMode(uint8_t mode){
  //Remember new mode
  _modeNum = mode;

  //Black the lights
  fill_solid(_leds, MAX_LEDS, CRGB::Black);

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

    //Read config previously saved data
    configCurEffect(true);

    //Process right away
    _millis = 0;
  }
}

Effect *EffectEngine::getEffect() const{
  if(_numModes == 0)
    return NULL; 

  if(!CUR_MODE.effects)
    return NULL;

  return CUR_MODE.effects[CUR_MODE.effectNum].effect; 
}

//////////////////////////////
//Command handlers

void EffectEngine::onModeChange(struct CtrlQueueData &data){ 
  //Get new mode value    
  uint8_t mode = (uint8_t)data.translate(_modeNum, 0, _numModes - 1);
  
  //Do nothing if did not change
  if(_modeNum != mode){    
    //Change mode
    setMode(mode);  
  }
}


void EffectEngine::onEffectChange(struct CtrlQueueData &data){
  
  //Get new effect value  
  uint8_t effectNum = (uint8_t)data.translate(CUR_MODE.effectNum, 0, CUR_MODE.numEffects - 1);
  
  //Black the leds
  fill_solid(_leds, MAX_LEDS, CRGB::Black);
  
  //Change effect
  setEffect(effectNum);
}

void EffectEngine::onNumLedsChange(struct CtrlQueueData &data){
  
  //Get new mode value
  uint16_t numLeds = data.translate(_numLeds, 0, MAX_LEDS);

  //Do nothing if did not change
  if(_numLeds == numLeds)
    return;

  //Save new value
  _numLeds = numLeds;

 //Black the lights
  fill_solid(_leds + _numLeds, MAX_LEDS - _numLeds, CRGB::Black);
}



bool EffectEngine::onCmd(struct CtrlQueueItemEx &itm){
  //Initial error
  bool processed = true;
  
  //Process command
  switch(itm.cmd){
    case EEMC_MODE:            
      onModeChange(itm.data);
#ifdef NTF_ENABLED     
      { itm.ntf.put_F(NULL, EECmdResponse<EEResp_ModeEffect> {itm.cmd, {_modeNum, CUR_MODE.effectNum}}); }
#endif                   
    break;

    case EEMC_EFFECT:    
      onEffectChange(itm.data);    
#ifdef NTF_ENABLED     
      { itm.ntf.put_F(NULL, EECmdResponse<EEResp_ModeEffect> {itm.cmd, {_modeNum, CUR_MODE.effectNum}}); }
#endif                   
    break;

    case EEMC_NUMLEDS:
      onNumLedsChange(itm.data);    
#ifdef NTF_ENABLED               
      { itm.ntf.put_F(NULL, EECmdResponse<EEResp_NumLeds>{ itm.cmd, { MAX_LEDS, (uint16_t)_numLeds }}); }
#endif      
    break;

#ifdef NTF_ENABLED
    //Error and Get commands
    case EEMC_GET_MODE:
      { itm.ntf.put_F(NULL, EECmdResponse<EEResp_ModeEffect> {itm.cmd, {_modeNum, CUR_MODE.effectNum}}); }
    break;
    case EEMC_GET_EFFECT:
      { itm.ntf.put_F(NULL, EECmdResponse<EEResp_ModeEffect> {itm.cmd, {_modeNum, CUR_MODE.effectNum}}); }
    break;
    case EEMC_GET_NUMLEDS:
      { itm.ntf.put_F(NULL, EECmdResponse<EEResp_NumLeds>{ itm.cmd, { MAX_LEDS, (uint16_t)_numLeds }} ); }
    break;
    case EEMC_GET_MODE_LIST:{
        EECmdResponse<EEResp_ModeList> resp;
        resp.cmd           = itm.cmd;
        resp.data.numModes = _numModes;
        for(size_t i = 0; i < _numModes; i++){
          resp.data.modes[i].numEffects = _modes[i].numEffects;
          resp.data.modes[i].modeName   = _modes[i].modeName;

          for(size_t j = 0; j < _modes[i].numEffects; j++){
            resp.data.modes[i].effects[j].index = j;          
            resp.data.modes[i].effects[j].effectName = _modes[i].effects[j].effectName;
          }
        }
        
        itm.ntf.put_F(NULL, resp);
      }
    break;
    case EEMC_ERROR:
      { itm.ntf.put_F(NULL, EECmdResponse<>{ itm.cmd, EEER_INVALID } ); }
    break;

#endif    

    default: //Processed by current effect
      Effect *curEffect = getEffect();      
      if(curEffect){
         processed = curEffect->onCmd(itm);
      }
    break;
  };

#ifdef NTF_ENABLED
// Process unhandled error
  if(!processed){
    { itm.ntf.put_F(NULL, EECmdResponse<>{ itm.cmd, EEER_UNHANDLED } ); }
  }
#endif

  return processed;
}

void EffectEngine::loop(struct CtrlQueueItemEx &itm){
  
  //Process command if there is command    
  if(itm.cmd != EEMC_NONE){
    onCmd(itm);  
  }
  
  //Proceed with current effect
  bool updateLeds = (itm.cmd & EEMC_LED) != EEMC_NONE;
  if(updateLeds){
    preSaveConfig(); 
  }

  //Current effect
  Effect  *curEffect = getEffect();

  if(curEffect != NULL){

    //Is it time to process ?
     if(_millis <= millis()){

        //Proceed
        curEffect->loop(_leds, _numLeds);
        
        //Remember when proceed next time
        _millis = millis() + curEffect->getSpeedDelay();
        
        updateLeds = true;
     }
  }
  
  //Updates leds and set timer to save configuration
  if(updateLeds){
     FastLED.show();
  }

  //See if we need to safe config
  if(_millisToSaveCfg != 0 && _millisToSaveCfg < millis()){
    //Reset
    _millisToSaveCfg = 0;
    
    //Safe config
    writeConfig();
    configCurEffect(false);  
  }
}

////////////////////////
// Read/Write configuration

void EffectEngine::readConfig(){
  EEPROMCfg ee(EE_ENGINE_IDX);

  uint8_t version;
  uint8_t total;
  uint8_t current;

  //Read version, number of modes, current mode
  ee >> version >> total >> current;

  //Check if version is correct 
  if(version != EE_VERSION)
    return;

  //Check if number of modes match
  if(total != _numModes)
    return;

  //Set current mode
  _modeNum = current < total ? current : 0;
 
  //For each mode check consistency and set current effect  
  for(uint8_t i = 0; i < _numModes; i++){
    //Read total and current
    ee >> total >> current;

    //Check if number of effect matchs
    if(total != _modes[i].numEffects)
      continue;

    //Save mode effect
    _modes[i].effectNum = current < total ? current : 0;
  }
}

void EffectEngine::writeConfig(){ 
  EEPROMCfg ee(EE_ENGINE_IDX);

  //Write version, number of modes and current mode
  ee << (uint8_t)EE_VERSION <<  (uint8_t) _numModes << (uint8_t)_modeNum;

  //Write number of effects and current effect for each mode
  for(uint8_t i = 0; i < _numModes; i++){
    ee << (uint8_t)_modes[i].numEffects << (uint8_t)_modes[i].effectNum;;
  }  
}

void EffectEngine::configCurEffect(bool read){  

  //Check if there is current effect
  Effect *curEffect = getEffect();

  if(!curEffect)
    return;
  
  //Calculate EEPROM index for 
  uint8_t index = EE_EFFECT_IDX; 
  for(int i = 0; i < _modeNum; i++){
      index += _modes[i].numEffects * EE_EFFECT_IDX_SIZE; 
  }
  index += CUR_MODE.effectNum * EE_EFFECT_IDX_SIZE;

  //Read or write current effect config
  EEPROMCfg ee(index);  
  curEffect->config(ee, read);
}


void EffectEngine::preSaveConfig(){
  _millisToSaveCfg = millis() + SAVE_CONFIG_TIMEOUT;
}

/////////////////////////////
// parseSerialInput
// Nothing fancy, enter from terminal if serial inteface is available
BEGIN_PARSE_ROUTINE(parseSerialInput)  
  BEGIN_GROUP_TOKEN("mode|m") //mode 
    TOKEN_IS_TEXT("list|l", EEMC_GET_MODE_LIST)
    TOKEN_IS_TEXT("get|g|", EEMC_GET_MODE)
    BEGIN_GROUP_TOKEN("set|s") //sets     
      TOKEN_IS_TEXT("next|n|", EEMC_MODE, 0, CTF_VAL_NEXT) //next mode
      TOKEN_IS_TEXT("prev|p", EEMC_MODE, 0, CTF_VAL_PREV)  //prev mode
      TOKEN_IS_NUMBER(EEMC_MODE, CTF_VAL_ABS)              //specific mode
    END_GROUP_TOKEN()  

    BEGIN_GROUP_TOKEN("effect|e") //effect
      TOKEN_IS_TEXT("get|g|", EEMC_GET_EFFECT)
      BEGIN_GROUP_TOKEN("set|s") //sets     
        TOKEN_IS_TEXT("next|n|", EEMC_EFFECT, 0, CTF_VAL_NEXT) //next effect
        TOKEN_IS_TEXT("prev|p", EEMC_EFFECT, 0, CTF_VAL_PREV)  //prev effect
        TOKEN_IS_NUMBER(EEMC_EFFECT, CTF_VAL_ABS)              //specific effect
      END_GROUP_TOKEN()
    END_GROUP_TOKEN()
  END_GROUP_TOKEN() //mode

  BEGIN_GROUP_TOKEN("effect|e") //current effect
    BEGIN_GROUP_TOKEN("speed|s")
      TOKEN_IS_TEXT("get|g|", EEMC_GET_SPEED)
      TOKEN_IS_PAIR("set|s", EEMC_SPEED, CTF_VAL_ABS)      
    END_GROUP_TOKEN()    
    
    BEGIN_GROUP_TOKEN("color|c") //set hsv 
      TOKEN_IS_TEXT("get|g|", EEMC_GET_COLOR_HSV)
      BEGIN_GROUP_TOKEN("set|s") //set hsv    
        TOKEN_IS_PAIR("hue|h", EEMC_COLOR_HUE, CTF_VAL_ABS)  //hue
        TOKEN_IS_PAIR("sat|s", EEMC_COLOR_SAT, CTF_VAL_ABS)  //saturation
        TOKEN_IS_PAIR("val|v", EEMC_COLOR_VAL, CTF_VAL_ABS)  //value
      END_GROUP_TOKEN()
    END_GROUP_TOKEN()    
  END_GROUP_TOKEN() //effect

  BEGIN_GROUP_TOKEN("leds|l") //num leds
    TOKEN_IS_TEXT("get|g|", EEMC_GET_NUMLEDS)
    TOKEN_IS_PAIR("set|s", EEMC_NUMLEDS, CTF_VAL_ABS) //sets     
  END_GROUP_TOKEN() //leds

END_PARSE_ROUTINE()