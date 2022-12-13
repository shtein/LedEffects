#ifndef __MATRIX_ANIMATION_H
#define __MATRIX_ANIMATION_H



///////////////////////////////////////
// Move linear routine
template<typename T>
void moveLinear(Obj<T> &obj, int16_t t){

  //Change position, speed does not change
  obj.pos.x += t * obj.vel.x;
  obj.pos.y += t * obj.vel.y;
}

////////////////////////////////////////
// Move gravity to the bottom routine

#define G_CONST 20

template<typename T>
void moveGravity(Obj<T> &obj, int16_t t){  

  obj.pos.x =                       obj.vel.x * t + obj.pos.x;
  obj.pos.y = t * t * G_CONST / 2 + obj.vel.y * t + obj.pos.y;

  obj.vel.y +=  G_CONST * t;
  
}

///////////////////////////////////////
// Move linear routine
template<typename T>
void moveLinear(Obj<T> &obj, uint8_t t){

  //Change position, speed does not change
  obj.pos.x += t * obj.vel.x;
  obj.pos.y += t * obj.vel.y;
}




#define MATRIX_OBJECTS_FADE 128



////////////////////////////////////////
// EffectMatrixDrops

template <const int MAX_OBJECTS = 5>
class EffectMatrixDrops: public EffectPaletteTransform{
public:
  inline EffectMatrixDrops(FuncGetPalette_t getPal = &FuncGetPal_Default) __attribute__((always_inline)):
    EffectPaletteTransform(getPal) {    
  };

protected:
  void reset(){
    EffectPaletteTransform::reset();

    for(int i = 0; i < MAX_OBJECTS; i++){
      _drops[i].obj.pos = Pnt8_t(127, 127);
      _drops[i].obj.vel = Pnt8_t(0, 1);
    }

    setSpeedDelay(80);
  }

  inline void proceed(CRGB *leds, uint16_t numLeds) __attribute__((always_inline))
  {
    EffectPaletteTransform::proceed(leds, numLeds);

      //Fade all
    fadeToBlackBy(leds, numLeds, MATRIX_OBJECTS_FADE);  

    XYDraw xy(leds, numLeds, XY_DRAW_ADD_COLORS);

    //Proceed with objects
    for(int i = 0; i < MAX_OBJECTS; i++){
      Drop  &drop  = _drops[i];
      Pnt8_t &pos  = drop.obj.pos;


      //Is over
      if(pos.y > random8(xy.height(), xy.height() + xy.height() / 4)){

        //Init
        pos.x           = random8(i * xy.width() / MAX_OBJECTS, (i + 1) * xy.width() / MAX_OBJECTS );
        pos.y           = random8(xy.height() / 2);
        drop.colorIndex = random8();
      }

      //Draw
      CRGB crl = getCurrentPalColor(drop.colorIndex); 
      xy.pixel(pos.x, pos.y, crl);

      //Move    
      moveLinear(drop.obj, 1);
    }
  }
  

protected:
  struct Drop{
    uint8_t         colorIndex;
    Obj8_t          obj;
  } _drops[MAX_OBJECTS];
};


////////////////////////////////////////
// EffectMatrixCircles
template <const int MAX_OBJECTS = 5>
class EffectMatrixCircles: public EffectPaletteTransform{
public:
  inline EffectMatrixCircles(FuncGetPalette_t getPal = &FuncGetPal_Default) __attribute__((always_inline)):
    EffectPaletteTransform(getPal) { 
  }
  

protected:
  void reset(){
    EffectPaletteTransform::reset();

    for(int i = 0; i < MAX_OBJECTS; i++){
        _circles[i].obj.pos = Pnt8_t(127, 127);
        _circles[i].obj.vel = Pnt8_t(1, 1);
        _circles[i].radius  = 127;
    }   

    setSpeedDelay(80);
  }

  
  inline void proceed(CRGB *leds, uint16_t numLeds) __attribute__((always_inline)){
    EffectPaletteTransform::proceed(leds, numLeds);

    //Fade all
    fadeToBlackBy(leds, numLeds, MATRIX_OBJECTS_FADE );  

    XYDraw xy(leds, numLeds, XY_DRAW_ADD_COLORS);

    //Proceed with objects
    for(int i = 0; i < MAX_OBJECTS; i++){
      
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
  } _circles[MAX_OBJECTS];
};


template <class T> 
class EffectMatrixKaleidoscope: public T{

protected:
  inline void proceed(CRGB *leds, uint16_t numLeds){
    T::proceed(leds, numLeds);
    
    XYDraw xy(leds, numLeds);

    

    xy.mirrorRightTriangleButterfly(0, xy.height() / 2 - 1, 
                                    xy.width() / 2, -xy.height() / 2,
                                    xy.width() / 2 - 1, -xy.height() / 2 + 1
                                  );
    xy.mirrorRectangleHorizontally(0, 0, xy.width()/2, xy.height()/2, xy.width()/2, xy.width() % 2 - 1);
    xy.mirrorRectangleVertically(0, 0, xy.width(), xy.height() / 2, xy.height() / 2, 0, xy.height() % 2 - 1);
  };

};

///////////////////////////////////////////////
//EffectMatrixBounsingDots

class EffectMatrixBounsingDots: public Effect{
public:
  inline EffectMatrixBounsingDots() __attribute__((always_inline)) {      
  };


protected:
  void reset(){
    _obj.pos = Pnt32_t(0, 0);
    _obj.vel = Pnt32_t(1000, 0);
    setSpeedDelay(20);  
  };

  void proceed(CRGB *leds, uint16_t numLeds){

    fadeToBlackBy(leds, numLeds, MATRIX_OBJECTS_FADE/4 );

    XYDraw xy(leds, numLeds);

    //DBG_OUTLN("%ld %ld", _obj.pos.x >> 16, _obj.pos.y >> 16);

    
    if(_obj.movesAwayLeft(0)){
      _obj.vel.x = -_obj.vel.x;
    }

    if(_obj.movesAwayRight((int32_t)xy.width() << 16)){
      _obj.vel.x = -_obj.vel.x;
    }

    if(_obj.movesAwayUp(0)){
      _obj.vel.y = -_obj.vel.y;    
    }

    if(_obj.movesAwayDown((int32_t)xy.height() << 16)){
      _obj.vel.y = -_obj.vel.y;      
    }

    //xy(_obj.pos.x >> 8, _obj.pos.y >> 8) = CRGB::Black;

    moveGravity(_obj, 20);
    //moveLinear(_obj, 20);

    xy(_obj.pos.x >> 16, _obj.pos.y >> 16) = CRGB::Blue;
  };

protected:
  Obj32_t _obj;
};


#endif //__MATRIX_ANIMATION_H