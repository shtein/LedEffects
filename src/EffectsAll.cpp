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
  #define _GET_EFFECT(ed, e, ename) \
    ed.effect     = &e; \
    ed.effectName = F(ename);
    
#else
  #define _GET_EFFECT(ed, e, ename) \
    ed.effect = &e;
#endif


#define GET_EFFECT(ed, EffectClass, ename) \
  { \
    static EffectClass e; \
    _GET_EFFECT(ed, e, ename); \
  }

bool getEffect(uint8_t effectId, EFFECT_DESCRIPTION &ed){
  
  ed.effectId = effectId;

  switch(effectId){
    case el_StaticColor:
      GET_EFFECT(ed, EffectStatic, "Static color");
    break;
    case el_Blur:
      GET_EFFECT(ed, EffectBlur, "Blur");
    break;
    case el_ColorWipe:
      GET_EFFECT(ed, EffectColorWipe, "Color wipe");
    break;
    case el_EmergencyLights:
      GET_EFFECT(ed, EffectEmergencyLights, "Emergancy lights");
    break;
    case el_FadeInOut:
      GET_EFFECT(ed, EffectFadeInOut, "Fade in out");
    break;
    case el_Juggle:
      GET_EFFECT(ed, EffectJuggle, "Juggle");
    break;
    case el_MeteorRain:
      GET_EFFECT(ed, EffectMeteorRain<>, "Meteor rain");      
    break;    
    case el_MoodBlobs:
      GET_EFFECT(ed, EffectMoodBlobs, "Mood blobs");      
    break;        
    case el_PaletteTransform:
      GET_EFFECT(ed, EffectPaletteTransformFast, "Palette transform");      
    break;        
    case el_Noise:
      GET_EFFECT(ed, EffectNoise, "Noise");      
    break;        
    case el_Plazma:
      GET_EFFECT(ed, EffectPlasma, "Plazma");      
    break;        
    case el_Confetti:
      GET_EFFECT(ed, EffectConfetti, "Confetti");      
    break;    
    case el_PacificOcean:
      GET_EFFECT(ed, EffectPacificOcean, "Pacific ocean");      
    break;    
    case el_Rainbow:
      GET_EFFECT(ed, EffectRainbow, "Rainbow");      
    break;    
    case el_TheaterChaseRainbow:
      GET_EFFECT(ed, EffectTheaterChaseRainbow, "Theater chase rainbow");      
    break;
    case el_RainbowMove:
      GET_EFFECT(ed, EffectRainbowMove, "Rainbow move");      
    break;    
    case el_Ripple:
      GET_EFFECT(ed, EffectRipple<>, "Ripple");      
    break;    
    case el_RunningLights:
      GET_EFFECT(ed, EffectRunningLights, "Running lights");      
    break;
    case el_TwinkleFox:
      GET_EFFECT(ed, EffectTwinkleFox, "Twinkle fox");      
    break;

    default:
    return false;
  }


  return true;
}
