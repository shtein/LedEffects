#ifndef __PACIFIC_OCEAN_H
#define __PACIFIC_OCEAN_H

class EffectPacificOcean: public Effect{
protected:
  void reset();
  void proceed(CRGB *leds, uint16_t numLeds); 

  void oneLayer(CRGB *leds, 
                uint16_t numLeds, 
                const CRGBPalette16 &pal, 
                uint16_t cistart, 
                uint16_t wavescale, 
                uint8_t bri, 
                uint16_t ioff 
               );
  void addWhitecaps(CRGB *leds, uint16_t numLeds);
  void deepenColors(CRGB *leds, uint16_t numLeds);

protected:
}; 

//Reuse _ctx.palCurrent for local data members
#define SCIStart ((uint16_t *)_ctx.palCurrent.entries)


inline void EffectPacificOcean::reset(){
  for(size_t i = 0; i < 4; i++){
    SCIStart[i] = 0;
  }
  
  setSpeedDelay(20);
}

inline void EffectPacificOcean::proceed(CRGB *leds, uint16_t numLeds){
  // Increment the four "color index start" counters, one for each wave layer.
  // Each is incremented at a different speed, and the speeds vary over time.
  uint32_t deltams1     = (getSpeedDelay() * beatsin16(3, 179, 269)) / 256;
  uint32_t deltams2     = (getSpeedDelay() * beatsin16(4, 179, 269)) / 256;
  uint32_t deltams21    = (deltams1 + deltams2) / 2;

  SCIStart[0] += (deltams1 * beatsin88(1011, 10, 13));
  SCIStart[1] -= (deltams21 * beatsin88(777, 8, 11));
  SCIStart[2] -= (deltams1 * beatsin88(501, 5, 7));
  SCIStart[3] -= (deltams2 * beatsin88(257, 4, 6));

  // Clear out the LED array to a dim background blue-green
  fill_solid( leds, numLeds, CRGB( 2, 6, 10));

  // Render each of four layers, with different scales and speeds, that vary over time
  oneLayer( leds, numLeds, Pacifica_1_p, SCIStart[0], beatsin16( 3, 11 * 256, 14 * 256), beatsin8( 10, 70, 130), 0 - beat16( 301) );
  oneLayer( leds, numLeds, Pacifica_2_p, SCIStart[1], beatsin16( 4,  6 * 256,  9 * 256), beatsin8( 17, 40,  80), beat16( 401) );
  oneLayer( leds, numLeds, Pacifica_3_p, SCIStart[2], 6 * 256, beatsin8( 9, 10,38), 0 - beat16(503));
  oneLayer( leds, numLeds, Pacifica_3_p, SCIStart[3], 5 * 256, beatsin8( 8, 10,28), beat16(601));

  // Add brighter 'whitecaps' where the waves lines up more
  addWhitecaps(leds, numLeds);

  // Deepen the blues and greens a bit
  deepenColors(leds, numLeds);
}


inline void EffectPacificOcean::oneLayer( CRGB *leds, 
                                          uint16_t numLeds, 
                                          const CRGBPalette16 &pal, 
                                          uint16_t cistart, 
                                          uint16_t wavescale, 
                                          uint8_t bri, 
                                          uint16_t ioff 
                                        ) {

  uint16_t ci             = cistart;
  uint16_t waveangle      = ioff;
  uint16_t wavescale_half = (wavescale / 2) + 20;

  for( uint16_t i = 0; i < numLeds; i++) {
    waveangle += 250;

    uint16_t s16 = sin16( waveangle ) + 32768;
    uint16_t cs  = scale16( s16 , wavescale_half ) + wavescale_half;

    ci += cs;

    uint16_t sindex16 = sin16( ci) + 32768;
    uint8_t sindex8   = scale16( sindex16, 240);

    leds[i] += ColorFromPalette( pal, sindex8, bri, LINEARBLEND);    
  }
}

// Add extra 'white' to areas where the four layers of light have lined up brightly
inline void EffectPacificOcean::addWhitecaps(CRGB *leds, uint16_t numLeds){

  uint8_t basethreshold = beatsin8( 9, 55, 65);
  uint8_t wave          = beat8( 7 );
  
  for( uint16_t i = 0; i < numLeds; i++) {

    uint8_t threshold = scale8( sin8( wave), 20) + basethreshold;
    wave += 7;

    uint8_t l = leds[i].getAverageLight();

    if( l > threshold) {
      uint8_t overage  = l - threshold;
      uint8_t overage2 = qadd8( overage, overage);
      leds[i]         += CRGB( overage, overage2, qadd8( overage2, overage2));
    }
  }
}

// Deepen the blues and greens
inline void EffectPacificOcean::deepenColors(CRGB *leds, uint16_t numLeds){

  for(uint16_t i = 0; i < numLeds; i++) {
    leds[i].blue  = scale8( leds[i].blue,  145); 
    leds[i].green = scale8( leds[i].green, 200); 
    leds[i]      |= CRGB( 2, 5, 7);
  }
}

#endif //__PACIFIC_OCEAN_H