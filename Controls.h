#ifndef __EFFECTCONTROLS_H
#define __EFFECTCONTROLS_H

#include <AnalogInput.h>

//Change commands
#define EEMC_NONE        0x00   //Nothing changed

///////////////////////////////////
// Control flags
#define CTF_NONE        0x00  //Nothing, value is absolute number
#define CTF_VAL_ABS     0x00  //Absolute number
#define CTF_VAL_DELTA   0x01  //Value is delta
#define CTF_VAL_NEXT    0x02  //Go next - in cycles
#define CTF_VAL_PREV    0x03  //Go previous - in cycles


////////////////////////////////////
// Control queue data
struct CtrlQueueData{
  uint8_t flag;   //Flag that shows how to interpret the value: absolute number, inrement, etc 
  int     value;  //Value
  int     min;    //Value minimum
  int     max;    //Value maximum   
  
  CtrlQueueData(){
      flag   = CTF_NONE;
      value  = 0;
      min    = 0;
      max    = 0;
  }

  int translate(int base, int vmin, int vmax) const{
    
     switch(flag){ 
      case CTF_VAL_ABS:  //Absolute value
        base = map(value, min, max, vmin, vmax);
      break;
      case CTF_VAL_NEXT: //Go next
        base ++;
        if(base > vmax) base = vmin;
      break;
      case CTF_VAL_PREV: //Go Prev
        base --;
        if(base < vmin) base = vmax;
      break;
      case CTF_VAL_DELTA://Delta
        base += value;
        if(base < vmin)      base = vmin;
        else if(base > vmax) base = vmax;
      break;
    }

    return base;
  }
};


////////////////////////////////////
// Control queue element
struct CtrlQueueItem {
  uint8_t       cmd;    // Command
  CtrlQueueData data;   // Data

  CtrlQueueItem(){
     cmd = 0;
  }
};

//////////////////////////////////////////
// ProcessControl - base class
class CtrlItem{
  public:
    CtrlItem(uint8_t cmd, BaseInput *input);
   ~CtrlItem();

    void loop(CtrlQueueItem &itm);
    
    BaseInput *getInput() const;
    
  protected:
    virtual bool triggered() const = 0;
    virtual void getData(CtrlQueueData &data) = 0;

  protected:
    uint8_t       _cmd;     //Command
    BaseInput    *_input;   //Analog input to retrieve control data
};

////////////////////////////////
// Push button control, reacts on either short click, long click or long push
class CtrlItemPb: public CtrlItem{
  public:
    CtrlItemPb(uint8_t cmd, PushButton *input, uint8_t ctrl = PB_CONTROL_CLICK, uint8_t flag = CTF_VAL_NEXT, int8_t value = 0);
   ~CtrlItemPb();

  protected:
    bool triggered() const;
    void getData(CtrlQueueData &data);

  protected:
    uint8_t _flag:4;
    uint8_t _ctrl:4;
    int8_t  _value;
};

////////////////////////////////
// CtrlItemPtmtr - analog input is Potentiometer - AnalogInput
#define POT_MIN             0
#define POT_MAX             1023

#define POT_NOISE_THRESHOLD 4


class CtrlItemPtmtr: public CtrlItem{
  public:
    CtrlItemPtmtr(uint8_t cmd, AnalogInput *ptn, int noiseThreshold = POT_NOISE_THRESHOLD);
   ~CtrlItemPtmtr();

  protected:
    bool triggered() const;
    void getData(CtrlQueueData &data);

  protected:
   uint16_t  _value:11;
   uint16_t  _noiseThreshold:5;
};


//////////////////////////////
// CtrlMic - analog input is Microphone - AnalogInput
class CtrlMic: public CtrlItem{
  public:
    CtrlMic(uint8_t cmd, AnalogInput *mic);
    ~CtrlMic();

   protected:
    bool triggered() const;
    void getData(CtrlQueueData &data);
};

//////////////////////////////
// CtrlItemIRBtn - analog input is one IR remote buttons
// Returns returns delta
// dir - direction (true is positive, false is negative)
// repeat - button repeat limit, 0 = single push, same as next or prev
class CtrlItemIRBtn: public CtrlItem{
  public:
    CtrlItemIRBtn(uint8_t cmd, IRRemoteRecv *ir, unsigned long btn, bool dir = true, uint8_t repeat = 0);
    ~CtrlItemIRBtn();

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

class CtrlItemRotEnc: public CtrlItem{
  public:
    CtrlItemRotEnc(uint8_t cmd, RotaryEncoder *re, uint8_t inc = ROTECT_DEFAULT_INC);
    ~CtrlItemRotEnc();

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

class CtrlPanel{
  public:
    CtrlPanel();
   ~CtrlPanel();

    void addControl(CtrlItem *ctrl);

    void loop(CtrlQueueItem &itm);

  protected:
    CtrlItem      *_controls[MAX_CONTROLS]; //Controls
    uint8_t        _numControls;            //Number of controls
    uint8_t        _controlNum;             //Last processed control

    BaseInput    *_inputs[MAX_INPUTS];     //Analog inputs
    uint8_t       _numInputs;
};


#endif //__EFFECTCONTROLS_H
