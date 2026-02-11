#ifndef __MATRIX_ANIMATION_H
#define __MATRIX_ANIMATION_H





#define MATRIX_OBJECTS_FADE 128

////////////////////////////////////////
// EffectMatrixDrops

///////////////////////////////////////
// Move linear routine
template<typename T>
void moveLinear(Obj<T> &obj, int16_t t){

  //Change position, speed does not change
  obj.pos.x += t * obj.vel.x;
  obj.pos.y += t * obj.vel.y;

}


#ifndef MAXTRIX_DROPS_MAX_OBJECTS
  #define MAXTRIX_DROPS_MAX_OBJECTS 8
#endif  

class EffectMatrixDrops: public EffectPaletteTransform{

protected:
   void reset(){
    EffectPaletteTransform::reset();

    for(int i = 0; i < MAXTRIX_DROPS_MAX_OBJECTS; i++){
      _drops[i].obj.pos = Pnt8_t(127, 127);
      _drops[i].obj.vel = Pnt8_t(0, 1);
    }
#ifdef USE_SOUND
    setSpeedDelay(10);
#else
    setSpeedDelay(80);
#endif //USE_SOUND
  }

  void proceed(CRGB *leds, uint16_t numLeds) 
  {    
    EffectPaletteTransform::proceed(leds, numLeds);
    
#ifdef USE_SOUND
    //Get sound data
    sc_band_t bands;
    getSoundBands(bands, false);

    bool silence = !(_cfg.flags & ECF_SOUND) || _sc->isSound(false, 5000);

    _ctx.byte ++;
    if(_ctx.byte * getSpeedDelay() < 80){
      //Skip this time
      return;
    }
    _ctx.byte = 0;

    fadeToBlackBy(leds, numLeds, MATRIX_OBJECTS_FADE);  

    bool treble = _sc->isTreblePeak() && TREBLE_BEAT_CHECK() >= 50;
    bool mid    = _sc->isMidPeak() && MID_BEAT_CHECK() >= 100;
    bool bass   = _sc->isBassPeak() && BASS_BEAT_CHECK() >= 150; 

    if(bass || mid || /*treble ||*/ silence){

#else
    fadeToBlackBy(leds, numLeds, MATRIX_OBJECTS_FADE);        
#endif    
    XYDraw xy(leds, numLeds, XY_DRAW_ADD_COLORS); 

    //Proceed with objects
    for(int i = 0; i < MAXTRIX_DROPS_MAX_OBJECTS; i++){
      Drop  &drop  = _drops[i];
      Pnt8_t &pos  = drop.obj.pos;

      //Is over
      if(pos.y > random8(xy.height(), xy.height() + xy.height() / 4)){

        //Init
        pos.x           = random8(i * xy.width() / MAXTRIX_DROPS_MAX_OBJECTS, (i + 1) * xy.width() / MAXTRIX_DROPS_MAX_OBJECTS );
        pos.y           = random8(xy.height() / 2);
        drop.colorIndex = random8();
      }

      //Draw
      CRGB crl = getCurrentPalColor(drop.colorIndex); 
      xy.pixel(pos.x, pos.y, crl);

      //Move    
      moveLinear(drop.obj, 1);
    }

#ifdef USE_SOUND
      if(treble) TREBLE_BEAT_RESET();
      if(mid) MID_BEAT_RESET();
      if(bass) BASS_BEAT_RESET();
    } //End of sound check
#endif //USE_SOUND
  }

  

protected:
  struct Drop{
    uint8_t         colorIndex;
    Obj8_t          obj;
  } _drops[MAXTRIX_DROPS_MAX_OBJECTS];
};



////////////////////////////////////////
// EffectMatrixCircles
#ifndef MAXTRIX_CIRCLES_MAX_OBJECTS
  #define MAXTRIX_CIRCLES_MAX_OBJECTS 2
#endif  

class EffectMatrixCircles: public EffectPaletteTransform{
public:

  void reset(){
    EffectPaletteTransform::reset();

    for(int i = 0; i < MAXTRIX_CIRCLES_MAX_OBJECTS; i++){
        _circles[i].obj.pos = Pnt8_t(127, 127);
        _circles[i].obj.vel = Pnt8_t(1, 1);
        _circles[i].radius  = 127;
    }   

    setSpeedDelay(80);
  }

  
  void proceed(CRGB *leds, uint16_t numLeds) {
    EffectPaletteTransform::proceed(leds, numLeds);

    //Fade all
    fadeToBlackBy(leds, numLeds, MATRIX_OBJECTS_FADE );  

    XYDraw xy(leds, numLeds, XY_DRAW_ADD_COLORS);

    //Proceed with objects
    for(int i = 0; i < MAXTRIX_CIRCLES_MAX_OBJECTS; i++){
      
      Circle &circle  = _circles[i];
      Pnt8_t &pos     = circle.obj.pos;


      //Time is over
      if( circle.radius > random8(xy.height() / 2, xy.height())){

        //Init
        pos.x             = random8(xy.width() / 2);
        pos.y             = random8(xy.height() / 2);
        circle.radius     = 1;
        circle.colorIndex = random8();
      }

      //Draw
      CRGB crl = getCurrentPalColor(circle.colorIndex, cos8(map(circle.radius, 0, xy.height(), 0, 172))); 
      xy.circle(pos.x, pos.y, circle.radius, crl);

      //Move
      moveLinear(circle.obj, 1);
      circle.radius ++;
    }

  };

protected:

  struct Circle{
    uint8_t colorIndex;
    uint8_t radius;
    Obj8_t  obj;    
  } _circles[MAXTRIX_CIRCLES_MAX_OBJECTS];
};


///////////////////////////////////////////////
//EffectMatrixBounsingDots

////////////////////////////////////////
// Move gravity to the bottom routine



// How to calculate gc:
// hight - number of pixels in height
// totalTime - total time of falling from hight to 0 in seconds
// t - time step in ticks
// tps - ticks per second
// T gc = (2 * (hight << (sizeof(T) * 8 / 2))) / (totalTime * totalTime * tps * tps);
//
// for example, T is int16_t for hight = 16, totalTime = 1s, t = 1, tps = 50
// g = (2 * (16 << 8) / 2 ) / (1 * 1 * 50 * 50)  = 8192 / 2500 = 3.2 = 3
// use 4 for better rounding effect



#define G_CONST 4

template<typename T>
void moveGravity(Obj<T> &obj, int16_t t){  
  //Change position
  obj.pos.x =  obj.vel.x * t + obj.pos.x;
  obj.pos.y = t * t * G_CONST / 2 + obj.vel.y * t + obj.pos.y;

  //Change speed
  obj.vel.y += G_CONST * t;
}


//////////////////////////////
// Bouncing in 1D
template<typename T>
void bounce1d(T &v1, uint8_t m1, T &v2, uint8_t m2){
  //Total mass
  T M = (T)m1 + (T)m2;

  //New speeds
  T u1 = divRound<T>((m1 - m2) * v1 + 2 * m2 * v2, M);
  T u2 = divRound<T>((m2 - m1) * v2 + 2 * m1 * v1, M);

  //Update speeds
  v1 = u1;
  v2 = u2;
}


template<typename T>
void bounce2d(Obj<T> &obj1, uint8_t m1, Obj<T> &obj2, uint8_t m2){
  using V = upper_type_t<T>; // wider integer for intermediates

  //Normal vector
  Pnt<T> n = obj2.pos - obj1.pos;

  //Tangent ortogonal vector
  Pnt<T> t = n.ortogonal();

  //Project speeds to normal and ortogonal vectors
  Pnt<V> u1(obj1.vel * n, obj1.vel * t);
  Pnt<V> u2(obj2.vel * n, obj2.vel * t);

  //Bounce speeds in normal direction
  bounce1d<V>(u1.x, m1, u2.x, m2);
 
  // len * len
  V len2 = n * n;

  //Update velocities
  obj1.vel.x = (T)divRound<V>(u1.x * n.x + u1.y * t.x, len2);
  obj1.vel.y = (T)divRound<V>(u1.x * n.y + u1.y * t.y, len2);

  obj2.vel.x = (T)divRound<V>(u2.x * n.x + u2.y * t.x, len2);
  obj2.vel.y = (T)divRound<V>(u2.x * n.y + u2.y * t.y, len2);
}


#ifndef MATRIX_BOUNCING_DOTS_MAX_OBJECTS
  #define MATRIX_BOUNCING_DOTS_MAX_OBJECTS 5
#endif

#define I2FP(v) ((int16_t)((v) << 8))
#define FP2I(v) ((int8_t)((v) >> 8))



class EffectMatrixBounsingDots: public EffectPaletteTransform{
public:
  void reset(){
    EffectPaletteTransform::reset();

    XY xy;

    for(size_t i = 0; i < MATRIX_BOUNCING_DOTS_MAX_OBJECTS; i++){
      _dots[i].colorIndex = random8();
      _dots[i].mass       = (i + 1) * 2;
      _dots[i].obj.pos    = Pnt16_t(random16(I2FP(xy.width() - 1)), random16(0, I2FP(xy.height() / 2 - 1)));
      _dots[i].obj.vel    = Pnt16_t(random16(64, 192) * (1 - random8(1) * 2), 0);      
    }
    
    setSpeedDelay(20);  
  };

  void proceed(CRGB *leds, uint16_t numLeds) {

    EffectPaletteTransform::proceed(leds, numLeds);
    
    fadeToBlackBy(leds, numLeds, MATRIX_OBJECTS_FADE );

    XYDraw xy(leds, numLeds, XY_DRAW_ADD_COLORS);

    //Boundaries
    int16_t xmin = 0;
    int16_t xmax = I2FP(xy.width() - 1);
    int16_t ymin = 0;
    int16_t ymax = I2FP(xy.height() - 1);

    int16_t dist = I2FP(1);

    for(size_t i = 0; i < MATRIX_BOUNCING_DOTS_MAX_OBJECTS; i++){
      Obj16_t &obj = _dots[i].obj;

      //Check for bounce  with other objects
      for(size_t j = i + 1; j < MATRIX_BOUNCING_DOTS_MAX_OBJECTS; j++){
        Obj16_t &obj2 = _dots[j].obj;

        if(obj.collides(obj2, dist)){                              
          bounce2d<int16_t>(obj, _dots[i].mass, obj2, _dots[j].mass);          
        }       
      }
    
    
      //Check for bounce with boundaries
      if(obj.movesAwayLeft(xmin)){
        obj.vel.x = -obj.vel.x;
      }

      if(obj.movesAwayRight(xmax)){
        obj.vel.x = -obj.vel.x;
      }

      if(obj.movesAwayUp(ymin)){
        obj.vel.y = -obj.vel.y;    
      }

      if(obj.movesAwayDown(ymax)){
        obj.vel.y = -obj.vel.y;      
      }          

      moveGravity(obj, 1);


      //Draw dot
      int8_t x = FP2I(obj.pos.x);
      int8_t y = FP2I(obj.pos.y);
      
      xy.pixel(x, y, ColorFromPalette(_ctx.palCurrent, _dots[i].colorIndex, 255, LINEARBLEND));
    }

  
  };

protected:  
  struct {  
    uint8_t colorIndex;    
    uint8_t mass;
    Obj16_t obj;
  } _dots[MATRIX_BOUNCING_DOTS_MAX_OBJECTS];
};





#endif //__MATRIX_ANIMATION_H