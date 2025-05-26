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

#define IR_CTRL VAR_NAME(rmt)

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
#endif //NTF_ENABLED

#ifdef SERIAL_ENABLED
  #define _SERIAL_INPUT() \
    static SerialInput inSer; \
    static CtrlItemSerial<parseCommandInput> ctrlSer(&inSer); \
    cp.addControl(&ctrlSer); \
    _NTF_ADD(&ctrlSer);
#else
  #ifdef NTF_ENABLED
    #define _SERIAL_INPUT() \
      static NtfSerial ntfSer; \
      _NTF_ADD(&ntfSer);
  #else
    #define _SERIAL_INPUT()
  #endif //NTF_ENABLED
#endif //SERIAL_ENABLED

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


//This macro needs to defined at the begining of config 
//#define SETUP_NAME xxx

//Effect config
#define BEGIN_EFFECTS(flg)\
void _CFG_SETUP() { \
  DBG_INIT(); \
  DBG_OUTLN("Config setup - " SETUP_NAME);  \
  prepareEngineConfig(flg); 

#define END_EFFECTS() \
} \
void _CFG_LOOP(){}

#define BEGIN_MODE(modeName) \
  if(addModeConfig(PSTR(modeName))) {
  
#define END_MODE() \
  }

#define _ADD_EFFECT(effect, flags, ...) \
  addEffectConfig(effect, EFFECT_DATA(flags, ##__VA_ARGS__));
  
#define ADD_EFFECT(effect, ...) _ADD_EFFECT(effect, 0, ##__VA_ARGS__)
#define ADD_EFFECT_KALEYDOSCOPE(effect, ...) _ADD_EFFECT(effect, ECF_KALEYDOSCOPE, ##__VA_ARGS__)
#define ADD_EFFECT_SOUND(effect, ...) _ADD_EFFECT(effect, ECF_SOUND, ##__VA_ARGS__)

//Effect Engine
#define BEGIN_EFFECT_ENGINE() \
static EffectEngine ee; \
static CtrlPanel cp; \
_NTF_INIT(); \
\
void _ENGINE_SETUP(){ \
  DBG_INIT(); \
  DBG_OUTLN("Engine started - " SETUP_NAME );  

#define END_EFFECT_ENGINE() \
  ee.init(); \
} \
\
void _ENGINE_LOOP() \
{ \
  CtrlQueueItem itm; \
  cp.loop(itm); \
  ee.loop(itm, ntf); \
}

#define BEGIN_LEDS() \

#define ADD_STRIP(Type, ...) \
  FastLED.addLeds<Type, __VA_ARGS__ >(ee.getLeds(), MAX_LEDS).setCorrection( TypicalLEDStrip );  
  
#define END_LEDS() 


///////////////////////////////////////
//Sound capture
#define INIT_SOUND_CAPTURE(className, ...) \
  static className snd(__VA_ARGS__); \
  Effect::initSoundCapture(&snd); 

///////////////////////////////////////
//Control map
#define BEGIN_CONTROL_MAP() \
  _SERIAL_INPUT()

#define END_CONTROL_MAP()


////////////////
// Push button
#define BEGIN_PUSH_BUTTON(pin) \
  { \
    static PushButton pb(pin);  \
    static CtrlItemPb pbCtrl([](uint8_t ctrl, CtrlQueueData &data)->uint8_t{ \
      switch (ctrl){
   

#define END_PUSH_BUTTON() \
        default: \
        break; \
      } \
      return EEMC_NONE;\
    }, &pb); \
    cp.addControl(&pbCtrl); \
  }

#define PUSH_BUTTON_TO_CMD(ctrl, cmd, ...) \
    case ctrl: \
      _CQD_SET_DATA(data, cmd, ##__VA_ARGS__); \
    break;

#define PUSH_BUTTON_SA_TO_CMD(pin, ctrl, cmd, ...) \
  BEGIN_PUSH_BUTTON(pin); \
  PUSH_BUTTON_TO_CMD(ctrl, cmd, ##__VA_ARGS__); \
  END_PUSH_BUTTON()

//////////////////////
// Two position switch
#define SW2POS_TO_CMD(pin, cmd) \
  static Switch2Pos SW2POS_NAME(pin); \
  static CtrlSwicth2Pos SW2POS_CTRL(cmd, &SW2POS_NAME); \
  cp.addControl(&SW2POS_CTRL);

//////////////////////
// Potentiometer 
#define POT_TO_CMD(pin, cmd, ...) \
  static AnalogInput POT_NAME(pin); \
  static CtrlItemPtmtr POT_CTRL(cmd, &POT_NAME, ##__VA_ARGS__); \
  cp.addControl(&POT_CTRL);

////////////////////
// Rotery enconder
#define ROTENC_TO_CMD(pinData, cmd, pinClock) \
  static RotaryEncoder ROT_NAME(pinData, pinClock); \
  static CtrlItemRotEnc ROT_CTRL(cmd, &ROT_NAME); \
  cp.addControl(&ROT_CTRL);

#ifdef USE_IR_REMOTE
////////////////////
// IR Remote
#define BEGIN_REMOTE(pin) \
  { \
    static IRRemoteRecv ir(pin); \
    static CtrlItemIR irCtrl([](unsigned long btn, CtrlQueueData &data)->uint8_t{ \
      switch (btn){

#define END_REMOTE() \
        default: \
        break; \
      } \
      return EEMC_NONE;\
    }, &ir); \
    cp.addControl(&irCtrl); \
  }

#define RMT_BUTTON_TO_CMD(btn, cmd, ...) \
  case btn: \
    _CQD_SET_DATA(data, cmd, ##__VA_ARGS__); \
  break;
  

#define RMT_BUTTON_PAIR_PREV_NEXT_TO_CMD(btn1, btn2, cmd) \
  RMT_BUTTON_TO_CMD(btn1, cmd, 0, CTF_VAL_NEXT) \
  RMT_BUTTON_TO_CMD(btn2, cmd, 0, CTF_VAL_PREV)

#define RMT_BUTTON_PAIR_DELTA_TO_CMD(btn1, btn2, cmd, value) \
  RMT_BUTTON_TO_CMD(btn1, cmd, value, CTF_VAL_DELTA) \
  RMT_BUTTON_TO_CMD(btn2, cmd, -value, CTF_VAL_DELTA)


#endif //USE_IR_REMOTE


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
