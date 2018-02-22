#include "precomp.h"
#include "Controls.h"


int powInt(int x, int y, int limit){
  int n = 1;

  if(y > limit)
    y = limit;
  
  for(int i = 0; i < y; i++){
    n = n * x;
  }

  return n; 
}

////////////////////////////
// EffectControl
CtrlItem::CtrlItem(uint8_t cmd, AnalogInput *input) {
   _input = input;
   _cmd   = cmd;
}

CtrlItem::~CtrlItem(){
}

AnalogInput *CtrlItem::getInput() const{
  return _input;
}

void CtrlItem::loop(CtrlQueueItem &itm){
  //Check if triggered
  if(triggered()){
    //Prepare command
    itm.cmd = _cmd;
    //Retrieve data
    getData(itm.data);
  }
}



////////////////////////////
// CtrlItemPb
CtrlItemPb::CtrlItemPb(uint8_t cmd, PushButton *btn, uint8_t ctrl, uint8_t flag):
  CtrlItem(cmd, btn){
  _flag = flag;
  _ctrl = ctrl;  
}

CtrlItemPb::~CtrlItemPb(){
}

bool CtrlItemPb::triggered() const{
  return ((PushButton *)getInput())->value(_ctrl);
}

void CtrlItemPb::getData(CtrlQueueData &data){
  data.flag  = _flag;
  data.value = 0;
  data.min   = 0;
  data.max   = 0;
}


////////////////////////////
//CtrlItemPtmtr

CtrlItemPtmtr::CtrlItemPtmtr(uint8_t cmd, Potentiometer *ptn, int noiseThreshold):
  CtrlItem(cmd, ptn) {
  _value          = POT_MAX + 1; //just to make sure it is different from what we read
  _noiseThreshold = noiseThreshold; 
}

CtrlItemPtmtr::~CtrlItemPtmtr(){
}

bool CtrlItemPtmtr::triggered() const{ 
  uint16_t value = ((Potentiometer *)getInput())->value();

  return (abs(value - _value) > _noiseThreshold);
}

void CtrlItemPtmtr::getData(CtrlQueueData &data){
  _value     = ((Potentiometer *)getInput())->value();
  
  data.flag  = CTF_VAL_ABS;
  data.value = _value;
  data.min   = POT_MIN;
  data.max   = POT_MAX;
}


////////////////////////////
// CtrlItemIRBtn

#define BTN_TO_KEY(btn) ((unsigned long)btn | 0xFF0000)
#define KEY_TO_BTN(key) (uint16_t)(btn & 0x0000FFFF)

CtrlItemIRBtn::CtrlItemIRBtn(uint8_t cmd, IRRemoteRecv *ir, unsigned long btn, bool dir, uint8_t repeat):
  CtrlItem(cmd, ir) {
  _btn    = KEY_TO_BTN(btn);
  _dir    = dir;
  _repeat = repeat;
}

CtrlItemIRBtn::~CtrlItemIRBtn(){
}

bool CtrlItemIRBtn::triggered() const{
  int n = ((IRRemoteRecv *)getInput())->pushed(BTN_TO_KEY(_btn));

  //Not pushed
  if(n == 0) {
    return false;
  }
  
  //Single click button
  if(n > 1 && _repeat == 0){
    return false;
  }
  
  //Pushed
  return true;
}

void CtrlItemIRBtn::getData(CtrlQueueData &data){
  data.flag  = _repeat > 0 ? CTF_VAL_DELTA : (_dir ? CTF_VAL_NEXT: CTF_VAL_PREV ); 
  data.value = (_dir ? 1 : -1) * powInt(2, ((IRRemoteRecv *)getInput())->pushed(BTN_TO_KEY(_btn)) - 1, _repeat);
  data.min   = 0;
  data.max   = 0;
}


////////////////////////////
// CtrlItemRotEnc
CtrlItemRotEnc::CtrlItemRotEnc(uint8_t cmd, RotaryEncoder *re, uint8_t inc): 
  CtrlItem(cmd, re) {
  _inc = inc;
}

CtrlItemRotEnc::~CtrlItemRotEnc(){
}

bool CtrlItemRotEnc::triggered() const{ 
  return ((RotaryEncoder *)getInput())->value() != 0;
}

void CtrlItemRotEnc::getData(CtrlQueueData &data){ 
  data.flag  = CTF_VAL_DELTA;
  data.value = ((RotaryEncoder *)getInput())->value() * _inc;  
  data.min   = 0;
  data.max   = 0;
}


////////////////////////////
// CtrlPanel
CtrlPanel::CtrlPanel(){
  _numControls = 0;
  _controlNum  = 0;
  _numInputs   = 0;
}

CtrlPanel::~CtrlPanel(){
}

void CtrlPanel::addControl(CtrlItem *ctrl){
  if(!ctrl)
    return;

  if(_numControls == sizeof(_controls)/sizeof(_controls[0]))
    return;

  //Add control
  _controls[_numControls] = ctrl;
  _numControls ++;

  
  //Add analog input - should be only of instance to avoid reading it twice
  AnalogInput *input = ctrl->getInput();
  if(!input)
    return;

  //Find if input is already there
  bool found = false;
  for(int i = 0; i < _numInputs; i++){
    if(input == _inputs[i]){
      found = true;
      break;
    }
  }

  //Save if not found
  if(!found){
    
    //Check number of inputs
    if(_numInputs == sizeof(_inputs)/sizeof(_inputs[0])) 
      return;
        
    _inputs[_numInputs] = input;
    _numInputs ++;
  }
}

void CtrlPanel::loop(CtrlQueueItem &itm){
  //Reset cmd
  itm.cmd       = EEMC_NONE;
  itm.data.flag = CTF_NONE;
  
  if(_controlNum >= _numControls){
     _controlNum = 0;
    
    //Read inputs
    for(int i = 0; i < _numInputs; i++){
      _inputs[i]->read();
    }
     
    //Send idle command once in a while
    return;
  }

  //process current control
  _controls[_controlNum]->loop(itm);

  //Go with next control next time
  _controlNum ++;
}

