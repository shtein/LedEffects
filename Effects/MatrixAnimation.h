#ifndef __MATRIX_ANIMATION_H
#define __MATRIX_ANIMATION_H

///////////////////////////////////////
// Point
struct Pnt {
  int8_t x;
  int8_t y;

  inline Pnt() __attribute__((always_inline)) = default;
  inline Pnt(int8_t x0, int8_t y0) __attribute__((always_inline)): x(x0), y(y0)
  {
  };

  inline Pnt& operator= (const Pnt& pnt) __attribute__((always_inline)) = default;

  inline Pnt mirror(const Pnt &m) const __attribute__((always_inline)) {
    return Pnt( MIRROR(x, m.x), MIRROR(y, m.y));
  }
  
};

inline Pnt operator+ (const Pnt& pnt1, const Pnt& pnt2)  {
  return Pnt(pnt1.x + pnt2.x, pnt1.y + pnt2.y);
};

inline Pnt operator-(const Pnt& pnt1, const Pnt& pnt2){
  return Pnt(pnt1.x - pnt2.x, pnt1.y - pnt2.y);
};




///////////////////////////////////////
// Object
struct Obj {
  Pnt pos;
  Pnt vel;

  inline Obj()  __attribute__((always_inline)) = default;
  inline Obj (const Pnt &pos0, const Pnt &vel0)  __attribute__((always_inline)): 
    pos(pos0), vel(vel0){
  }
  inline Obj (int8_t x0, int8_t y0, int8_t vx0, int8_t vy0)  __attribute__((always_inline)):
    pos(x0, y0), vel(vx0, vy0)
  {
  }

  inline Obj& operator= (const Obj &)  __attribute__((always_inline)) = default;

};


///////////////////////////////////////
// Move linear routine
void moveLinear(Obj &obj, uint8_t t){

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

    setSpeedDelay(80);

    for(int i = 0; i < MAX_OBJECTS; i++){
      _drops[i].obj.pos = Pnt(127, 127);
      _drops[i].obj.vel = Pnt(0, 1);
    }
   
  };

protected:
  inline void updateLeds(CRGB *leds, int numLeds) __attribute__((always_inline))
  {
      //Fade all
    fadeToBlackBy(leds, numLeds, MATRIX_OBJECTS_FADE);  

    XYDraw xy(leds, numLeds, XY_DRAW_ADD_COLORS);

    //Proceed with objects
    for(int i = 0; i < MAX_OBJECTS; i++){
      Drop &drop = _drops[i];
      Pnt  &pos  = drop.obj.pos;


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
    uint8_t colorIndex;
    Obj     obj;
  } _drops[MAX_OBJECTS];
};


////////////////////////////////////////
// EffectMatrixCircles

template <const int MAX_OBJECTS = 5>
class EffectMatrixCircles: public EffectPaletteTransform{
public:
  inline EffectMatrixCircles(FuncGetPalette_t getPal = &FuncGetPal_Default) __attribute__((always_inline)):
    EffectPaletteTransform(getPal) {

      setSpeedDelay(80);

      for(int i = 0; i < MAX_OBJECTS; i++){
        _circles[i].obj.pos = Pnt(127, 127);
        _circles[i].obj.vel = Pnt(1, 1);
        _circles[i].radius  = 127;
    }   

  }
  

protected:
  inline void updateLeds(CRGB *leds, int numLeds) __attribute__((always_inline)){
    //Fade all
    fadeToBlackBy(leds, numLeds, MATRIX_OBJECTS_FADE );  

    XYDraw xy(leds, numLeds, XY_DRAW_ADD_COLORS);

    //Proceed with objects
    for(int i = 0; i < MAX_OBJECTS; i++){
      
      Circle &circle = _circles[i];
      Pnt    &pos    = circle.obj.pos;


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
    Obj     obj;    
  } _circles[MAX_OBJECTS];

};


template <class T> 
class EffectMatrixKaleidoscope: public T{

  protected:
  inline void proceed(CRGB *leds, int numLeds){
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



#endif //__MATRIX_ANIMATION_H