#include "LedEffects.h"

#include <Fastled.h>
#include <Controls.h>
#include <EEPROMCfg.h>

#include "effectenginectx.h"
#include "effect.h"


void setRandomColor(CHSV &hsv){
  hsv.h = random8();
  hsv.s = 0xFF;
  hsv.v = 0xFF;
}


#ifdef NTF_ENABLED

 void putNtfObject(NtfBase &resp, const CHSV &data){
  resp.put_F(rs_Hue, data.h);
  resp.put_F(rs_Sat, data.s);
  resp.put_F(rs_Val, data.v);
}


//Getting/setting pallete transform rutine
struct EEResp_EffectTransform{  
  uint8_t transform;
};

void putNtfObject(NtfBase &resp, const EEResp_EffectTransform &data){
  resp.put_F(rs_Transform, data.transform);
}


//Getting/setting effect speed
struct EEResp_EffectSpeed{  
  uint16_t speed;
};

void putNtfObject(NtfBase &resp, const EEResp_EffectSpeed &data){
  resp.put_F(rs_Speed, data.speed);
}


#endif //NTF_ENABLED

/////////////////////////////////
// Effect
uint8_t Effect::_speedDelay = 0;
EFFECT_DATA Effect::_cfg;
Effect::EFFECT_CONTEXT Effect::_ctx;

//Draw 
void  Effect::draw(CRGB *leds, uint16_t numLeds){
  proceed(leds, numLeds);

#ifdef USE_MATRIX
  if(_cfg.flags & ECF_KALEYDOSCOPE){
      kaleidoscope(leds, numLeds);
  }
#endif

}


bool Effect::onCmd(const struct CtrlQueueItem &itm, NtfSet &ntf){ 

  //Process command
  switch(itm.cmd){    
    case EEMC_SPEED:
      setSpeedDelay(itm.data.translate(getSpeedDelay(), SPEED_DELAY_MIN, SPEED_DELAY_MAX));
    break;

#ifdef NTF_ENABLED  
    //All get commands
    case EEMC_GET_SPEED:
    break;
#endif     

    default:
    return false;
  } 

  //Notifications
#ifdef NTF_ENABLED  
  switch(itm.cmd){    
    case EEMC_SPEED:      
    case EEMC_GET_SPEED:
    { ntf.put(CmdResponse<EEResp_EffectSpeed> {itm.cmd, { getSpeedDelay() }}); }
    break;

    break;    
  }    
#endif         

  return true;
}

void Effect::setSpeedDelay(uint8_t speedDelay){  
  _speedDelay = speedDelay < SPEED_DELAY_MIN ? SPEED_DELAY_MIN : speedDelay > SPEED_DELAY_MAX ? SPEED_DELAY_MAX : speedDelay;
}

uint8_t Effect::getSpeedDelay() const{  
  return _speedDelay;
}

void Effect::setConfig(const EFFECT_DATA &cfg){
  _cfg = cfg;
}

void Effect::getConfig(EFFECT_DATA &cfg){
  cfg = _cfg;
}
    


//////////////////////////////////////
// EffectColor


bool EffectColor::onCmd(const struct CtrlQueueItem &itm, NtfSet &ntf){  
//Process command  
  switch(itm.cmd){
    case EEMC_COLOR_HUE: 
    case EEMC_COLOR_SAT: 
    case EEMC_COLOR_VAL: {
      //Get effect color
      CHSV hsv = _cfg.hsv;

      //Update corresponding color value
      hsv.raw[itm.cmd - EEMC_COLOR_HUE] = (uint8_t)itm.data.translate( (int)hsv.raw[itm.cmd - EEMC_COLOR_HUE], 0, 255);
  
      //Set effect color
      _cfg.hsv = hsv;       
    }   
    break;

#ifdef NTF_ENABLED
  //All get commands
    case EEMC_GET_COLOR_HSV:
    break;
#endif

    default:
    return Effect::onCmd(itm, ntf);    
  }    

#ifdef NTF_ENABLED
//Notification
  switch(itm.cmd){
    case EEMC_COLOR_HUE: 
    case EEMC_COLOR_SAT: 
    case EEMC_COLOR_VAL: 
    case EEMC_GET_COLOR_HSV:
      { ntf.put(CmdResponse<CHSV>{ itm.cmd, {  _cfg.hsv } }); }   
    break;
  }    
#endif

  return true;
}


//////////////////////////////////////
// EffectPaletteTransform

void EffectPaletteTransform::updatePal(){
  //Retreive transformation schema
  TRANSFORM_DESCRIPTION td;
  if(getPalTransform(EFFECT_PARAM_TRANSFORM(_cfg), td)){
    td.tFunc(_ctx.palTarget);
  }
}

void EffectPaletteTransform::reset(){
  
  //Init target pallete
  updatePal();
  
  //Safe target into current  
  _ctx.palCurrent = _ctx.palTarget;
                      
   //Update target palette again
  updatePal();

  //Reset step
  _ctx.step = getMaxStep();

  //Speed
  setSpeedDelay(25);  
}


void EffectPaletteTransform::proceed(CRGB *leds, uint16_t numLeds){
    //Check if it is to update target palette
  if(_ctx.step == 0){
    //Update palette
    updatePal();

    //Reset step
    _ctx.step = getMaxStep();
  }
  
  //Proceed with palette transtion
  nblendPaletteTowardPalette(_ctx.palCurrent, _ctx.palTarget, MAX_PAL_CHANGES); 

  //Prepare for the next move                                        
  _ctx.step--;              
}

int EffectPaletteTransform::getMaxStep() const{
  return CHANGE_PAL_STEP;
}

CRGB EffectPaletteTransform::getCurrentPalColor(uint8_t index, uint8_t brightness, TBlendType blendType) const{
  return ColorFromPalette(_ctx.palCurrent, index, brightness, blendType);
}

bool EffectPaletteTransform::onCmd(const struct CtrlQueueItem &itm, NtfSet &ntf){
  switch(itm.cmd){
  //All get commands
    case EEMC_TRANSFORM:{
      TRANSFORM_DESCRIPTION td;
      if(getPalTransform(itm.data.value, td)){
        EFFECT_PARAM_TRANSFORM(_cfg) = td.transformId;
      }
    }
    break;
    
#ifdef NTF_ENABLED    
    case EEMC_GET_TRANSFORM: 
    break;    
#endif

    default:
    return Effect::onCmd(itm, ntf);    
  }    

#ifdef NTF_ENABLED
//Notification
  switch(itm.cmd){
    case EEMC_GET_TRANSFORM: 
    case EEMC_TRANSFORM:     
      { ntf.put(CmdResponse<EEResp_EffectTransform>{ itm.cmd, { EFFECT_PARAM_TRANSFORM(_cfg) } } ); }   
    break;
  }    
#endif

  return true;

}