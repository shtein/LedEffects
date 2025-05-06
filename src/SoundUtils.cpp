#ifdef USE_SOUND

#include "LedEffects.h"
#include "SoundUtils.h"
#include <utils.h>






/////////////////////////////////////
// RunningStats
RunningStats::RunningStats(uint8_t smoothFactor){
  _smoothFactor = smoothFactor;

  reset();
} 

void RunningStats::reset(){
  _mean     = 0;
  _variance = 0;
}

#define SMOOTH_FACTOR_MAX 255

void RunningStats::add(uint8_t val){
  int16_t delta = (int16_t)val - _mean;

  uint16_t tmp = (delta * _smoothFactor + SMOOTH_FACTOR_MAX / 2) / SMOOTH_FACTOR_MAX;

  _mean += tmp;

  _variance = ((SMOOTH_FACTOR_MAX - _smoothFactor) * _variance + 
                _smoothFactor * delta * delta + SMOOTH_FACTOR_MAX / 2) / SMOOTH_FACTOR_MAX;
}

uint8_t RunningStats::getAverage() const{
  return _mean;
}

uint8_t RunningStats::getStdDev() const{
  return u8Sqrt(_variance);
}


////////////////////////////////////
// SoundStat
#define SOUNDSTAT_MAX_COUNT 70

SoundStats::SoundStats(): _meanBass(25) {
  reset();  
}

void SoundStats::reset(){
  _min.reset();
  _max.reset();
  _mean.reset();
  _meanBass.reset();
  _meanMid.reset();
  _meanTreble.reset();

  _curMin = 255;
  _curMax = 0;
  _count  = 0;
}


void SoundStats::process(uint8_t *vals, size_t count){

  for(size_t i = 0; i < count; i++){

    //Increment counter
    _count++;
    
    //add bass
    if(i < 2){
      _meanBass.add(vals[i]);  
    }
    else if(i < 4){
      _meanMid.add(vals[i]);
    }
    else{
      _meanTreble.add(vals[i]);
    }
    

    //Add mean
    _mean.add(vals[i]);
    
    //Min
    if(_curMin > vals[i]){
      _curMin = vals[i];
    }

    //Max
    if(_curMax < vals[i]){
      _curMax = vals[i];
    }   


    //See if we need add min and max
    if(_count >= SOUNDSTAT_MAX_COUNT){
      _min.add(_curMin);
      _max.add(_curMax);

      _curMin  = 255;      
      _curMax  = 0;

      _count = 0;
    }
  }     
}

const RunningStats& SoundStats::get(SoundStatGet ssg) const{
  
  
  switch(ssg){
    case ssgMin:
    return _min;

    case ssgMax:
    return _max;

    case ssgAverage:
    return _mean;

    case ssgAverageBass:
    return _meanBass;

    case ssgAverageMid:
    return _meanMid;

    case ssgAverageTreble:
    return _meanTreble;
  }

  //Return average by default
  return _mean;
}


//////////////////////////////////////////////
// Sound capture visualization
void scaleSound(uint8_t *bands, uint8_t numBands, uint8_t flags,
                uint8_t lower, uint8_t upper, 
                uint8_t min, uint8_t max, uint8_t average, uint8_t stdDev
              ){


  uint8_t mx  = (flags & SC_MAP_USE_MAX) ? max : upper;
  uint8_t mn  = (flags & SC_MAP_USE_MIN) ? min : lower < upper ? lower : upper;  

  for(uint8_t i = 0; i < numBands; i++){

    uint8_t &val = bands[i];

    //if noise threshold is set and value is below noise threshold
    if((flags & SC_MAP_ABOVE_NOISE) && (val < average - stdDev )){
      val = 0;
    }
    else{
      if(flags & SC_MAP_LOG){
        //TODO - implement logarithmic scale

        //mx  = (uint8_t)((uint16_t)mx * mx / 255);
        //mn  = (uint8_t)((uint16_t)mn * mn / 255);
        //val = (uint8_t)((uint16_t)val * val / 255);

        //mx = log10(mx) / log10(255) * 255;
        //mn = log10(mn) / log10(255) * 255;
        //val = val == 0 ? 0 : log10(val) / log10(255) * 255;
      }  
      
  
      val =  mapEx(val > mx ? mx : val < mn ? mn : val, mn, mx, 0, 255);
    }
  }
} 






#endif //USE_SOUND