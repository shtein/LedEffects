#include "LedEffects.h"

#include <Controls.h>
#include <EEPROMCfg.h>
#include <CtrlWiFi.h>


#include "Effect.h"
#include "EffectEngine.h"
#include "EffectEngineCtx.h"

#ifdef NTF_ENABLED  
////////////////////////////////////////////
//Notifictaions serialization
struct EEResp_ModeList{
  uint8_t     numModes;
  EFFECT_MODE *modes;
};

inline void putNtfObject(NtfBase &resp, const EFFECT_EFFECT &data){  
  if(resp.getContext().flags & NTF_CTX_ARRAY){
    resp.put_F(F("effect"), resp.getContext().arrayIndex);
  }
  resp.put_F(F("effectName"), data.effectName);
}

void putNtfObject(NtfBase &resp, const EFFECT_MODE &data){
  if(resp.getContext().flags & NTF_CTX_ARRAY){
    resp.put_F(F("mode"), resp.getContext().arrayIndex);    
  }
  resp.put_F(F("modeName"), data.modeName);
  resp.put_F(F("effectCount"), data.numEffects);   
  resp.put_F(F("effects"), data.effects, data.numEffects);
}

void putNtfObject(NtfBase &resp, const EEResp_ModeList &data){
   resp.put_F(F("modeCount"), data.numModes);
   resp.put_F(F("modes"), data.modes, data.numModes);
}
#endif


////////////////////////////////////////////
// EffectEngine
#define CUR_MODE _modes[_modeNum]  

EffectEngine::EffectEngine(uint8_t flags){
  _flags    = flags | EEF_DEFINED;
  
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
    //curEffect->init(_leds, _numLeds);
    curEffect->reset();

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


bool EffectEngine::onCmdEE(struct CtrlQueueItemEx &itm){
  
  //Process command
  switch(itm.cmd){
    case EEMC_MODE:            
      onModeChange(itm.data);
    break;

    case EEMC_EFFECT:    
      onEffectChange(itm.data);    
    break;

    case EEMC_NUMLEDS:
      onNumLedsChange(itm.data);    
    break;

#ifdef NTF_ENABLED
    //All get commands to process with NTF
    case EEMC_GET_MODE:
    case EEMC_GET_EFFECT:
    case EEMC_GET_NUMLEDS:
    case EEMC_GET_MODE_LIST:
    break;
#endif
    
    default: 
    return false;
  };

//Notifications
#ifdef NTF_ENABLED

  switch(itm.cmd){
    case EEMC_MODE:            
    case EEMC_GET_MODE:
    case EEMC_EFFECT:  
    case EEMC_GET_EFFECT:  
      { itm.ntf.put(EECmdResponse<EEResp_ModeEffect> {itm.cmd, {_modeNum, CUR_MODE.effectNum}}); }
    break;

    case EEMC_NUMLEDS:    
    case EEMC_GET_NUMLEDS:       
      { itm.ntf.put(EECmdResponse<EEResp_NumLeds>{ itm.cmd, { MAX_LEDS, (uint16_t)_numLeds }}); }
    break;
      
    case EEMC_GET_MODE_LIST:               
      { itm.ntf.put(EECmdResponse<EEResp_ModeList>{itm.cmd, {_numModes, _modes}}); }
    break;        
  }
#endif

  return true;

}

bool EffectEngine::onCmd(struct CtrlQueueItemEx &itm){
  
  //Process error
  if(itm.cmd == EEMC_ERROR) {
#ifdef NTF_ENABLED    
    itm.ntf.put(EECmdResponse<>{ itm.cmd, EEER_INVALID } );
#endif    
    return true;
  }
  
  //Initial
  bool processed = false;
  
  if(itm.cmd & EEMC_EE){ //Engine command
    processed = onCmdEE(itm);
  }  
#if defined(ESP8266) || defined(ESP32)
  else if(itm.cmd & EEMC_WIFI){ //Wifi command
    processed =  _wifi.onCmd(itm);
  }
#endif
  else{ //Effect command
      Effect *curEffect = getEffect();      
      if(curEffect){
         processed = curEffect->onCmd(itm);
      }
  }
    
  //Report of not processed
#ifdef NTF_ENABLED    
  if(!processed){
    itm.ntf.put(EECmdResponse<>{ itm.cmd, EEER_UNHANDLED } );
  }
#endif

  return processed;
}

void EffectEngine::loop(struct CtrlQueueItemEx &itm){
 
  if(itm.cmd != EEMC_NONE){
    onCmd(itm);      
  }
  
  //See if there were changes
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
        curEffect->proceed(_leds, _numLeds);
        
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
  uint8_t flags;

  //Read version, number of modes, current mode
  ee >> version >> total >> current >> flags;

  //Check if version is correct 
  if(version != EE_VERSION){
    return;
  }

  //Check if number of modes match
  if(total != _numModes)
    return;

  //Only use flags if they are set
  if(flags & EEF_DEFINED)
    _flags = flags;

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
  ee << (uint8_t)EE_VERSION <<  (uint8_t) _numModes << (uint8_t)_modeNum << (uint8_t)_flags;

  //Write number of effects and current effect for each mode
  for(uint8_t i = 0; i < _numModes; i++){
    ee << (uint8_t)_modes[i].numEffects << (uint8_t)_modes[i].effectNum;
  }  
}

void EffectEngine::configCurEffect(bool read){  

  //Check if there is current effect
  Effect *curEffect = getEffect();

  if(!curEffect)
    return;
  
  //Calculate EEPROM index for 
  size_t index = EE_EFFECT_IDX; 
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
BEGIN_PARSE_ROUTINE(parseCommandInput)  
  BEGIN_GROUP_TOKEN("mode|m") //mode 
    VALUE_IS_TOKEN("list|l", EEMC_GET_MODE_LIST)
    VALUE_IS_TOKEN("get|g|", EEMC_GET_MODE)
    BEGIN_GROUP_TOKEN("set|s") //sets     
      VALUE_IS_TOKEN("next|n|", EEMC_MODE, 0, CTF_VAL_NEXT) //next mode
      VALUE_IS_TOKEN("prev|p", EEMC_MODE, 0, CTF_VAL_PREV)  //prev mode
      VALUE_IS_NUMBER(EEMC_MODE, CTF_VAL_ABS)              //specific mode
    END_GROUP_TOKEN()  

    BEGIN_GROUP_TOKEN("effect|e") //effect
      VALUE_IS_TOKEN("get|g|", EEMC_GET_EFFECT)
      BEGIN_GROUP_TOKEN("set|s") //sets     
        VALUE_IS_TOKEN("next|n|", EEMC_EFFECT, 0, CTF_VAL_NEXT) //next effect
        VALUE_IS_TOKEN("prev|p", EEMC_EFFECT, 0, CTF_VAL_PREV)  //prev effect
        VALUE_IS_NUMBER(EEMC_EFFECT, CTF_VAL_ABS)              //specific effect
      END_GROUP_TOKEN()
    END_GROUP_TOKEN()
  END_GROUP_TOKEN() //mode

  BEGIN_GROUP_TOKEN("effect|e") //current effect
    BEGIN_GROUP_TOKEN("speed|s")
      VALUE_IS_TOKEN("get|g|", EEMC_GET_SPEED)
      VALUE_IS_PAIR("set|s", EEMC_SPEED, CTF_VAL_ABS)      
    END_GROUP_TOKEN()    
    
    BEGIN_GROUP_TOKEN("color|c") //set hsv 
      VALUE_IS_TOKEN("get|g|", EEMC_GET_COLOR_HSV)
      BEGIN_GROUP_TOKEN("set|s") //set hsv    
        VALUE_IS_PAIR("hue|h", EEMC_COLOR_HUE, CTF_VAL_ABS)  //hue
        VALUE_IS_PAIR("sat|s", EEMC_COLOR_SAT, CTF_VAL_ABS)  //saturation
        VALUE_IS_PAIR("val|v", EEMC_COLOR_VAL, CTF_VAL_ABS)  //value
      END_GROUP_TOKEN()
    END_GROUP_TOKEN()    
  END_GROUP_TOKEN() //effect

  BEGIN_GROUP_TOKEN("leds|l") //num leds
    VALUE_IS_TOKEN("get|g|", EEMC_GET_NUMLEDS)
    VALUE_IS_PAIR("set|s", EEMC_NUMLEDS, CTF_VAL_ABS) //sets     
  END_GROUP_TOKEN() //leds
  
//WIFI
#if defined(ESP8266) || defined(ESP32)
  BEGIN_GROUP_TOKEN("wifi") 
    VALUE_IS_TOKEN("status|", EEMC_WIFI_STATUS)                   //WiFi status

    BEGIN_GROUP_TOKEN("ap")                                       //AP control
      VALUE_IS_TOKEN("on", EEMC_WIFI_AP_ON)                       //AP off               
      VALUE_IS_TOKEN("off", EEMC_WIFI_AP_OFF)                     //AP off         
    END_GROUP_TOKEN()

    VALUE_IS_TOKEN("scan|s", EEMC_WIFI_SCAN)                      //Scan networks  

    BEGIN_OBJECT("connect|c", WIFI_CONNECT, EEMC_WIFI_CONNECT)   //Connect
      DATA_MEMBER("ssid|s", ssid)                                 //SSID
      DATA_MEMBER("pwd|p", pwd, "")                               //Password
      DATA_MEMBER_AS_IP("gateway|gw", gateway, 0)                 //Gateway
      DATA_MEMBER_AS_IP("subnetmask|sm", subnetMask, 0)           //Subnet mask
      DATA_MEMBER_AS_IP("dns1", dns1, 0)                          //DNS 1
      DATA_MEMBER_AS_IP("dns2", dns2, 0)                          //DNS 2
    END_OBJECT()
    VALUE_IS_TOKEN("disconnect|d", EEMC_WIFI_DISCONNECT)          //Disconnect

    BEGIN_GROUP_TOKEN("config|cfg")
      VALUE_IS_TOKEN("get|", EEMC_WIFI_CFG_GET)                   //Get config saved after last succesfull connect
      VALUE_IS_TOKEN("clear", EEMC_WIFI_CFG_CLEAR)                //Clear config for debugging purposes
    END_GROUP_TOKEN()
  
  END_GROUP_TOKEN()
#endif

END_PARSE_ROUTINE()