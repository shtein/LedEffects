#include "LedEffects.h"

#include <Fastled.h>
#include <Controls.h>
#include <EEPROMCfg.h>

#include "effectenginectx.h"
#include "effect.h"
#include "Macro.h"
#include "Resp.h"


#ifdef NTF_ENABLED

 void putNtfObject(NtfBase &resp, const EEResp_EffectColor &data){
  resp.put_F(rs_Red, data.rgb[0]);
  resp.put_F(rs_Green, data.rgb[1]);
  resp.put_F(rs_Blue, data.rgb[2]);
}

//Getting/setting pallete transform rutine
void putNtfObject(NtfBase &resp, const EEResp_EffectTransform &data){
  resp.put_F(rs_Transform, data.transform);
}


//Getting/setting effect speed

void putNtfObject(NtfBase &resp, const EEResp_EffectSpeed &data){
  resp.put_F(rs_Speed, data.speed);
}


#endif //NTF_ENABLED

/////////////////////////////////
// Effect
uint8_t Effect::_speedDelay = 0;
EFFECT_DATA Effect::_cfg;
Effect::EFFECT_CONTEXT Effect::_ctx;

#ifdef USE_SOUND
//Sound specific
SoundCapture *Effect::_sc = NULL;
Effect::EFFECT_SOUND_CONTEXT Effect::_ctxSound = { SC_MAP_USE_MIN | SC_MAP_USE_MAX | SC_MAP_ABOVE_NOISE, SOUND_LOWER_MIN, SOUND_UPPER_MAX };
#endif


//Draw 
void Effect::draw(CRGB *leds, uint16_t numLeds){
  proceed(leds, numLeds);

//Matrix effect
#ifdef USE_MATRIX
  if(_cfg.flags & ECF_KALEYDOSCOPE){
      kaleidoscope(leds, numLeds);
  }
#endif
}


bool Effect::onCmd(const struct CtrlQueueItem &itm){ 

  //Process command
  switch(itm.cmd){    
    case EEMC_SPEED:
      setSpeedDelay(itm.data.translate(getSpeedDelay(), SPEED_DELAY_MIN, SPEED_DELAY_MAX));
      //Notification

    case EEMC_GET_SPEED:
      NTF_RESP(itm.cmd, EEResp_EffectSpeed, getSpeedDelay()); 
    break;

    default:
    return false;
  } 


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
    
#ifdef USE_SOUND    

void Effect::initSoundCapture(SoundCapture *sc){
    //Remember instances
    _sc = sc;
    //Init instance
    _sc->init();
}

void Effect::getSoundBands(sc_band_t &bands, bool scale){
  //Get band data
  _sc->getProcessedData(bands);

  if(scale){
    //Scale sound bands
    _sc->scaleSound(bands, _ctxSound.flags, _ctxSound.lower, _ctxSound.upper);
  }

  //Ticks
  _ctxSound.bassTicks++;
  _ctxSound.midTicks++;
  _ctxSound.trebleTicks++;  
}

void Effect::getSound(){
  sc_band_t bands;

  //Get band data
  _sc->getProcessedData(bands);

  //Ticks
  _ctxSound.bassTicks++;
  _ctxSound.midTicks++;
  _ctxSound.trebleTicks++;  
}

uint16_t Effect::beatCheckBass(uint16_t delta, uint8_t sensForBanAg, uint8_t sensForAvg) const {
  uint16_t n =  (uint16_t)_ctxSound.bassTicks * (uint16_t)getSpeedDelay();
  if( n >= delta && _sc->isBassPeak(sensForBanAg, sensForAvg))
    return n;
    
  return 0;
}

uint16_t Effect::beatCheckMid(uint16_t delta, uint8_t sensForBanAg, uint8_t sensForAvg) const {
  uint16_t n =  (uint16_t) _ctxSound.midTicks * (uint16_t)getSpeedDelay();
  if( n >= delta && _sc->isMidPeak(sensForBanAg, sensForAvg))
    return n;
    
  return 0;
}

uint16_t Effect::beatCheckTreble(uint16_t delta, uint8_t sensForBanAg, uint8_t sensForAvg) const {
  uint16_t n =  (uint16_t) _ctxSound.trebleTicks * (uint16_t)getSpeedDelay();
  if( n >= delta && _sc->isTreblePeak(sensForBanAg, sensForAvg))
    return n;
    
  return 0;
}

#endif //USE_SOUND


//////////////////////////////////////
// EffectColor
bool EffectColor::onCmd(const struct CtrlQueueItem &itm){  
//Process command  
  switch(itm.cmd){
    case EEMC_COLOR: {
      CRGB *rgb = (CRGB *)itm.data.str;
      _cfg.bytes[0] = rgb->r;
      _cfg.bytes[1] = rgb->g;
      _cfg.bytes[2] = rgb->b; 
    }
    //Notfification
    case EEMC_GET_COLOR:
      NTF_RESP(itm.cmd, EEResp_EffectColor, _cfg.bytes[0], _cfg.bytes[1], _cfg.bytes[2]);
    break;

    default:
    return Effect::onCmd(itm);    
  }    

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
  _ctx.step --;              
}

int EffectPaletteTransform::getMaxStep() const{
  return CHANGE_PAL_STEP;
}

CRGB EffectPaletteTransform::getCurrentPalColor(uint8_t index, uint8_t brightness, TBlendType blendType) const{
  return ColorFromPalette(_ctx.palCurrent, index, brightness, blendType);
}

bool EffectPaletteTransform::onCmd(const struct CtrlQueueItem &itm){
  
  switch(itm.cmd){
  //All get commands
    case EEMC_TRANSFORM:{
      TRANSFORM_DESCRIPTION td;
      if(getPalTransform(itm.data.value, td)){
        EFFECT_PARAM_TRANSFORM(_cfg) = td.transformId;
      }
    }

    case EEMC_GET_TRANSFORM: 
      NTF_RESP(itm.cmd, EEResp_EffectTransform, EFFECT_PARAM_TRANSFORM(_cfg));      
    break;    

    default:
    return Effect::onCmd(itm);    
  }    

  return true;
}