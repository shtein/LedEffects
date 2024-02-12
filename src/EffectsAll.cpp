#include "LedEffects.h"
#include <Controls.h>
#include "EffectsAll.h"

//Effects

#include "Effects/Rainbow.h"
#include "Effects/FadeInOut.h"
#include "Effects/RunningLights.h"
#include "Effects/ColorWipe.h"
#include "Effects/MoodBlobs.h"
#include "Effects/Static.h"
#include "Effects/Noise.h"
#include "Effects/Blur.h"
#include "Effects/EmergencyLights.h"
#include "Effects/Fire.h"
#include "Effects/Meteor.h"  
#include "Effects/Ripple.h"
#include "Effects/Juggle.h"
#include "Effects/TwinkleFox.h"
#include "Effects/PacificOcean.h"
//#include "Effects/MatrixAnimation.h"
//#include "Effects/Sound.h"

#ifdef NTF_ENABLED
  #define _GET_EFFECT_NAME(ed, ename) \
    ed.effectName = ename;    
#else
  #define _GET_EFFECT_NAME(ed, ename)
#endif


#define GET_EFFECT(ed, EffectClass, ename) \
  { \
    static EffectClass e; \
    ed.effect     = &e; \
    _GET_EFFECT_NAME(ed, ename); \
  }

DEFINE_STR_PROGMEM(rs_Effect_StaticColor,        "Static color")
DEFINE_STR_PROGMEM(rs_Effect_Blur,               "Blur")
DEFINE_STR_PROGMEM(rs_Effect_ColorWipe,          "Color wipe")
DEFINE_STR_PROGMEM(rs_Effect_EmergencyLights,    "Emergency lights")
DEFINE_STR_PROGMEM(rs_Effect_FadeInOut,          "Fade in out")
DEFINE_STR_PROGMEM(rs_Effect_Juggle,              "Juggle")
DEFINE_STR_PROGMEM(rs_Effect_MeteorRain,          "Meteor rain")
DEFINE_STR_PROGMEM(rs_Effect_MoodBlobs,           "Mood blobs")
DEFINE_STR_PROGMEM(rs_Effect_PaletteTransform,    "Palette transform")
DEFINE_STR_PROGMEM(rs_Effect_Noise,               "Noise")
DEFINE_STR_PROGMEM(rs_Effect_Plasma,              "Plasma")
DEFINE_STR_PROGMEM(rs_Effect_Confetti,            "Confetti")
DEFINE_STR_PROGMEM(rs_Effect_PacificOcean,        "Pacific ocean")
DEFINE_STR_PROGMEM(rs_Effect_Rainbow,             "Rainbow")
DEFINE_STR_PROGMEM(rs_Effect_TheaterChaseRainbow, "Theater chase rainbow")
DEFINE_STR_PROGMEM(rs_Effect_RainbowMove,         "Rainbow move")
DEFINE_STR_PROGMEM(rs_Effect_Ripple,              "Ripple" )
DEFINE_STR_PROGMEM(rs_Effect_RunningLigts,        "Running lights")
DEFINE_STR_PROGMEM(rs_Effect_TwinkleFox,          "Twinkle fox")
DEFINE_STR_PROGMEM(rs_Effect_Fire,                "Fire")


bool getEffect(uint8_t effectId, EFFECT_DESCRIPTION &ed){
  
  ed.effectId   = effectId;
  ed.effect     = NULL;
#ifdef NTF_ENABLED 
  ed.effectName = NULL;
#endif  


  switch(effectId){
    case el_StaticColor:
      GET_EFFECT(ed, EffectStatic, rs_Effect_StaticColor);
    break;
    case el_Blur:
      GET_EFFECT(ed, EffectBlur, rs_Effect_Blur);
    break;    
    case el_ColorWipe:
      GET_EFFECT(ed, EffectColorWipe, rs_Effect_ColorWipe);
    break;    
    case el_EmergencyLights:
      GET_EFFECT(ed, EffectEmergencyLights, rs_Effect_EmergencyLights);
    break;
    case el_FadeInOut:
      GET_EFFECT(ed, EffectFadeInOut, rs_Effect_FadeInOut);
    break;
    case el_Juggle:
      GET_EFFECT(ed, EffectJuggle, rs_Effect_Juggle);
    break;
    case el_MeteorRain:
      GET_EFFECT(ed, EffectMeteorRain, rs_Effect_MeteorRain);      
    break;    
    case el_MoodBlobs:
      GET_EFFECT(ed, EffectMoodBlobs, rs_Effect_MoodBlobs);      
    break;        
    case el_PaletteTransform:
      GET_EFFECT(ed, EffectPaletteTransformFast, rs_Effect_PaletteTransform);      
    break;        
    case el_Noise:
      GET_EFFECT(ed, EffectNoise, rs_Effect_Noise);      
    break;        
    case el_Plasma:
      GET_EFFECT(ed, EffectPlasma, rs_Effect_Plasma);      
    break;        
    case el_Confetti:
      GET_EFFECT(ed, EffectConfetti, rs_Effect_Confetti);      
    break;    
    case el_PacificOcean:
      GET_EFFECT(ed, EffectPacificOcean, rs_Effect_PacificOcean);      
    break;    
    case el_Rainbow:
      GET_EFFECT(ed, EffectRainbow, rs_Effect_Rainbow);      
    break;    
    case el_TheaterChaseRainbow:
      GET_EFFECT(ed, EffectTheaterChaseRainbow, rs_Effect_TheaterChaseRainbow);      
    break;
    case el_RainbowMove:
      GET_EFFECT(ed, EffectRainbowMove, rs_Effect_RainbowMove);      
    break;    
    case el_Ripple:
      GET_EFFECT(ed, EffectRipple, rs_Effect_Ripple);      
    break;    
    case el_RunningLights:
      GET_EFFECT(ed, EffectRunningLights, rs_Effect_RunningLigts);      
    break;
    case el_TwinkleFox:
      GET_EFFECT(ed, EffectTwinkleFox, rs_Effect_TwinkleFox);      
    break;
    case el_Fire:
      GET_EFFECT(ed, EffectFire, rs_Effect_Fire);      
    break;
    default:
    return false;
  }


  return true;
}
