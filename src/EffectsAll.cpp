#include "LedEffects.h"
#include <Controls.h>
#include <utils.h>
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
#ifdef USE_MATRIX
  #include "Matrix.h"
  #include "Effects/MatrixAnimation.h"
#endif
//#include "Effects/Sound.h"

#ifdef NTF_ENABLED
  #define _GET_EFFECT_NAME(ed, ename) \
    ed.effectName = ename;    
#else
  #define _GET_EFFECT_NAME(ed, ename)
#endif

#define _EFFECT_FLAGS_NONE(flags, ...)
#define _EFFECT_FLAGS_DEFINED(flags, ...) flags = ARG_NUM_1(__VA_ARGS__)

#define _GET_EFFECT_FLAGS(flags, ...) ARG_NUM( NUM_ARGS(flags, ##__VA_ARGS__), _EFFECT_FLAGS_NONE, _EFFECT_FLAGS_DEFINED)(flags, __VA_ARGS__)

#define GET_EFFECT(ed, EffectClass, ename, ...) \
  { \
    static EffectClass e; \
    ed.effect     = &e; \
    _GET_EFFECT_FLAGS(ed.flags, ##__VA_ARGS__); \
    _GET_EFFECT_NAME(ed, ename); \
  }

DEFINE_STR_PROGMEM(rs_Effect_StaticColor,          "Static color")
DEFINE_STR_PROGMEM(rs_Effect_Blur,                 "Blur")
DEFINE_STR_PROGMEM(rs_Effect_ColorWipe,            "Color wipe")
DEFINE_STR_PROGMEM(rs_Effect_EmergencyLights,      "Emergency lights")
DEFINE_STR_PROGMEM(rs_Effect_FadeInOut,            "Fade in out")
DEFINE_STR_PROGMEM(rs_Effect_Juggle,               "Juggle")
DEFINE_STR_PROGMEM(rs_Effect_MeteorRain,           "Meteor rain")
DEFINE_STR_PROGMEM(rs_Effect_MoodBlobs,            "Mood blobs")
DEFINE_STR_PROGMEM(rs_Effect_PaletteTransform,     "Palette transform")
DEFINE_STR_PROGMEM(rs_Effect_Noise,                "Noise")
DEFINE_STR_PROGMEM(rs_Effect_Plasma,               "Plasma")
DEFINE_STR_PROGMEM(rs_Effect_Confetti,             "Confetti")
DEFINE_STR_PROGMEM(rs_Effect_PacificOcean,         "Pacific ocean")
DEFINE_STR_PROGMEM(rs_Effect_Rainbow,              "Rainbow")
DEFINE_STR_PROGMEM(rs_Effect_TheaterChaseRainbow,  "Theater chase rainbow")
DEFINE_STR_PROGMEM(rs_Effect_RainbowMove,          "Rainbow move")
DEFINE_STR_PROGMEM(rs_Effect_Ripple,               "Ripple" )
DEFINE_STR_PROGMEM(rs_Effect_RunningLigts,         "Running lights")
DEFINE_STR_PROGMEM(rs_Effect_TwinkleFox,           "Twinkle fox")
DEFINE_STR_PROGMEM(rs_Effect_Fire,                 "Fire")
DEFINE_STR_PROGMEM(rs_Effect_Matrix_Drops,         "Matrix drops")
DEFINE_STR_PROGMEM(rs_Effect_Matrix_Bouncing_Dots, "Matrix bouncing dots")
DEFINE_STR_PROGMEM(rs_Effect_Matrix_Circles,        "Matrix circles")

uint8_t getEffectFlags(uint8_t effectId){
  EFFECT_DESCRIPTION ed;
  getEffect(effectId, ed);

  return ed.flags;
}

bool getEffect(uint8_t effectId, EFFECT_DESCRIPTION &ed){
  
  ed.effectId   = effectId;
  ed.effect     = NULL;
  ed.flags      = ECF_NONE;
#ifdef NTF_ENABLED 
  ed.effectName = NULL;
#endif  

  switch(effectId){

#ifndef NO_EFFECT_STATIC_COLOR    
    case el_StaticColor:
      GET_EFFECT(ed, EffectStatic, rs_Effect_StaticColor, ECF_HSV);
    break;
#endif //NO_EFFECT_STATIC_COLOR

#ifndef NO_EFFECT_BLUR    
    case el_Blur:
      GET_EFFECT(ed, EffectBlur, rs_Effect_Blur);
    break;    
#endif //NO_EFFECT_BLUR    

#ifndef NO_EFFECT_COLOR_WIPE    
    case el_ColorWipe:
      GET_EFFECT(ed, EffectColorWipe, rs_Effect_ColorWipe);
    break;    
#endif

#ifndef NO_EFFECT_EMERGANCY_LIGHTS    
    case el_EmergencyLights:
      GET_EFFECT(ed, EffectEmergencyLights, rs_Effect_EmergencyLights);
    break;
#endif //NO_EFFECT_EMERGANCY_LIGHTS

/*
    case el_FadeInOut:
      GET_EFFECT(ed, EffectFadeInOut, rs_Effect_FadeInOut);
    break;
    case el_Juggle:
      GET_EFFECT(ed, EffectJuggle, rs_Effect_Juggle);
    break;
    case el_MeteorRain:
      GET_EFFECT(ed, EffectMeteorRain, rs_Effect_MeteorRain);      
    break;    
*/    
    case el_MoodBlobs:
      GET_EFFECT(ed, EffectMoodBlobs, rs_Effect_MoodBlobs);      
    break;        

#ifndef NO_EFFECT_PALLETE_TRANSFORM    
    case el_PaletteTransform:
      GET_EFFECT(ed, EffectPaletteTransformFast, rs_Effect_PaletteTransform, ECF_TRANSFORM);      
    break;        
#endif //NO_EFFECT_PALLETE_TRANSFORM  

    case el_Noise:
      GET_EFFECT(ed, EffectNoise, rs_Effect_Noise, ECF_TRANSFORM);      
    break;        
    case el_Plasma:
      GET_EFFECT(ed, EffectPlasma, rs_Effect_Plasma, ECF_TRANSFORM);      
    break;        
    case el_Confetti:
      GET_EFFECT(ed, EffectConfetti, rs_Effect_Confetti, ECF_TRANSFORM);      
    break;    
    case el_PacificOcean:
      GET_EFFECT(ed, EffectPacificOcean, rs_Effect_PacificOcean);      
    break;    

#ifndef NO_EFFECT_RAINBOW
    case el_Rainbow:
      GET_EFFECT(ed, EffectRainbow, rs_Effect_Rainbow);      
    break;    
#endif //NO_EFFECT_RAINBOW

#ifndef NO_THEATER_CHASE_RAINBOW
    case el_TheaterChaseRainbow:
      GET_EFFECT(ed, EffectTheaterChaseRainbow, rs_Effect_TheaterChaseRainbow);      
    break;
#endif //NO_THEATER_CHASE_RAINBOW

#ifndef NO_EFFECT_RAINBOW_MOVE    
    case el_RainbowMove:
      GET_EFFECT(ed, EffectRainbowMove, rs_Effect_RainbowMove);      
    break;    
#endif //NO_EFFECT_RAINBOW_MOVE

#ifndef NO_EFFECT_RIPPLE
    case el_Ripple:
      GET_EFFECT(ed, EffectRipple, rs_Effect_Ripple);      
    break;    
#endif //NO_EFFECT_RIPPLE

#ifndef NO_EFFECT_RUNNING_LIGHTS
    case el_RunningLights:
      GET_EFFECT(ed, EffectRunningLights, rs_Effect_RunningLigts);      
    break;
#endif //NO_EFFECT_RUNNING_LIGHTS

    case el_TwinkleFox:
      GET_EFFECT(ed, EffectTwinkleFox, rs_Effect_TwinkleFox, ECF_TRANSFORM);      
    break;

#ifndef NO_EFFECT_FIRE    
    case el_Fire:
      GET_EFFECT(ed, EffectFire, rs_Effect_Fire);      
    break;
#endif //NO_EFFECT_FIRE    
    
#ifdef USE_MATRIX
    case el_Maxtrix_Drops:
      GET_EFFECT(ed, EffectMatrixDrops, rs_Effect_Matrix_Drops, ECF_TRANSFORM);
    break;

    case el_Matrix_Bouncing_Dots:
      GET_EFFECT(ed, EffectMatrixBounsingDots, rs_Effect_Matrix_Bouncing_Dots);
    break;

    case el_Matrix_Circles:
      GET_EFFECT(ed, EffectMatrixCircles, rs_Effect_Matrix_Circles, ECF_TRANSFORM);
    break;
  
#endif //USE_MATRIX

    default:
    return false;
  }


  return true;
}
