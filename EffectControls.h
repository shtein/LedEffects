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


#endif //__EFFECTCONTROLS_H
