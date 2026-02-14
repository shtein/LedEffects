#ifndef __TWINKLE_FOX
#define __TWINKLE_FOX



///////////////////////////////////////
// EffectTwinkleFox

//Whtever it function does
inline uint8_t attackDecayWave8(uint8_t i){  
  return (i < 86) ? i * 3 : 255 - ((i - 86) + ((i - 86) / 2));
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


//getPal_TwinkleFox
class EffectTwinkleFox: public EffectPaletteTransform{
  
protected:
  void reset(){
    EffectPaletteTransform::reset();
    _ctx.rgb = _ctx.palCurrent[0]; //background
  }

  void proceed(CRGB *leds, uint16_t numLeds) {
    //Call parent 
    EffectPaletteTransform::proceed(leds, numLeds);

    //Default background is black
    CRGB bg = CRGB::Black;

    //If first two colors of the palletter are the same use it for backround
    if(_ctx.palCurrent[0] == _ctx.palCurrent[1] ){

      bg = _ctx.palCurrent[0];

      //Check how bright background is
      uint8_t bglight = bg.getAverageLight();
      bg.nscale8_video(bglight > 64 ? 16 : bglight > 16 ? 64 : 86);      
    }
    
    if(_ctx.rgb != bg){
      bg  = blend(_ctx.rgb, bg, 35);
      _ctx.rgb = bg;
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
      CRGB color = computeOneTwinkle( (uint32_t)((clk * speedMultiplier) >> 3) + clkOffs, rnd >> 8);

      //Proceed with setting color to current led
      int16_t deltabright = color.getAverageLight() - bg.getAverageLight();
      leds[i] = deltabright >= 32 || (!bg) ? color : deltabright > 0 ? blend( bg, color, deltabright * 8) : bg;
    }

  }

  CRGB computeOneTwinkle( uint32_t ms, uint8_t salt) const{
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
      color = ColorFromPalette( Effect::_ctx.palCurrent, slowcycle8 - salt, bright, NOBLEND);
      coolLikeIncandescent( color, fastcycle8);
    }
    return color;
  }
};



#endif //__TWINKLE_FOX