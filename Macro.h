#ifndef __MACRO_H
#define __MACRO_H

///////////////////////////////////////
//Macros for temp variable names
#define TOKEN_CONCAT(x, y) x ## y
#define TOKEN_CONCAT2(x, y) TOKEN_CONCAT(x, y)
#define VAR_NAME(prefix) TOKEN_CONCAT2(prefix, __LINE__)

#define EFFECT_ARR_NAME VAR_NAME(efcts)

#define EFFECT_NAME VAR_NAME(efct)

#define BTN VAR_NAME(btn) //Temp button variable
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


//Effect Engine
#define BEGIN_EFFECT_ENGINE() \
  _CM EffectEngine ee; \
  _CM CtrlPanel cp; \

#define END_EFFECT_ENGINE()  \
  _CM CtrlQueueItem itm; \
  for( ;; ){\
     cp.loop(itm); \
     ee.loop(itm); \
  }

#define BEGIN_EFFECTS()

#define END_EFFECTS()

#define BEGIN_MODE(modeName, maxEffects) \
  _CM Effect *modeName[maxEffects]; \
  ee.addMode(modeName);

 #define END_MODE()

#define BLACK_MODE() \
  ee.addMode(NULL);
 

#define ADD_EFFECT(ClassEffect) \
  _CM ClassEffect EFFECT_NAME; \
  ee.addEffect(&EFFECT_NAME);

#define ADD_STATIC_COLOR(hsv) \
  _CM EffectStatic EFFECT_NAME(hsv); \
  ee.addEffect(&EFFECT_NAME);



#define BEGIN_LEDS(xmaxleds) \
   _CM CRGB leds[xmaxleds];
    
#define ADD_STRIP(Type, ...) \
  FastLED.addLeds<Type, __VA_ARGS__ >(leds, sizeof(leds) / sizeof(leds[0])).setCorrection( TypicalLEDStrip );  

#define END_LEDS() \
    ee.init(leds, sizeof(leds) / sizeof(leds[0]));


///////////////////////////////////////
//Control map
#define BEGIN_CONTROL_MAP() \
  _CM BaseInput *ai = NULL;

#define END_CONTROL_MAP() \
  ai = NULL;

#define BEGIN_PUSH_BUTTON(pin) \
  _CM PushButton BTN(pin); \
  ai = &BTN;  

#define END_PUSH_BUTTON() \
  ai = NULL;  

#define PUSH_BUTTON_TO_CMD(cmd, ...) \
  _CM CtrlItemPb BTN_CTRL(cmd, (PushButton *)ai, __VA_ARGS__); \
  cp.addControl(&BTN_CTRL);


#define PUSH_BUTTON_SA_TO_CMD(cmd, pin) \
  BEGIN_PUSH_BUTTON(pin); \
  PUSH_BUTTON_TO_CMD(cmd, PB_CONTROL_CLICK); \
  END_PUSH_BUTTON()


#define POT_TO_CMD(cmd, pin) \
  _CM AnalogInput POT_NAME(pin); \
  _CM CtrlItemPtmtr POT_CTRL(cmd, &POT_NAME); \
  cp.addControl(&POT_CTRL);

#define MIC_TO_CMD(cmd, pin) \
  _CM AnalogInput MIC_NAME(pin); \
  _CM CtrlItemPtmtr MIC_CTRL(cmd, &MIC_NAME); \
  cp.addControl(&MIC_CTRL);


#define ROTENC_TO_CMD(cmd, pinData, pinClock) \
  _CM RotaryEncoder ROT_NAME(pinData, pinClock); \
  _CM CtrlItemRotEnc ROT_CTRL(cmd, &ROT_NAME); \
  cp.addControl(&ROT_CTRL);

#define BEGIN_REMOTE(pin) \
  _CM IRRemoteRecv IR_NAME(pin); \
  ai = &IR_NAME;
  
#define END_REMOTE() \
  ai = NULL;

#define RMT_BUTTON_TO_CMD(cmd, code) \
  _CM CtrlItemIRBtn IR_CTRL(cmd, (IRRemoteRecv *)ai, code); \
  cp.addControl(&IR_CTRL);

#define RMT_BUTTON_PAIR_TO_CMD(cmd, code1, code2, repeat) \
  _CM CtrlItemIRBtn IR_CTRL_UP(cmd, (IRRemoteRecv *)ai, code1, true, repeat); \
  cp.addControl(&IR_CTRL_UP); \
  _CM CtrlItemIRBtn IR_CTRL_DOWN(cmd, (IRRemoteRecv *)ai, code2, false, repeat); \
  cp.addControl(&IR_CTRL_DOWN);


#endif //__MACRO_H  

