#ifndef __MACRO_H
#define __MACRO_H

#include <utils.h>

///////////////////////////////////////
//Macros for temp variable names
#define VAR_NAME(prefix) TOKEN_CONCAT(prefix, __LINE__)

#define MODE_NAME VAR_NAME(md)
#define EFFECT_NAME VAR_NAME(efct)

#define BTN VAR_NAME(btn) 
#define BTN_CTRL VAR_NAME(ecbtn)

#define IR_NAME VAR_NAME(rmt)
#define IR_CTRL VAR_NAME(ectbtn)
#define IR_CTRL_UP VAR_NAME(ecbtnUp)
#define IR_CTRL_DOWN VAR_NAME(ecbtnDown)

#define POT_NAME VAR_NAME(pot)
#define POT_CTRL VAR_NAME(ecpot)

#define ROT_NAME VAR_NAME(rot)
#define ROT_CTRL VAR_NAME(ecrot)

#define MIC_NAME VAR_NAME(mic)
#define MIC_CTRL VAR_NAME(ecmic)

#define SW2POS_NAME VAR_NAME(sw2p)
#define SW2POS_CTRL VAR_NAME(ecsw2p)
  
//Notifications
#define _NTF_INIT() static NtfSet ntf;      
#ifdef NTF_ENABLED
  #define _NTF_ADD(a) ntf.addNtf(a); 
#else  
  #define _NTF_ADD(a)
#endif

#ifdef WRITE_CONFIG_ONLY
  #define _CFG_SETUP     setup
  #define _CFG_LOOP      loop
  #define _ENGINE_SETUP _setup
  #define _ENGINE_LOOP  _loop
#else
  #define _CFG_SETUP    _setup
  #define _CFG_LOOP     _loop
  #define _ENGINE_SETUP  setup
  #define _ENGINE_LOOP   loop
#endif

//Effect config
#define BEGIN_EFFECTS(flg)\
void _CFG_SETUP() { \
  DBG_INIT(); \
  DBG_OUTLN("Config setup");  \
  prepareEngineConfig(flg); 

#define END_EFFECTS() \
} \
void _CFG_LOOP(){}

#define BEGIN_MODE(modeName) \
  if(addModeConfig(PSTR(modeName))) {

  
#define END_MODE()\        
  }


#define ADD_EFFECT(effect, ...) \
  addEffectConfig(effect, ##__VA_ARGS__);


//Effect Engine
#define BEGIN_EFFECT_ENGINE(name) \
static EffectEngine ee; \
static CtrlPanel cp; \
_NTF_INIT(); \
\
void _ENGINE_SETUP(){ \
  DBG_INIT(); \
  DBG_OUTLN("Led effect started - " name);  


#define END_EFFECT_ENGINE() \
  ee.init(); \
} \
\
void _ENGINE_LOOP() \
{ \
  CtrlQueueItem itm; \
  for( ;; ){ \
    cp.loop(itm); \
    ee.loop(itm, ntf); \
  } \
}

#define BEGIN_LEDS() \

#define ADD_STRIP(Type, ...) \
  FastLED.addLeds<Type, __VA_ARGS__ >(ee.getLeds(), MAX_LEDS).setCorrection( TypicalLEDStrip );  
  
#define END_LEDS() 


///////////////////////////////////////
//Sound capture
#define INIT_SOUND_CAPTURE(className, ...) \
  static className snd(__VA_ARGS__); \
  EffectSound::initSoundCapture(&snd); 

///////////////////////////////////////
//Control map
#define BEGIN_CONTROL_MAP() \
  BaseInput *ai = NULL;

#define END_CONTROL_MAP() \
  ai = NULL;

#define BEGIN_PUSH_BUTTON(pin) \
  static PushButton BTN(pin); \
  ai = &BTN;  

#define END_PUSH_BUTTON() \
  ai = NULL;  

#define PUSH_BUTTON_TO_CMD(cmd, ...) \
  static CtrlItemPb<__VA_ARGS__> BTN_CTRL(cmd, (PushButton *)ai); \
  cp.addControl(&BTN_CTRL);


#define PUSH_BUTTON_SA_TO_CMD(cmd, pin, ...) \
  BEGIN_PUSH_BUTTON(pin); \
  PUSH_BUTTON_TO_CMD(cmd, PB_CONTROL_CLICK, ##__VA_ARGS__); \
  END_PUSH_BUTTON()

#define SW2POS_TO_CMD(cmd, pin) \
  static Switch2Pos SW2POS_NAME(pin); \
  static CtrlSwicth2Pos SW2POS_CTRL(cmd, &SW2POS_NAME); \
  cp.addControl(&SW2POS_CTRL);


#define POT_TO_CMD(cmd, pin, ...) \
  static AnalogInput POT_NAME(pin); \
  static CtrlItemPtmtr<__VA_ARGS__> POT_CTRL(cmd, &POT_NAME); \
  cp.addControl(&POT_CTRL);

#define ROTENC_TO_CMD(cmd, pinData, pinClock) \
  static RotaryEncoder ROT_NAME(pinData, pinClock); \
  static CtrlItemRotEnc ROT_CTRL(cmd, &ROT_NAME); \
  cp.addControl(&ROT_CTRL);

#ifdef USE_IR_REMOTE

#define BEGIN_REMOTE(pin) \
  static IRRemoteRecv IR_NAME(pin); \
  ai = &IR_NAME;
  
#define END_REMOTE() \
  ai = NULL;

#define RMT_BUTTON_TO_CMD(cmd, code) \
  static CtrlItemIRBtn<code> IR_CTRL(cmd, (IRRemoteRecv *)ai); \
  cp.addControl(&IR_CTRL);

#define RMT_BUTTON_PAIR_TO_CMD(cmd, code1, code2, repeat) \
  static CtrlItemIRBtn<code1, true, repeat> IR_CTRL_UP(cmd, (IRRemoteRecv *)ai); \
  cp.addControl(&IR_CTRL_UP); \
  static CtrlItemIRBtn<code2, false, repeat> IR_CTRL_DOWN(cmd, (IRRemoteRecv *)ai); \
  cp.addControl(&IR_CTRL_DOWN);

#endif //USE_IR_REMOTE

#define SERIAL_INPUT() \
  static SerialInput inSer; \
  static CtrlItemSerial<parseCommandInput> ctrlSer(&inSer); \
  cp.addControl(&ctrlSer); \
  _NTF_ADD(&ctrlSer);


//Wifi and Web
#if defined(ESP8266) || defined(ESP32)

#define WEB_INPUT(port) \
  static WebApiInput inWeb; \
  static CtrlItemWebApi<parseCommandInput> ctrlWeb(&inWeb); \
  cp.addControl(&ctrlWeb); \
  _NTF_ADD(&ctrlWeb); \
  initWebServer(port);

#define WIFI_INPUT() \
  static CtrlWifiStatus ctrlWifi(EEMC_WIFI_STATUS_CHANGE); \
  cp.addControl(&ctrlWifi);
  
#endif

#endif //__MACRO_H  
