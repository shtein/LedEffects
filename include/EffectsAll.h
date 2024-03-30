#ifndef __EFFECTS_ALL_H
#define __EFFECTS_ALL_H

#include "EffectEngineCtx.h"
class Effect;

/////////////////////////
//List of all effects
enum EffectList{
  el_None = 0,
  el_StaticColor,
  el_Blur,
  el_ColorWipe,
  el_EmergencyLights,
  el_FadeInOut,
  el_Juggle,   
  el_MeteorRain,
  el_MoodBlobs,
  el_PaletteTransform,
  el_Noise, 
  el_Plasma,
  el_Confetti,
  el_PacificOcean, 
  el_Rainbow,
  el_TheaterChaseRainbow,
  el_RainbowMove,
  el_Ripple,
  el_RunningLights, 
  el_TwinkleFox,
  el_Fire,
  el_Maxtrix_Drops,
  el_Matrix_Bouncing_Dots,
  el_Matrix_Circles,
  el_Total
};

struct EFFECT_DESCRIPTION{ 
  uint8_t   effectId;
  uint8_t   flags;
  Effect   *effect;        //Pointer to an effect  
#ifdef NTF_ENABLED  
  const char *effectName;  //Name of the effect, this is PROGMEM
#endif  
};

//Effect factory
uint8_t getEffectFlags(uint8_t effectId);
bool getEffect(uint8_t effectId, EFFECT_DESCRIPTION &ed);

#endif //__EFFECTS_ALL_H
