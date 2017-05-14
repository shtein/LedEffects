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
// Push button control, reacts on click
class EffectControlPb: public EffectControl{
  public:
    EffectControlPb(uint8_t cmd, PushButton *input, uint8_t flag = CTF_VAL_NEXT);
   ~EffectControlPb();

  protected:
    bool triggered() const;
    void getData(CtrlQueueData &data);

  protected:
    uint8_t _flag;
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
    unsigned long _btn;
    uint8_t       _dir:1;
    uint8_t       _repeat:7;
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
//Control map
#define BEGIN_CONTROL_MAP(p) \ 
  EffectControlPanel *ecc = &p;

#define END_CONTROL_MAP()

#define PUSH_BUTTON_TO_CMD(name, cmd, pin) \ 
  PushButton btn##name(pin); \
  EffectControlPb ec##name(cmd, &btn##name); \
  ecc->addControl(&ec##name);

#define POT_TO_CMD(name, cmd, pin) \ 
  Potentiometer pot##name(pin); \
  EffectControlPtmtr ec##name(cmd, &pot##name); \
  ecc->addControl(&ec##name);

#define BEGIN_REMOTE(name, pin) \ 
  IRRemoteRecv rmt##Name(pin); \
  IRRemoteRecv *r = &rmt##Name;
#define END_REMOTE()


#define RMT_BUTTON_TO_CMD(name, cmd, code) \ 
  EffectControlIRBtn rmb##name(cmd, r, code); \
  ecc->addControl(&rmb##name);

#define RMT_BUTTON_PAIR_TO_CMD(name, cmd, code1, code2, repeat) \ 
  EffectControlIRBtn rmb##name##Up(cmd, r, code1, true, repeat); \ 
  ecc->addControl(&rmb##name##Up); \
  EffectControlIRBtn rmb##name##Down(cmd, r, code2, false, repeat); \
  ecc->addControl(&rmb##name##Down);


#endif //__EFFECTCONTROLS_H
