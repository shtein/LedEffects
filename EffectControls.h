#ifndef __EFFECTCONTROLS_H
#define __EFFECTCONTROLS_H

#include "EffectEngineCtx.h"
#include "AnalogInput.h"

//////////////////////////////////////////
// EffectControl - base class
class EffectControl{
  public:
    EffectControl(int cmd, AnalogInput *input);
   ~EffectControl();

    void loop(CtrlQueueItem &itm);
    
    AnalogInput *getInput() const;
    
  protected:
    virtual bool triggered() const = 0;
    virtual void getData(CtrlQueueData &data) = 0;

  protected:
    int           _cmd;     //Command
    AnalogInput  *_input;   //Analog input to retrieve control data
};

///////////////////////////////////////
//EffectControlPb - analog input is push button
class EffectControlPb: public EffectControl{
  public:
    EffectControlPb(int cmd, PushButton *btn, uint8_t flag = CTF_VAL_NEXT);
   ~EffectControlPb();

  protected:
    bool triggered() const;
    virtual void getData(CtrlQueueData &data);

  protected:
    uint8_t _flag;
};

////////////////////////////////
// EffectControlPtmtr - analog input is Potentiometer

class EffectControlPtmtr: public EffectControl{
  public:
    EffectControlPtmtr(int cmd, Potentiometer *ptn, int noiseThreshold = POT_NOISE_THRESHOLD);
   ~EffectControlPtmtr();

  protected:
    bool triggered() const;
    virtual void getData(CtrlQueueData &data);

  protected:
   int _value;
   int _noiseThreshold;
};

//////////////////////////////
// EffectControlIRPb - analog input is one IR remote buttons
class EffectControlIRPb: public EffectControl{
  public:
    EffectControlIRPb(int cmd, IRRemoteRecv *ir, int btn, uint8_t flag = CTF_VAL_NEXT);
    ~EffectControlIRPb();

   protected:
    bool triggered() const;
    virtual void getData(CtrlQueueData &data);

  protected:
    int _flag;
    int _btn;
};

////////////////////////////////////////
// EffectControlPanel

#define MAX_CONTROLS 20

class EffectControlPanel{
  public:
    EffectControlPanel();
   ~EffectControlPanel();

    void addControl(EffectControl *ctrl);

    void loop(CtrlQueueItem &itm);

  protected:
    EffectControl* _controls[MAX_CONTROLS]; //Controls
    int            _numControls;            //Number of controls
    int            _controlNum;             //Last processed control

    AnalogInput*  _inputs[MAX_CONTROLS];   //Analog inputs
    int           _numInputs;
};

/*
class EffectControls{
  public:
    EffectControls();
    ~EffectControls();

   void init(struct EffectEngineCtx &ctx);

    //To be called before or after EffectEngine::loop
    void loop(struct EffectEngineCtx &ctx);

  protected:
    int getCurrentEffect() const;
    void setCurrentEffect(int curEffect);
  
    int getSpeedDelay() const;
    void setSpeedDelay(int speedDelay);

    CHSV getHSV() const;
    void setHSV(const CHSV &hsv);

    int getNumLeds() const;
    void setNumLeds(int numLeds);

    int getMode() const;

    int getRemotePushedDec(int value, int limit, unsigned long key, int repeatLimit) const;
    int getRemotePushedInc(int value, int limit, unsigned long key, int repeatLimit) const;
    int getRemotePushedValue(int value, int limitDec, int limitInc, unsigned long keyDec, unsigned long keyInc, int repeatLimit) const;
    
    void readControls();

  protected:
    //Mode
    int _mode;
  
    //Effects
    int _maxEffects;
    int _effectNum;
  
    //Speed delay
    int _speedDelay;

    //Color
    CHSV _hsv;

    //Num leds
    int _numLeds;
    
    //Remote control
    IRRemoteRecv  _remote;
};
*/

#endif //__EFFECTCONTROLS_H
