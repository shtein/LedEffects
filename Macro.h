#ifndef __MACRO_H
#define __MACRO_H

///////////////////////////////////////
//Macros for temp variable names
#define TOKEN_CONCAT(x, y) x ## y
#define TOKEN_CONCAT2(x, y) TOKEN_CONCAT(x, y)
#define VAR_NAME(prefix) TOKEN_CONCAT2(prefix, __LINE__)

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


//Effect Engine
#define BEGIN_EFFECT_ENGINE() \
  _CM EffectEngine ee; \
  _CM EffectControlPanel cp; \

#define END_EFFECT_ENGINE()  \
  _CM CtrlQueueItem itm; \
  for( ;; ){\
     cp.loop(itm); \
     ee.loop(itm); \
  }

#define BEGIN_EFFECTS()

#define END_EFFECTS()

#define ADD_EFFECT(ClassEffect) \
  _CM ClassEffect e##ClassEffect; \
  ee.addEffect(&e##ClassEffect);


#define BEGIN_LEDS(xmaxleds, xmode) \
   _CM CRGB leds[xmaxleds]; \
    uint8_t mode = xmode;
    
#define ADD_STRIP(Type, ...) \
  FastLED.addLeds<Type, __VA_ARGS__ >(leds, sizeof(leds) / sizeof(leds[0])).setCorrection( TypicalLEDStrip );  

#define END_LEDS() \
    ee.init(leds, sizeof(leds) / sizeof(leds[0]), mode);


///////////////////////////////////////
//Control map
#define BEGIN_CONTROL_MAP() \
  _CM AnalogInput *ai = NULL;

#define END_CONTROL_MAP() \
  ai = NULL;

#define BEGIN_PUSH_BUTTON(pin) \
  _CM PushButton BTN(pin); \
  ai = &BTN;  

#define END_PUSH_BUTTON() \
  ai = NULL;  

#define PUSH_BUTTON_TO_CMD(cmd, ...) \
  _CM EffectControlPb BTN_CTRL(cmd, (PushButton *)ai, __VA_ARGS__); \
  cp.addControl(&BTN_CTRL);


#define PUSH_BUTTON_SA_TO_CMD(cmd, pin) \
  BEGIN_PUSH_BUTTON(pin); \
  PUSH_BUTTON_TO_CMD(cmd, PB_CONTROL_CLICK); \
  END_PUSH_BUTTON()


#define POT_TO_CMD(cmd, pin) \
  _CM Potentiometer POT_NAME(pin); \
  _CM EffectControlPtmtr POT_CTRL(cmd, &POT_NAME); \
  cp.addControl(&POT_CTRL);

#define ROTENC_TO_CMD(cmd, pinData, pinClock) \
  _CM RotaryEncoder ROT_NAME(pinData, pinClock); \
  _CM EffectControlRotEnc ROT_CTRL(cmd, &ROT_NAME); \
  cp.addControl(&ROT_CTRL);

#define BEGIN_REMOTE(pin) \
  _CM IRRemoteRecv IR_NAME(pin); \
  ai = &IR_NAME;
  
#define END_REMOTE() \
  ai = NULL;

#define RMT_BUTTON_TO_CMD(cmd, code) \
  _CM EffectControlIRBtn IR_CTRL(cmd, (IRRemoteRecv *)ai, code); \
  cp.addControl(&IR_CTRL);

#define RMT_BUTTON_PAIR_TO_CMD(cmd, code1, code2, repeat) \
  _CM EffectControlIRBtn IR_CTRL_UP(cmd, (IRRemoteRecv *)ai, code1, true, repeat); \
  cp.addControl(&IR_CTRL_UP); \
  _CM EffectControlIRBtn IR_CTRL_DOWN(cmd, (IRRemoteRecv *)ai, code2, false, repeat); \
  cp.addControl(&IR_CTRL_DOWN);


#endif //__MACRO_H  

