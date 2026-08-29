
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unity.h>
#include <alutils.h>
#include "MatrixUtils.h"


///////////////////////////////////////////////////////////////////////////////
// EffectMatrixBounsingDots reproduction
//
// Emulates EffectMatrixBounsingDots::proceed() WITHOUT the draw step:
//   1) object collision pass (pairwise collides() -> bounce2d())
//   2) move-with-gravity + boundary bounce (velocity flip, with a speed-bleed
//      that shaves 1 unit off the flipped component when a dot is running hot)
// then counts how many DISTINCT integer cells the 5 objects occupy.
//
// In the real effect the draw uses XY_DRAW_ADD_COLORS, so two objects sharing
// one integer cell light a SINGLE pixel; the distinct-cell count is therefore
// the number of lit pixels.
//
// MatrixAnimation.h can't be included here (FastLED deps), so the physics is
// replicated locally, identical to the original.

#define BD_N   5
#define BD_W   16
#define BD_H   16
#define BD_G   4                        // G_CONST in MatrixAnimation.h
#define BD_I2FP(v) ((int16_t)((v) << 8))
#define BD_FP2I(v) ((int8_t)((v) >> 8))
#define BD_MAX_VEL_SQ 250000            // speed-bleed threshold (squared vel units)

// Identical to EffectMatrixBounsingDots::moveGravity.
static void bdMoveGravity(Obj<int16_t> &o, int16_t t){
  o.pos.x = o.vel.x * t + o.pos.x;
  o.pos.y = t * t * BD_G / 2 + o.vel.y * t + o.pos.y;
  o.vel.y += BD_G * t;
}

// Number of distinct occupied cells (= number of lit pixels after additive draw).
static int bdDistinctCells(Obj<int16_t> *o, int n){
  int cx[BD_N], cy[BD_N], m = 0;
  for(int i = 0; i < n; i++){
    int8_t x = BD_FP2I(o[i].pos.x);
    int8_t y = BD_FP2I(o[i].pos.y);
    bool seen = false;
    for(int k = 0; k < m; k++)
      if(cx[k] == x && cy[k] == y){ seen = true; break; }
    if(!seen){ cx[m] = x; cy[m] = y; m++; }
  }
  return m;
}


// One frame of proceed() physics, minus the draw. Returns distinct cell count.
static int bdStep(Obj<int16_t> *o, const uint8_t *mass){
  const int16_t dist = BD_I2FP(1);
  const int16_t xmax = BD_I2FP(BD_W - 1);
  const int16_t ymax = BD_I2FP(BD_H - 1);

  // Object collision, identical to the effect.
  for(int i = 0; i < BD_N; i++){
    for(int j = i + 1; j < BD_N; j++){
      if(o[i].collides(o[j], dist)){
        bounce2d<int16_t>(o[i], mass[i], o[j], mass[j]);
      }
    }
  }

  // Move with gravity + boundary bounce. The bounce is a velocity flip, plus a
  // speed-bleed: when a dot is running hot (velSq > BD_MAX_VEL_SQ) the flipped
  // component is shrunk toward zero by 1, shedding a little energy each wall hit.
  // The +-1 must OPPOSE the sign of the post-flip velocity to reduce its
  // magnitude (left/up flip to +, so subtract; right/down flip to -, so add).
  for(int i = 0; i < BD_N; i++){

    bdMoveGravity(o[i], 1);

    uint32_t velSq = (uint32_t)o[i].vel.x * o[i].vel.x + (uint32_t)o[i].vel.y * o[i].vel.y;

    if(o[i].movesAwayLeft(0))     o[i].vel.x = -o[i].vel.x - (velSq > BD_MAX_VEL_SQ ? 1 : 0);
    if(o[i].movesAwayRight(xmax)) o[i].vel.x = -o[i].vel.x + (velSq > BD_MAX_VEL_SQ ? 1 : 0);
    if(o[i].movesAwayUp(0))       o[i].vel.y = -o[i].vel.y - (velSq > BD_MAX_VEL_SQ ? 1 : 0);
    if(o[i].movesAwayDown(ymax))  o[i].vel.y = -o[i].vel.y + (velSq > BD_MAX_VEL_SQ ? 1 : 0);
  }

  return bdDistinctCells(o, BD_N);
}

static int16_t bdRandRange(int lo, int hi){   // uniform in [lo, hi)
  return (int16_t)(lo + rand() % (hi - lo));
}


void testBouncingDotsNoStuckMerge(){
  uint8_t mass[BD_N] = {2, 4, 6, 8, 10};
  Obj<int16_t> o[BD_N];

  for(int i = 0; i < BD_N; i++){
    o[i].pos.x = bdRandRange(0, BD_I2FP(BD_W));
    o[i].pos.y = bdRandRange(0, BD_I2FP(BD_H / 2));
    o[i].vel.x = bdRandRange(64, 192);
    if(rand() & 1) o[i].vel.x = -o[i].vel.x;
    o[i].vel.y = 0;
  }

  int belowStreak = 0, maxBelow = 0;
  for(uint32_t f = 0; f < 10000000; f++){
    int c = bdStep(o, mass);
    if(c < 5){ belowStreak++; if(belowStreak > maxBelow) maxBelow = belowStreak; }
    else       belowStreak = 0;

    if(maxBelow >= 100) break;   // real stick: bail fast, assertion below fails
  }

  TEST_ASSERT_TRUE(maxBelow < 100);
}


int main(int , char **){
  srand(time(NULL));

  UNITY_BEGIN();

  for(size_t i = 0; i < 100; i++)
    RUN_TEST(testBouncingDotsNoStuckMerge);

  return UNITY_END();
}

