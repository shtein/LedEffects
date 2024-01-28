#include "LedEffects.h"

#include <Controls.h>
#include <EEPROMCfg.h>
#include <CtrlWiFi.h>


#include "Effect.h"
#include "EffectsAll.h"
#include "EffectEngine.h"

#ifdef NTF_ENABLED  
////////////////////////////////////////////
//Notifictaions serialization
struct EEResp_ModeList{
  uint8_t     numModes;
};

void putNtfObject(NtfBase &resp, const EEResp_ModeList &data){
   resp.put_F(F("modeCount"), data.numModes);
}


void putNtfObject(NtfBase &resp, const EFFECT_DESCRIPTION &data){
   resp.put_F(F("effectId"), data.effectId);
   resp.put_F(F("effectName"), data.effectName);
}

struct EEResp_EffectList{};

void putNtfObject(NtfBase &resp, const EEResp_EffectList &data){
  resp.beginArray_F(F("Effects"));

  EFFECT_DESCRIPTION ed;
  for(size_t i = 0; i < el_Total; i++){
    getEffect(i, ed);
    if(ed.effect){
      resp.put_F(F("Effect"), ed);
    }
  }

  resp.endArray();
}


#endif


////////////////////////////////////////////
// EffectEngine

EffectEngine::EffectEngine(){
  memset(&_cfgEngine, 0, sizeof(_cfgEngine));
  memset(&_cfgMode, 0, sizeof(_cfgMode));
  _curEffect  = NULL;

  //Leds
  _numLeds  = MAX_LEDS;

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

  //Check mode startup flags
  if(_flags & EFF_RANDOM_START_MODE){
    _cfgEngine.modeNum  = random8(_cfgEngine.numModes); //random mode
  }

  //Read mode config
  getModeConfig(_cfgEngine.modeNum, _cfgMode);

  //Check effect startup flags
  if(_flags & EFF_RANDOM_START_EFFECT){
    _cfgMode.effectNum = random8(_cfgMode.numEffects); //random effect
  }

  //Set mode
  setEffect(_cfgMode.effectNum);

  //Light LEDs
  FastLED.show();
}

void EffectEngine::setMode(uint8_t mode){  
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
  if(_cfgEngine.numModes){
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
      getEffect(cfgEffect.effect, ed);
     _curEffect = ed.effect;

      //Configgure it
      if(_curEffect != NULL){
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

void EffectEngine::onModeChange(struct CtrlQueueData &data){ 
  //Get new mode value    
  uint8_t mode = (uint8_t)data.translate(_cfgEngine.modeNum, 0, _cfgEngine.numModes - 1);
  
  //Do nothing if did not change
  if(_cfgEngine.modeNum != mode){    

    //Change mode
    setMode(mode);  
  }
}


void EffectEngine::onEffectChange(struct CtrlQueueData &data){

  //Get new effect value  
  uint8_t effectNum = (uint8_t)data.translate(_cfgMode.effectNum, 0, _cfgMode.numEffects - 1);
  
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
    case EEMC_GET_EFFECT_LIST:
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
      { itm.ntf.put(EECmdResponse<EEResp_ModeEffect> {itm.cmd, {_cfgEngine.modeNum, _cfgMode.effectNum}}); }
    break;

    case EEMC_NUMLEDS:    
    case EEMC_GET_NUMLEDS:       
      { itm.ntf.put(EECmdResponse<EEResp_NumLeds>{ itm.cmd, { MAX_LEDS, (uint16_t)_numLeds }}); }
    break;
      
    case EEMC_GET_MODE_LIST:               
      { itm.ntf.put(EECmdResponse<EEResp_ModeList>{itm.cmd, {_cfgEngine.numModes}}); }
    break;       

    case EEMC_GET_EFFECT_LIST:
      { itm.ntf.put(EECmdResponse<EEResp_EffectList>{itm.cmd}); }
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
      if(_curEffect){
         processed = _curEffect->onCmd(itm);
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
  if(_curEffect != NULL){

    //Is it time to process ?
     if(_millis <= millis()){

        //Proceed
        _curEffect->proceed(_leds, _numLeds);
        
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
    writeConfig();
    configCurEffect(false);  
  }
}

////////////////////////
// Read/Write configuration

void EffectEngine::writeConfig(){ 

}

void EffectEngine::configCurEffect(bool read){
}


void EffectEngine::preSaveConfig(){
  _millisToSaveCfg = millis() + SAVE_CONFIG_TIMEOUT;
}

/////////////////////////////
// parseSerialInput
// Nothing fancy, enter from terminal if serial inteface is available
BEGIN_PARSE_ROUTINE(parseCommandInput)  
  VALUE_IS_TOKEN("effectlist|el", EEMC_GET_EFFECT_LIST)
  
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
        VALUE_IS_NUMBER(EEMC_EFFECT, CTF_VAL_ABS)               //specific effect
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