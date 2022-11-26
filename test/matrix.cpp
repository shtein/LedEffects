#include <unity.h>
#include "MatrixUtils.h"
#include <stdio.h>


const char *strMsgVert = "Move away %s check fails -- vertical: %d, position: %d, speed: %d";

void testObjMoveAwayLeft(){    

  Obj<int8_t> obj;  
  const uint8_t vert = 5;
  char msg[128];

  for(int pos = 0; pos < 3; pos++){ //left side, on, right side of the vertical
    obj.pos.x = vert - pos - 1;

    for(int vel = 0; vel < 3; vel++){ //moves to the left, stays, to the right      
            
      obj.vel.x  = vel - 1;
      sprintf(msg, strMsgVert, "left", vert, obj.pos.x, obj.vel.x);

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
      sprintf(msg, strMsgVert, "right", vert, obj.pos.x, obj.vel.x);

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
      sprintf(msg, strMsgVert, "up", horiz, obj.pos.y, obj.vel.y);

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
      sprintf(msg, strMsgVert, "down", horiz, obj.pos.y, obj.vel.y);

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



int main(int , char **){
  UNITY_BEGIN();

  RUN_TEST(testObjMoveAwayLeft);
  RUN_TEST(testObjMoveAwayRight);
  RUN_TEST(testObjMoveAwayUp);
  RUN_TEST(testObjMoveAwayDown);
  
  UNITY_END();
  return 0;
}