
#include <stdio.h>
#include <unity.h>
#include <alutils.h>
#include "MatrixUtils.h"



const char *strMsgVert = "Move away %s check fails -- vertical: %d, position: %d, speed: %d";

void testObjMoveAwayLeft(){    

  Obj<int8_t> obj;  
  const uint8_t vert = 5;
  char msg[128];

  for(int pos = 0; pos < 3; pos++){ //left side, on, right side of the vertical
    obj.pos.x = vert - pos - 1;

    for(int vel = 0; vel < 3; vel++){ //moves to the left, stays, to the right      
            
      obj.vel.x  = vel - 1;
      snprintf(msg, sizeof(msg), strMsgVert, "left", vert, obj.pos.x, obj.vel.x);

      //TRUE is only if on or on the left and is moving to the left
      if(obj.pos.x <= vert && obj.vel.x < 0){
        TEST_ASSERT_TRUE_MESSAGE(obj.movesAwayLeft(vert), msg); 
      }        
      else{
        TEST_ASSERT_FALSE_MESSAGE(obj.movesAwayLeft(vert), msg); 
      }
    }
  }
  
}

void testObjMoveAwayRight(){    

  Obj<int8_t> obj;  
  const uint8_t vert = 5;
  char msg[128];

  for(int pos = 0; pos < 3; pos++){ //left side, on, right side of the vertical
    obj.pos.x = vert - pos - 1;

    for(int vel = 0; vel < 3; vel++){ //moves to the left, stays, to the right      
            
      obj.vel.x  = vel - 1;
      snprintf(msg, sizeof(msg), strMsgVert, "right", vert, obj.pos.x, obj.vel.x);

      //TRUE is only if on or on the righ and is moving to the right
      if(obj.pos.x >= vert && obj.vel.x > 0){
        TEST_ASSERT_TRUE_MESSAGE(obj.movesAwayRight(vert), msg); 
      }        
      else{
        TEST_ASSERT_FALSE_MESSAGE(obj.movesAwayRight(vert), msg); 
      }
    }
  }  
}

void testObjMoveAwayUp(){    

  Obj<int8_t> obj;  
  const uint8_t horiz = 5;
  char msg[128];

  for(int pos = 0; pos < 3; pos++){ //top side, on, bottom side of the horizontal
    obj.pos.y = horiz - pos - 1;

    for(int vel = 0; vel < 3; vel++){ //moves to the top, stays, to up
            
      obj.vel.y  = vel - 1;
      snprintf(msg, sizeof(msg), strMsgVert, "up", horiz, obj.pos.y, obj.vel.y);

      //TRUE is only if on or on the top and is moving to the up
      if(obj.pos.y <= horiz && obj.vel.y < 0){
        TEST_ASSERT_TRUE_MESSAGE(obj.movesAwayUp(horiz), msg); 
      }        
      else{
        TEST_ASSERT_FALSE_MESSAGE(obj.movesAwayUp(horiz), msg); 
      }
    }
  }  
}

void testObjMoveAwayDown(){    

  Obj<int8_t> obj;  
  const uint8_t horiz = 5;
  char msg[128];

  for(int pos = 0; pos < 3; pos++){ //top side, on, bottom side of the horizontal
    obj.pos.y = horiz - pos - 1;

    for(int vel = 0; vel < 3; vel++){ //moves to the top, stays, to the bottom
            
      obj.vel.y  = vel - 1;
      snprintf(msg, sizeof(msg), strMsgVert, "down", horiz, obj.pos.y, obj.vel.y);

      //TRUE is only if on or on the bottom and is moving to down
      if(obj.pos.y >= horiz && obj.vel.y > 0){
        TEST_ASSERT_TRUE_MESSAGE(obj.movesAwayDown(horiz), msg); 
      }        
      else{
        TEST_ASSERT_FALSE_MESSAGE(obj.movesAwayDown(horiz), msg); 
      }
    }
  }  
}

void testu8Sqrt(){  
  struct {    
    int8_t  out;
    int16_t in;    
  } data[] = {{0, 0}, {1, 1}, {2, 4}, {3, 9}, {4, 16}, {5, 25}, {15, 225}, {16, 256}};
  
  for(size_t i = 0; i < sizeof(data)/sizeof(data[0]); i++)
    TEST_ASSERT_EQUAL_INT8(data[i].out, uSqrt<int8_t>(data[i].in));
}

void testu16Sqrt(){
  struct {    
    int16_t out;
    int32_t in;    
  } data[] = { {0, 0}, {1, 1}, {2, 4}, {3, 9}, {4, 16}, {5, 25}, {15, 225}, {16, 256},
               {32, 1024}, {100, 10000}, {128, 16384}, {255, 65025}, 
               {2, 5}, {3, 10}, {7, 50}, {11, 125}, {22, 500}, {70, 5000}, {141, 20000}, {253, 64156}, {255, 65535}
             };

  for(size_t i = 0; i < sizeof(data)/sizeof(data[0]); i++)
    TEST_ASSERT_EQUAL_INT16(data[i].out, uSqrt<int16_t>(data[i].in));

}


void testBounce1d(){

  struct {
    int16_t v1;
    int16_t m1;
    int16_t v2;
    int16_t m2;
    int16_t v1After;
    int16_t v2After;
  } testData[] = {
      // 1. Equal masses: Full velocity swap
      {5, 1, -2, 1, -2, 5},
      
      // 2. Heavy hits light stationary (m1=3, m2=1): Heavy slows, light zooms
      {400, 3, 0, 1, 200, 600},
      
      // 3. Light hits heavy stationary (m1=1, m2=3): Light bounces back
      {400, 1, 0, 3, -200, 200},
      
      // 4. Head-on collision (m1=2, m2=3): Both reverse directions
      {100, 2, -100, 3, -140, 60},
      
      // 5. Catch-up (m1=5, m2=3): Object 1 hits Object 2 from behind
      {160, 5, 80, 3, 100, 180},
      
      // 6. High mass ratio (m1=7, m2=1): Small change for heavy, large for light
      {200, 7, 0, 1, 150, 350},
      
      // 7. Perfect symmetry (m1=2, m2=2): Opposite speeds
      {500, 2, -500, 2, -500, 500},
      
      // 8. Large velocities (m1=1, m2=4): Testing near int16 limits
      {1000, 1, 0, 4, -600, 400}
 };



  for(size_t i = 0; i < sizeof(testData) / sizeof(testData[0]); i++){  
    
    bounce1d<int16_t>(testData[i].v1, testData[i].m1, testData[i].v2, testData[i].m2);

    TEST_ASSERT_EQUAL_INT16(testData[i].v1After, testData[i].v1);
    TEST_ASSERT_EQUAL_INT16(testData[i].v2After, testData[i].v2);
  }
}


void testBounce2d(){
  struct {
    Obj<int16_t> obj1;
    int16_t m1;
    Obj<int16_t> obj2;
    int16_t m2;
    Pnt<int16_t> vel1After;
    Pnt<int16_t> vel2After;
} testData[] = {
    // 1. HIGH SPEED SWAP (Equal mass, head-on)
    // Speed: 1000 and 800 -> Swap
    { Obj<int16_t>(0, 0, 1000, 0), 10, Obj<int16_t>(20, 0, -800, 0), 10, Pnt<int16_t>(-800, 0), Pnt<int16_t>(1000, 0)},

    // 2. SLOW SPEED BOUNCE (Light hits Heavy)
    // m1=1, m2=3. Speed: 40 hits stationary. 
    // Formulas: u1 = (1-3)/(1+3)*40 = -20 | u2 = (2*1)/(1+3)*40 = 20
    { Obj<int16_t>(0, 0, 40, 0), 1, Obj<int16_t>(10, 0, 0, 0), 3, Pnt<int16_t>(-20, 0), Pnt<int16_t>(20, 0)},

    // 3. 2D "3-4-5" TRIANGLE IMPACT (Equal mass)
    // Position offset is (3, 4), so Normal vector is (0.6, 0.8).
    // Obj1 speed 500 hits stationary Obj2.
    // Result: Obj1 deflects at 90 deg, Obj2 moves along normal.
    { 
        Obj<int16_t>(0, 0, 500, 0), 5, 
        Obj<int16_t>(30, 40, 0, 0), 5, 
        Pnt<int16_t>(320, -240), // Obj1 new velocity
        Pnt<int16_t>(180, 240)   // Obj2 new velocity
    },

    // 4. HEAVY SPEEDER (m1=4, m2=1)
    // Speed: 500 hits stationary.
    // u1 = (4-1)/(4+1)*500 = 300 | u2 = (2*4)/(4+1)*500 = 800
    { Obj<int16_t>(0, 0, 500, 0), 4, Obj<int16_t>(10, 0, 0, 0), 1, Pnt<int16_t>(300, 0), Pnt<int16_t>(800, 0)},

    // 5. DIAGONAL HIGH SPEED CRASH (Equal mass)
    // Obj1: (600, 600) hits Obj2: (-400, -400) head-on.
    // Result: Complete swap of diagonal vectors.
    { 
        Obj<int16_t>(-10, -10, 600, 600), 2, 
        Obj<int16_t>(10, 10, -400, -400), 2, 
        Pnt<int16_t>(-400, -400), 
        Pnt<int16_t>(600, 600) 
    },

    // 6. GLANCING BLOW (Vertical offset)
    // Speed 1000. Impact purely on Y-axis (normal is 0,1). 
    // X speed (tangent) stays 1000. Y speeds swap.
    { 
        Obj<int16_t>(0, 0, 1000, 100), 2, 
        Obj<int16_t>(0, 10, 0, -100), 2, 
        Pnt<int16_t>(1000, -100), 
        Pnt<int16_t>(0, 100) 
    },

    // 7. ASYMMETRIC 2D (m1=2, m2=1)
    // Impact on X-axis (normal is 1,0). Y speeds (tangent) stay.
    // X logic: u1 = (2-1)/3 * 300 = 100 | u2 = (2*2)/3 * 300 = 400
    { 
        Obj<int16_t>(0, 0, 300, 50), 2, 
        Obj<int16_t>(10, 0, 0, 150), 1, 
        Pnt<int16_t>(100, 50), 
        Pnt<int16_t>(400, 150) 
    }
 };


  for(size_t i = 0; i < sizeof(testData) / sizeof(testData[0]); i++){  
    bounce2d<int16_t>(testData[i].obj1, testData[i].m1, testData[i].obj2, testData[i].m2);
    
    TEST_ASSERT_EQUAL_INT16(testData[i].vel1After.x, testData[i].obj1.vel.x);
    TEST_ASSERT_EQUAL_INT16(testData[i].vel1After.y, testData[i].obj1.vel.y);
    TEST_ASSERT_EQUAL_INT16(testData[i].vel2After.x, testData[i].obj2.vel.x);
    TEST_ASSERT_EQUAL_INT16(testData[i].vel2After.y, testData[i].obj2.vel.y);
  }

  
}

void testRect2Line(){

  struct {
    int8_t t;
    int8_t x;
    int8_t y;    
  } testData[] = {
    {0, 0, 0}, {1, 1, 0}, {14, 14, 0}, {15, 15, 0},
    {16, 15, 1}, {17, 15, 2}, {29, 15, 14}, {30, 15, 15},
    {31, 14, 15}, {32, 13, 15}, {44, 1, 15}, {45, 0, 15},
    {46, 0, 14}, {47, 0, 13}, {59, 0, 1}, {60, 0, 0}, {-1, 0, 1}, {-2, 0, 2 }
  };  

  Rect2Line8_t rl(15, 15);

  for(size_t i = 0; i < sizeof(testData) / sizeof(testData[0]); i++){
    
    Pnt<int8_t> p = rl.toRect(testData[i].t);    
    TEST_ASSERT_EQUAL_INT8(testData[i].x, p.x);
    TEST_ASSERT_EQUAL_INT8(testData[i].y, p.y);

    if(testData[i].t < 0 || testData[i].t >= rl.length()) 
      continue; //Skip negative t for toLine test
    
    int8_t t = rl.toLine(p);
    TEST_ASSERT_EQUAL_INT8(testData[i].t, t);
  }

  
}


int main(int , char **){
  UNITY_BEGIN();

  
  RUN_TEST(testu8Sqrt);
  RUN_TEST(testu16Sqrt);
  RUN_TEST(testBounce1d);
  RUN_TEST(testBounce2d);
  RUN_TEST(testObjMoveAwayLeft);
  RUN_TEST(testObjMoveAwayRight);
  RUN_TEST(testObjMoveAwayUp);
  RUN_TEST(testObjMoveAwayDown);
  RUN_TEST(testRect2Line);

  UNITY_END();
  return 0;
}

