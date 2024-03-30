#include "LedEffects.h"

#include <Controls.h>
#include <EEPROMCfg.h>
#include <CtrlWiFi.h>


#include "Effect.h"
#include "EffectsAll.h"
#include "EffectEngine.h"

#ifdef NTF_ENABLED  

//String resources to be stored in flash memory
DEFINE_STR_PROGMEM(rs_VerCfg,       "verCfg")
DEFINE_STR_PROGMEM(rs_VerEng,       "verEng")
DEFINE_STR_PROGMEM(rs_NumLeds,      "numLeds")
DEFINE_STR_PROGMEM(rs_MaxLeds,      "maxLeds")
DEFINE_STR_PROGMEM(rs_Index,        "idx")
DEFINE_STR_PROGMEM(rs_ModeCount,    "modeCnt")
DEFINE_STR_PROGMEM(rs_Modes,        "modes")
DEFINE_STR_PROGMEM(rs_Mode,         "mode")
DEFINE_STR_PROGMEM(rs_Name,         "name")
DEFINE_STR_PROGMEM(rs_EffectCount,  "effectCnt")
DEFINE_STR_PROGMEM(rs_Effects,      "effects")
DEFINE_STR_PROGMEM(rs_Effect,       "effect")
DEFINE_STR_PROGMEM(rs_Id,           "id")
DEFINE_STR_PROGMEM(rs_Speed,        "speed")
DEFINE_STR_PROGMEM(rs_Hue,          "hue")
DEFINE_STR_PROGMEM(rs_Sat,          "sat")
DEFINE_STR_PROGMEM(rs_Val,          "val")
DEFINE_STR_PROGMEM(rs_HSV,          "hsv")
DEFINE_STR_PROGMEM(rs_Transforms,   "transforms")
DEFINE_STR_PROGMEM(rs_Transform,    "transform")
DEFINE_STR_PROGMEM(rs_Flags,        "flags")
DEFINE_STR_PROGMEM(rs_Dsc,          "dsc")
DEFINE_STR_PROGMEM(rs_Cfg,          "cfg")
DEFINE_STR_PROGMEM(rs_Kaleidoscope, "kldsc")

////////////////////////////////////////////
//Notifictaions serialization

void putNtfObject(NtfBase &resp, const EFFECT_DESCRIPTION &data){
  resp.put_F(rs_Id, data.effectId);
  resp.put_FP(rs_Name, data.effectName);
  resp.put_F(rs_Flags, data.flags);
}

void putNtfObject(NtfBase &resp, const TRANSFORM_DESCRIPTION &data){
  resp.put_F(rs_Id, data.transformId); 
  resp.put_FP(rs_Name,  data.transformName);
  
}

void putNtfObject(NtfBase &resp, const EFFECT_MODE_CONFIG &data){
  resp.put_F(rs_Name, data.name);  
  resp.put_F(rs_EffectCount, data.numEffects);
  resp.put_F(rs_Effect, data.effectNum);
}


void putNtfObject(NtfBase &resp,  const EFFECT_DATA &data){
  //Do nothing if no data

  resp.put_F(rs_Flags, data.flags);
  
  if(data.flags & ECF_HSV) {
    resp.put_F(rs_HSV, data.hsv);
  }
  
  if(data.flags & ECF_RGB){    
  }

  if(data.flags & ECF_TRANSFORM){
    TRANSFORM_DESCRIPTION td;
    if(getPalTransform(EFFECT_PARAM_TRANSFORM(data), td)){
      resp.put_F(rs_Transform, td);
    }      
  }
  
  if(data.flags & ECF_KALEYDOSCOPE){
    resp.put_F(rs_Kaleidoscope, true);
  }
} 

void putNtfObject(NtfBase &resp, const EFFECT_CONFIG &data){  
  resp.put_F(rs_Speed, data.speedDelay);    
  resp.put_F(rs_Data, data.data);    
}

struct EEResp_Effect{
  uint8_t            effectNum;
  EFFECT_CONFIG      cfg;
};

void putNtfObject(NtfBase &resp, const EEResp_Effect &data){
  resp.put_F(rs_Index, data.effectNum);

  EFFECT_DESCRIPTION ed;
  getEffect(data.cfg.effectId, ed);
  resp.put_F(rs_Dsc, ed); 

  resp.put_F(rs_Cfg, data.cfg); 
}

struct EEResp_Mode{
  uint8_t             modeNum;
  EFFECT_MODE_CONFIG  cfgMode;
};

void putNtfObject(NtfBase &resp, const EEResp_Mode &data){
  resp.put_F(rs_Index, data.modeNum);
  resp.put_F(rs_Cfg, data.cfgMode);
  

  resp.beginArray_F(rs_Effects); 

  for(size_t i = 0; i < data.cfgMode.numEffects; i++){      
    //Effect        
    EEResp_Effect respEffect;
    respEffect.effectNum = i;
    if(getEffectConfig(data.modeNum, i, respEffect.cfg)){
      resp.put_F(rs_Effect, respEffect);
    }      
  }

  resp.endArray();
}

struct EEResp_ModeList{
  uint8_t     numModes;
};

void putNtfObject(NtfBase &resp, const EEResp_ModeList &data){
  resp.put_F(rs_ModeCount, data.numModes);
  
  resp.beginArray_F(rs_Modes);

  EEResp_Mode mode;
  for(size_t i = 0; i < data.numModes; i++){
    mode.modeNum = i;    
    if(getModeConfig(i, mode.cfgMode)){
      resp.put_F(rs_Mode, mode);
    }
  }
  
  resp.endArray();
}


struct EEResp_EffectList{};

void putNtfObject(NtfBase &resp, const EEResp_EffectList &data){
  resp.beginArray_F(rs_Effects);

  EFFECT_DESCRIPTION ed;
  for(size_t i = 0; i < el_Total; i++){    
    if(getEffect(i, ed)){
      resp.put_F(rs_Effect, ed);
    }
  }

  resp.endArray();
}

struct EEResp_TransformList{};

void putNtfObject(NtfBase &resp, const EEResp_TransformList &data){
  resp.beginArray_F(rs_Transforms);

  TRANSFORM_DESCRIPTION td;
  for(size_t i = 0; i < tl_Total; i++){    
    if(getPalTransform(i, td)){
      resp.put_F(rs_Transform, td);
    }
  }

  resp.endArray();
}


struct EEResp_NumLeds{  
  uint16_t maxLeds;
  uint16_t numLeds;
};

 void putNtfObject(NtfBase &resp, const EEResp_NumLeds &data){
  resp.put_F(rs_MaxLeds, data.maxLeds);
  resp.put_F(rs_NumLeds, data.numLeds);
}

struct EEResp_Version{  
};

void putNtfObject(NtfBase &resp, const EEResp_Version &data){
  EFFECT_ENGINE_VERSION ver;
  getConfigVersion(ver);

  char verStr[8];

  sprintf(verStr, "%d.%d", ver.verHigh, ver.verLow);
  resp.put_F(rs_VerCfg, verStr);

  sprintf(verStr, "%d.%d", EFFECT_ENGINE_VERSION_HIGH,  EFFECT_ENGINE_VERSION_LOW);
  resp.put_F(rs_VerEng, verStr);
}

#endif


////////////////////////////////////////////
// EffectEngine

EffectEngine::EffectEngine(){
  memset(&_cfgEngine, 0, sizeof(_cfgEngine));
  memset(&_cfgMode, 0, sizeof(_cfgMode));
  _curEffect  = NULL;

  //Timing
  _millis          = 0;
  _millisToSaveCfg = 0;  
}

CRGB *EffectEngine::getLeds() const{
  return (CRGB *)_leds;
}

EffectEngine::~EffectEngine(){
}


void EffectEngine::init() {  

  //FastLed randmization seeding Random number initialization  
  random16_set_seed(analogRead(0)); 
 
  //Don't uncomment it if you don;t know what it is 
  //FastLED.setMaxPowerInVoltsAndMilliamps(5,1000);

  fill_solid(_leds, MAX_LEDS, CRGB::Black);

  //Read engine config
  getEngineConfig(_cfgEngine);

  if(_cfgEngine.numLeds == 0 || _cfgEngine.numLeds > MAX_LEDS){
    _cfgEngine.numLeds = MAX_LEDS;
  }  

  //Check mode startup flags
  if(_cfgEngine.flags & EFF_RANDOM_START_MODE){
    _cfgEngine.modeNum  = random8(_cfgEngine.numModes); //random mode
  }

  //Read mode config
  getModeConfig(_cfgEngine.modeNum, _cfgMode);

  //Check effect startup flags
  if(_cfgEngine.flags & EFF_RANDOM_START_EFFECT){
    _cfgMode.effectNum = random8(_cfgMode.numEffects); //random effect
  }

  //Set mode
  setEffect(_cfgMode.effectNum);

  //Light LEDs
  FastLED.show();
}

void EffectEngine::setMode(uint8_t mode){  
  //Save mode
  _cfgEngine.modeNum = mode;

  //Black the lights
  fill_solid(_leds, MAX_LEDS, CRGB::Black);

  //Read mode configuration
  getModeConfig(mode, _cfgMode);

  //Refresh effect
  setEffect(_cfgMode.effectNum);
}

void EffectEngine::setEffect(uint8_t effectNum){  
  //Set new effect number  
  _cfgMode.effectNum = effectNum;

 //Get effect object
  if(_cfgEngine.numModes == 0){
    _curEffect =  NULL; 
  }
  else if(_cfgMode.numEffects == 0){
    _curEffect =  NULL; 
  }
  else {
    //Get effect config
    EFFECT_CONFIG cfgEffect;
    if(getEffectConfig(_cfgEngine.modeNum, _cfgMode.effectNum, cfgEffect)){

      //Get effect object
      EFFECT_DESCRIPTION ed;    
      getEffect(cfgEffect.effectId, ed);
     _curEffect = ed.effect;
     
      //Configure it
      if(_curEffect != NULL){
        //This sequence is important
        //Config first
        _curEffect->setConfig(cfgEffect.data);          
        //Reset accoring to config settings
        _curEffect->reset();
        //Set Effect speed delay
        _curEffect->setSpeedDelay(cfgEffect.speedDelay);
        
      }
    }
  }
  
  //Process right away
  _millis = 0;
}

//////////////////////////////
//Command handlers

void EffectEngine::onModeChange(const struct CtrlQueueData &data){ 
  //Get new mode value    
  uint8_t mode = (uint8_t)data.translate(_cfgEngine.modeNum, 0, _cfgEngine.numModes - 1);
  
  //Do nothing if did not change
  if(_cfgEngine.modeNum != mode){    

    //Change mode
    setMode(mode);  
  }
}


void EffectEngine::onEffectChange(const struct CtrlQueueData &data){
  //Get new effect value  
  uint8_t effectNum = (uint8_t)data.translate(_cfgMode.effectNum, 0, _cfgMode.numEffects - 1);
  
  //Black the leds
  fill_solid(_leds, MAX_LEDS, CRGB::Black);
  
  //Change effect
  setEffect(effectNum);
}

void EffectEngine::onNumLedsChange(const struct CtrlQueueData &data){
  
  //Get new mode value
  uint16_t numLeds = data.translate(_cfgEngine.numLeds, 0, MAX_LEDS);

  //Do nothing if did not change
  if(_cfgEngine.numLeds == numLeds)
    return;

  //Save new value
  _cfgEngine.numLeds = numLeds;

 //Black the lights
  fill_solid(_leds + _cfgEngine.numLeds, MAX_LEDS - _cfgEngine.numLeds, CRGB::Black);
}


bool EffectEngine::onCmdEE(const struct CtrlQueueItem &itm, NtfSet &ntf){
  
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
    case EEMC_GET_EFFECT_LIST:
    case EEMC_GET_TRANSFORM_LIST:
    case EEMC_GET_VERSION:
    break;
#endif
    
    default: 
    return false;
  };

//Notifications
#ifdef NTF_ENABLED

  switch(itm.cmd){
    case EEMC_MODE:            
    case EEMC_GET_MODE:{         
      CmdResponse<EEResp_Mode> resp{itm.cmd, {_cfgEngine.modeNum, _cfgMode}};      
      ntf.put(resp); 
    }
    break;
    case EEMC_EFFECT:  
    case EEMC_GET_EFFECT:{
      CmdResponse<EEResp_Effect> resp{itm.cmd, {_cfgMode.effectNum }};
      getEffectConfig(_cfgEngine.modeNum, _cfgMode.effectNum, resp.data.cfg);
      if(_curEffect){
        resp.data.cfg.speedDelay = _curEffect->getSpeedDelay();
        _curEffect->getConfig(resp.data.cfg.data);        
      }
      ntf.put(resp);
    }
    break;

    case EEMC_NUMLEDS:    
    case EEMC_GET_NUMLEDS:       
      { ntf.put(CmdResponse<EEResp_NumLeds>{ itm.cmd, { MAX_LEDS, (uint16_t)_cfgEngine.numLeds }}); }
    break;
      
    case EEMC_GET_MODE_LIST:               
      { ntf.put(CmdResponse<EEResp_ModeList>{itm.cmd, {_cfgEngine.numModes }}); }
    break;       

    case EEMC_GET_EFFECT_LIST:
      { ntf.put(CmdResponse<EEResp_EffectList>{itm.cmd}); }
    break; 

    case EEMC_GET_TRANSFORM_LIST:
      { ntf.put(CmdResponse<EEResp_TransformList>{itm.cmd}); }
    break; 

    case EEMC_GET_VERSION:
      { ntf.put(CmdResponse<EEResp_Version>{itm.cmd}); }
    break;
  }
#endif

  return true;

}

bool EffectEngine::onCmd(const struct CtrlQueueItem &itm, NtfSet &ntf){
  
  //Process error
  if(itm.cmd == EEMC_ERROR) {
#ifdef NTF_ENABLED    
    ntf.put(CmdResponse<>{ itm.cmd, EEER_INVALID } );
#endif    
    return true;
  }
  
  //Initial
  bool processed = false;
  
  if(itm.cmd & EEMC_EE){ //Engine command
    processed = onCmdEE(itm, ntf);
  }  
#if defined(ESP8266) || defined(ESP32)
  else if(itm.cmd & EEMC_WIFI){ //Wifi command
    processed =  _wifi.onCmd(itm);
  }
#endif
  else{ //Effect command
      if(_curEffect){
         processed = _curEffect->onCmd(itm, ntf);
      }
  }
    
  //Report of not processed
#ifdef NTF_ENABLED    
  if(!processed){
    ntf.put(CmdResponse<>{ itm.cmd, EEER_UNHANDLED } );
  }
#endif

  return processed;
}

void EffectEngine::loop(const struct CtrlQueueItem &itm, NtfSet &ntf){
 
  if(itm.cmd != EEMC_NONE){
    onCmd(itm, ntf);      
  }
  
  //See if there were changes
  bool updateLeds = (itm.cmd & EEMC_LED) != EEMC_NONE;
  if(updateLeds){
    preSaveConfig(); 
  }

  //Current effect
  if(_curEffect != NULL){

    //Is it time to process ?
     if(_millis <= millis()){

        //Proceed
        _curEffect->draw(_leds, _cfgEngine.numLeds );
        
        
        //Remember when proceed next time
        _millis = millis() + _curEffect->getSpeedDelay();
        
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
    saveConfig();
  }
}

////////////////////////
// Read/Write configuration

void EffectEngine::saveConfig(){ 
  //Engine
  setEngineConfig(_cfgEngine);
  //Current mode
  setModeConfig(_cfgEngine.modeNum, _cfgMode);
  //Current effect
  if(_curEffect){
    EFFECT_CONFIG cfgEff;

    getEffectConfig(_cfgEngine.modeNum, _cfgMode.effectNum, cfgEff);
    cfgEff.speedDelay = _curEffect->getSpeedDelay();
    _curEffect->getConfig(cfgEff.data);

    setEffectConfig(_cfgEngine.modeNum, _cfgMode.effectNum, cfgEff);
  }
}

void EffectEngine::preSaveConfig(){
  _millisToSaveCfg = millis() + SAVE_CONFIG_TIMEOUT;
}

/////////////////////////////
// parseSerialInput
// Nothing fancy, enter from terminal if serial inteface is available
DEFINE_STR_PROGMEM(rs_CmdParam_Version,       "version|v")
DEFINE_STR_PROGMEM(rs_CmdParam_EffectList,    "effectlist|el")
DEFINE_STR_PROGMEM(rs_CmdParam_TransformList, "tflist|tl")
DEFINE_STR_PROGMEM(rs_CmdParam_ModeList,      "modelist|ml")
DEFINE_STR_PROGMEM(rs_CmdParam_Mode,          "mode|m")
DEFINE_STR_PROGMEM(rs_CmdParam_Get,           "get|g|")
DEFINE_STR_PROGMEM(rs_CmdParam_Set,           "set|s")
DEFINE_STR_PROGMEM(rs_CmdParam_Next,          "next|n|")
DEFINE_STR_PROGMEM(rs_CmdParam_Prev,          "prev|p")
DEFINE_STR_PROGMEM(rs_CmdParam_Effect,        "effect|e")
DEFINE_STR_PROGMEM(rs_CmdParam_Speed,         "speed|s")
DEFINE_STR_PROGMEM(rs_CmdParam_Color,         "color|c")
DEFINE_STR_PROGMEM(rs_CmdParam_Hue,           "hue|h")
DEFINE_STR_PROGMEM(rs_CmdParam_Sat,           "sat|s")
DEFINE_STR_PROGMEM(rs_CmdParam_Val,           "sat|v")
DEFINE_STR_PROGMEM(rs_CmdParam_Trans,         "transpal|t")
DEFINE_STR_PROGMEM(rs_CmdParam_Leds,          "leds|l")


BEGIN_PARSE_ROUTINE(parseCommandInput)  
  VALUE_IS_TOKEN(rs_CmdParam_Version, EEMC_GET_VERSION)
  VALUE_IS_TOKEN(rs_CmdParam_EffectList, EEMC_GET_EFFECT_LIST)
  VALUE_IS_TOKEN(rs_CmdParam_TransformList, EEMC_GET_TRANSFORM_LIST)
  VALUE_IS_TOKEN(rs_CmdParam_ModeList, EEMC_GET_MODE_LIST)
  
  BEGIN_GROUP_TOKEN(rs_CmdParam_Mode) //mode 
    VALUE_IS_TOKEN(rs_CmdParam_Get, EEMC_GET_MODE)
    BEGIN_GROUP_TOKEN(rs_CmdParam_Set) //sets     
      VALUE_IS_TOKEN(rs_CmdParam_Next, EEMC_MODE, 0, CTF_VAL_NEXT)  //next mode
      VALUE_IS_TOKEN(rs_CmdParam_Prev, EEMC_MODE, 0, CTF_VAL_PREV)  //prev mode
      VALUE_IS_NUMBER(EEMC_MODE, CTF_VAL_ABS)                       //specific mode
    END_GROUP_TOKEN()  

    
  END_GROUP_TOKEN() //mode

  BEGIN_GROUP_TOKEN(rs_CmdParam_Effect) //current effect
    VALUE_IS_TOKEN(rs_CmdParam_Get, EEMC_GET_EFFECT)
    BEGIN_GROUP_TOKEN(rs_CmdParam_Set) //sets     
      VALUE_IS_TOKEN(rs_CmdParam_Next, EEMC_EFFECT, 0, CTF_VAL_NEXT)  //next effect
      VALUE_IS_TOKEN(rs_CmdParam_Prev, EEMC_EFFECT, 0, CTF_VAL_PREV)  //prev effect
      VALUE_IS_NUMBER(EEMC_EFFECT, CTF_VAL_ABS)                       //specific effect
    END_GROUP_TOKEN()
    
    BEGIN_GROUP_TOKEN(rs_CmdParam_Speed)
      VALUE_IS_TOKEN(rs_CmdParam_Get, EEMC_GET_SPEED)
      VALUE_IS_PAIR(rs_CmdParam_Set, EEMC_SPEED, CTF_VAL_ABS)      
    END_GROUP_TOKEN()    
    
    BEGIN_GROUP_TOKEN(rs_CmdParam_Color) //set hsv 
      VALUE_IS_TOKEN(rs_CmdParam_Get, EEMC_GET_COLOR_HSV)
      BEGIN_GROUP_TOKEN(rs_CmdParam_Set) //set hsv    
        VALUE_IS_PAIR(rs_CmdParam_Hue, EEMC_COLOR_HUE, CTF_VAL_ABS)  //hue
        VALUE_IS_PAIR(rs_CmdParam_Sat, EEMC_COLOR_SAT, CTF_VAL_ABS)  //saturation
        VALUE_IS_PAIR(rs_CmdParam_Val, EEMC_COLOR_VAL, CTF_VAL_ABS)  //value
      END_GROUP_TOKEN()
    END_GROUP_TOKEN()  

    BEGIN_GROUP_TOKEN(rs_CmdParam_Trans) //set palette transform
      VALUE_IS_TOKEN(rs_CmdParam_Get, EEMC_GET_TRANSFORM)
      VALUE_IS_PAIR(rs_CmdParam_Set, EEMC_TRANSFORM, CTF_VAL_ABS)      
    END_GROUP_TOKEN()

  END_GROUP_TOKEN() //effect

  BEGIN_GROUP_TOKEN(rs_CmdParam_Leds) //num leds
    VALUE_IS_TOKEN(rs_CmdParam_Get, EEMC_GET_NUMLEDS)
    VALUE_IS_PAIR(rs_CmdParam_Set, EEMC_NUMLEDS, CTF_VAL_ABS) //sets     
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