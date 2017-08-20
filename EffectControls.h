#ifndef __EFFECTCONTROLS_H
#define __EFFECTCONTROLS_H

#include "EffectEngineCtx.h"
#include "AnalogInput.h"

//////////////////////////////////////////
// EffectControl - base class
class EffectControl{
  public:
    EffectControl(uint8_t cmd, AnalogInput *input);
   ~EffectControl();

    void loop(CtrlQueueItem &itm);
    
    AnalogInput *getInput() const;
    
  protected:
    virtual bool triggered() const = 0;
    virtual void getData(CtrlQueueData &data) = 0;

  protected:
    uint8_t       _cmd;     //Command
    AnalogInput  *_input;   //Analog input to retrieve control data
};

////////////////////////////////
// Push button control, reacts on either short click, long click or long push
class EffectControlPb: public EffectControl{
  public:
    EffectControlPb(uint8_t cmd, PushButton *input, uint8_t ctrl = PB_CONTROL_CLICK, uint8_t flag = CTF_VAL_NEXT);
   ~EffectControlPb();

  protected:
    bool triggered() const;
    void getData(CtrlQueueData &data);

  protected:
    uint8_t _flag:4;
    uint8_t _ctrl:4;
};

////////////////////////////////
// EffectControlPtmtr - analog input is Potentiometer
class EffectControlPtmtr: public EffectControl{
  public:
    EffectControlPtmtr(uint8_t cmd, Potentiometer *ptn, int noiseThreshold = POT_NOISE_THRESHOLD);
   ~EffectControlPtmtr();

  protected:
    bool triggered() const;
    void getData(CtrlQueueData &data);

  protected:
   uint16_t  _value:11;
   uint16_t  _noiseThreshold:5;
};

//////////////////////////////
// EffectControlIRBtn - analog input is one IR remote buttons
// Returns returns delta
// dir - direction (true is positive, false is negative)
// repeat - button repeat limit, 0 = single push, same as next or prev
class EffectControlIRBtn: public EffectControl{
  public:
    EffectControlIRBtn(uint8_t cmd, IRRemoteRecv *ir, unsigned long btn, bool dir = true, uint8_t repeat = 0);
    ~EffectControlIRBtn();

   protected:
    bool triggered() const;
    void getData(CtrlQueueData &data);

  protected:
    uint16_t _btn; //Making it 16 bit instead of 32 to save some memory for uno and nano. For the purpose of the remote key recognition first leading FFs are stripped
    uint8_t  _dir:1;
    uint8_t  _repeat:7;
};

////////////////////////////////////////
// Rotary encoder control
// Always returns incremental/decremental value
#define ROTECT_DEFAULT_INC 10 //Default incremement value

class EffectControlRotEnc: public EffectControl{
  public:
    EffectControlRotEnc(uint8_t cmd, RotaryEncoder *re, uint8_t inc = ROTECT_DEFAULT_INC);
    ~EffectControlRotEnc();

  protected:
    bool triggered() const;
    void getData(CtrlQueueData &data);  

  protected: 
    uint8_t  _inc;
};

////////////////////////////////////////
// EffectControlPanel

#define MAX_CONTROLS 20
#define MAX_INPUTS   10

class EffectControlPanel{
  public:
    EffectControlPanel();
   ~EffectControlPanel();

    void addControl(EffectControl *ctrl);

    void loop(CtrlQueueItem &itm);

  protected:
    EffectControl* _controls[MAX_CONTROLS]; //Controls
    uint8_t        _numControls;            //Number of controls
    uint8_t        _controlNum;             //Last processed control

    AnalogInput*  _inputs[MAX_INPUTS];     //Analog inputs
    uint8_t       _numInputs;
};


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


///////////////////////////////////////
//Control map
#define BEGIN_CONTROL_MAP() \
  _CM EffectControlPanel cp; \
  IRRemoteRecv *r = NULL; \
  PushButton *pb  = NULL; 

#define END_CONTROL_MAP()

#define BEGIN_PUSH_BUTTON(pin) \
  _CM PushButton BTN(pin); \
  pb = &BTN;  

#define END_PUSH_BUTTON() \
  pb = NULL;  

#define PUSH_BUTTON_TO_CMD(cmd, ...) \
  _CM EffectControlPb BTN_CTRL(cmd, pb, __VA_ARGS__); \
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
  r = &IR_NAME;
  
#define END_REMOTE() \
  r = NULL;


#define RMT_BUTTON_TO_CMD(cmd, code) \
  _CM EffectControlIRBtn IR_CTRL(cmd, r, code); \
  cp.addControl(&IR_CTRL);

#define RMT_BUTTON_PAIR_TO_CMD(cmd, code1, code2, repeat) \
  _CM EffectControlIRBtn IR_CTRL_UP(cmd, r, code1, true, repeat); \
  cp.addControl(&IR_CTRL_UP); \
  _CM EffectControlIRBtn IR_CTRL_DOWN(cmd, r, code2, false, repeat); \
  cp.addControl(&IR_CTRL_DOWN);


#endif //__EFFECTCONTROLS_H
