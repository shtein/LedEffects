#include "LedEffects.h"

#include <Fastled.h>
#include <Controls.h>
#include <EEPROMCfg.h>

#include "effectenginectx.h"
#include "effect.h"

#ifdef USE_SOUND  
#include <SoundCapture.h>

#ifdef NTF_ENABLED

//Getting/setting effect speed
struct EEResp_EffectSound{  
  uint8_t flags;
  uint8_t lower;
  uint8_t upper;
  const RunningStats &min;
  const RunningStats &max;
  const RunningStats &average;
};

void putNtfObject(NtfBase &resp, const RunningStats &data){
  resp.put_F(rs_SndAverage, data.getAverage());
  resp.put_F(rs_SndStdDev, data.getStdDev());
} 

void putNtfObject(NtfBase &resp, const EEResp_EffectSound &data){
  resp.put_F(rs_Flags, data.flags);
  resp.put_F(rs_SndLower, data.lower);
  resp.put_F(rs_SndUpper, data.upper);
  
  resp.put_F(rs_SndMin, data.min);
  resp.put_F(rs_SndMax, data.max);
  resp.put_F(rs_SndAverage, data.average);
}
#endif //NTF_ENABLED

#endif //USE_SOUND


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


bool Effect::onCmd(const struct CtrlQueueItem &itm, NtfSet &ntf){ 

  //Process command
  switch(itm.cmd){    
    case EEMC_SPEED:
      setSpeedDelay(itm.data.translate(getSpeedDelay(), SPEED_DELAY_MIN, SPEED_DELAY_MAX));
      //Notification
#ifdef NTF_ENABLED      
    case EEMC_GET_SPEED:
    { ntf.put(CmdResponse<EEResp_EffectSpeed> {itm.cmd, { getSpeedDelay() }}); }
#endif     
    break;

    default:
    #if defined (USE_SOUND) && !defined(NO_SOUND_COMMANDS)      
        //Sound command
      if(_cfg.flags & ECF_SOUND){
        return onCmdSound(itm, ntf);
      }    
    #endif //USE_SOUND
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

  _ctxSound.bassTicks++;
  _ctxSound.midTicks++;
  _ctxSound.trebleTicks++;  
}


bool Effect::onCmdSound(const struct CtrlQueueItem &itm, NtfSet &ntf){  
  
  switch(itm.cmd){    
    case EEMC_SOUND_LOW:
      _ctxSound.lower = itm.data.translate(_ctxSound.lower, SOUND_LOWER_MIN, SOUND_UPPER_MAX);
    break;

    case EEMC_SOUND_HIGH:
      _ctxSound.upper = itm.data.translate(_ctxSound.upper, SOUND_LOWER_MIN, SOUND_UPPER_MAX);
    break;
    
    case EEMC_SOUND_LOG:
      _ctxSound.flags = itm.data.value == 0 ? _ctxSound.flags & ~SC_MAP_LOG : _ctxSound.flags | SC_MAP_LOG; 
    break;

    case EEMC_SOUND_USE_MAX:
      _ctxSound.flags = itm.data.value == 0 ? _ctxSound.flags & ~SC_MAP_USE_MAX : _ctxSound.flags | SC_MAP_USE_MAX; 
    break;

    case EEMC_SOUND_USE_MIN:
      _ctxSound.flags = itm.data.value == 0 ? _ctxSound.flags & ~SC_MAP_USE_MIN : _ctxSound.flags | SC_MAP_USE_MIN; 
    break;

    case EEMC_SOUND_NOISE:
      _ctxSound.flags = itm.data.value == 0 ? _ctxSound.flags & ~SC_MAP_ABOVE_NOISE : _ctxSound.flags | SC_MAP_ABOVE_NOISE;
    break;

    case EEMC_GET_SOUND:
    break;

    default:
    return false;
  }

#ifdef NTF_ENABLED    
/*
  ntf.put(CmdResponse<EEResp_EffectSound> { itm.cmd, {_ctxSound.flags, _ctxSound.lower, _ctxSound.upper,
                                                      _statsSound.get(SoundStatGet::ssgMin), 
                                                      _statsSound.get(SoundStatGet::ssgMax),
                                                      _statsSound.get(SoundStatGet::ssgAverage)                                                          
                                                    } 
                                               }); 
*/                                               
#endif

  return true;
}


#endif //USE_SOUND


//////////////////////////////////////
// EffectColor


bool EffectColor::onCmd(const struct CtrlQueueItem &itm, NtfSet &ntf){  
//Process command  
  switch(itm.cmd){
    case EEMC_COLOR_HUE: 
    case EEMC_COLOR_SAT: 
    case EEMC_COLOR_VAL:
      //Update corresponding color value
      _cfg.bytes[itm.cmd - EEMC_COLOR_HUE] = (uint8_t)itm.data.translate( (int)_cfg.bytes[itm.cmd - EEMC_COLOR_HUE], 0, 255);
#ifdef NTF_ENABLED
    //Notfification
    case EEMC_GET_COLOR_HSV:
      { ntf.put(CmdResponse<CHSV>{ itm.cmd, {  CHSV(_cfg.bytes[0], _cfg.bytes[1], _cfg.bytes[2]) } }); }   
#endif    
    break;

    default:
    return Effect::onCmd(itm, ntf);    
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

bool EffectPaletteTransform::onCmd(const struct CtrlQueueItem &itm, NtfSet &ntf){
  
  switch(itm.cmd){
  //All get commands
    case EEMC_TRANSFORM:{
      TRANSFORM_DESCRIPTION td;
      if(getPalTransform(itm.data.value, td)){
        EFFECT_PARAM_TRANSFORM(_cfg) = td.transformId;
      }
    }
#ifdef NTF_ENABLED    
    case EEMC_GET_TRANSFORM: 
      { ntf.put(CmdResponse<EEResp_EffectTransform>{ itm.cmd, { EFFECT_PARAM_TRANSFORM(_cfg) } } ); }   
#endif
    break;    

    default:
    return Effect::onCmd(itm, ntf);    
  }    

  return true;

}