#ifndef __TWINKLE_FOX
#define __TWINKLE_FOX


////////////////////////////////////
// TwinkleFox palette transformations 

BEGIN_TRANSFORM_SCHEMA_RGB16_PALETTE(TwinkleFox)
  TRANSOFRM_PALETTE(RetroC9_p)
  TRANSOFRM_PALETTE(BlueWhite_p)
  TRANSOFRM_PALETTE(RainbowColors_p)
  TRANSOFRM_PALETTE(FairyLight_p)
  TRANSOFRM_PALETTE(RedGreenWhite_p)
  TRANSOFRM_PALETTE(PartyColors_p)
  TRANSOFRM_PALETTE(RedWhite_p)
  TRANSOFRM_PALETTE(Snow_p)
  TRANSOFRM_PALETTE(Holly_p)
  TRANSOFRM_PALETTE(Ice_p)
END_TRANSFORM_SCHEMA()

BEGIN_TRANSFORM_SCHEMA_RGB16_PALETTE(SnowAndIce)
  TRANSOFRM_PALETTE(BlueWhite_p)
  TRANSOFRM_PALETTE(Snow_p)
  TRANSOFRM_PALETTE(Ice_p)
END_TRANSFORM_SCHEMA()

///////////////////////////////////////
// EffectTwinkleFox
class EffectTwinkleFox: public EffectPaletteTransform{
public:
  EffectTwinkleFox(FuncGetPalette_t getPal = &TwinkleFox);

protected:
  void reset();
  void updateLeds(CRGB *leds, uint16_t numLeds);

  CRGB computeOneTwinkle( uint32_t ms, uint8_t salt) const;

private:
  CRGB _bg;   //Last remembered background color
};

inline EffectTwinkleFox::EffectTwinkleFox(FuncGetPalette_t getPal): EffectPaletteTransform(getPal){
}

inline void EffectTwinkleFox::reset(){
  EffectPaletteTransform::reset();

  _bg = _ctx.palCurrent[0];
}

inline void EffectTwinkleFox::updateLeds(CRGB *leds, uint16_t numLeds){  
  
  //Default background is black
  CRGB bg = CRGB::Black;
  

  //If first two colors of the palletter are the same use it for backround
  if(_ctx.palCurrent[0] == _ctx.palCurrent[1] ){

    bg = _ctx.palCurrent[0];

    //Check how bright background is
    uint8_t bglight = bg.getAverageLight();

    if( bglight > 64) {
      bg.nscale8_video(16); // very bright
    } 
    else if( bglight > 16) {
      bg.nscale8_video(64); // not that bright
    } 
    else {
      bg.nscale8_video(86); // dim
    }       
  }
  
  if(_bg != bg){
    bg  = blend(_bg, bg, 35);
    _bg = bg;
  }  

  //Init pseudo random elements
  uint16_t rnd   = 11337;
  uint32_t clk   = millis();

  //Proceed with leds
  for(uint16_t i = 0; i < numLeds; i++){

    //Whatever this magic is
    rnd = (uint16_t)(rnd * 2053) + 1384; 
    uint16_t clkOffs = rnd;   
    rnd = (uint16_t)(rnd * 2053) + 1384; 
    uint8_t speedMultiplier =  ((((rnd & 0xFF) >> 4) + (rnd & 0x0F)) & 0x0F) + 0x08;

    //Get new color
    CRGB color = computeOneTwinkle( (uint32_t)((clk * speedMultiplier) >> 3) + clkOffs, 
                                    rnd >> 8
                                  );

    //Proceed with setting color to current led
    int16_t deltabright = color.getAverageLight() - bg.getAverageLight();

    if( deltabright >= 32 || (!bg)) {
      leds[i] = color;
    } else 
    if( deltabright > 0 ) {
      leds[i] = blend( bg, color, deltabright * 8);
    } else { 
      leds[i] = bg;
    }
  }
 
}


//Whtever it function does
inline uint8_t attackDecayWave8( uint8_t i){
  if( i < 86) {
    return i * 3;
  } 
  else {
    i -= 86;
    return 255 - (i + (i / 2));
  }
}

//Whtever it function does
inline void coolLikeIncandescent( CRGB& c, uint8_t phase){
  if( phase < 128) 
    return;

  uint8_t cooling = (phase - 128) >> 4;
  c.g = qsub8(c.g, cooling);
  c.b = qsub8(c.b, cooling * 2);
}

#define TWINKLE_SPEED   4
#define TWINKLE_DENSITY 5

inline CRGB EffectTwinkleFox::computeOneTwinkle( uint32_t ms, uint8_t salt) const
{
  //Another magic here
  uint16_t ticks       = ms >> (8 - TWINKLE_SPEED);
  uint8_t fastcycle8   = ticks;
  uint16_t slowcycle16 = (ticks >> 8) + salt;

  slowcycle16 += sin8( slowcycle16);
  slowcycle16 = (slowcycle16 * 2053) + 1384;

  uint8_t slowcycle8   = (slowcycle16 & 0xFF) + (slowcycle16 >> 8);  
  uint8_t bright       = 0;

  if( ((slowcycle8 & 0x0E)/2) < TWINKLE_DENSITY) {
    bright = attackDecayWave8(fastcycle8);
  }

  CRGB color = CRGB::Black;
  if( bright > 0) {
    color = ColorFromPalette( _ctx.palCurrent, slowcycle8 - salt, bright, NOBLEND);
    coolLikeIncandescent( color, fastcycle8);
  }
  return color;
}







#endif //__TWINKLE_FOX