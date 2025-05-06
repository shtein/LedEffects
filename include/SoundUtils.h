#ifndef __SOUND_UTILS_H
#define __SOUND_UTILS_H

#ifdef USE_SOUND


/////////////////////////////////////////////
// Running statistics
class RunningStats{ 
  public:
    RunningStats(uint8_t smoothFactor = 25);

    void reset();
    void add(uint8_t val);

    uint8_t getAverage() const;
    uint8_t getStdDev() const;
  
  private:
    uint8_t  _mean;
    uint16_t _variance;

    uint8_t  _smoothFactor;
};


/////////////////////////////////////////////
// Sound Statistics

//Get statistics from SoundStats class
enum SoundStatGet{
  ssgMin = 0,
  ssgMax,  
  ssgAverage,
  ssgAverageBass,
  ssgAverageMid,
  ssgAverageTreble,
};

class SoundStats{
public:
  SoundStats();

  void reset();                                   //Reset statistics
  void process(uint8_t *val, size_t count);       //Process new data

  const RunningStats& get(SoundStatGet ssg) const; //Get statistics

private:
  RunningStats _min;
  RunningStats _max;
  RunningStats _mean;
  RunningStats _meanBass;    
  RunningStats _meanMid;
  RunningStats _meanTreble;
  
  uint8_t  _curMin;
  uint8_t  _curMax;
  uint8_t  _count;
};


//////////////////////////////////////////////
// Representation for sound capture visualization

#define SC_MAX_BANDS        7    //Number of bands

#define SC_MAP_USE_MIN      0x01 //Use running min for lower boundary
#define SC_MAP_USE_MAX      0x02 //Use running max for upper boundary
#define SC_MAP_ABOVE_NOISE  0x04 //Make 0 everything below noise threshold
#define SC_MAP_LOG          0x10 //Logarithmic scale

#define SOUND_LOWER_MIN     0    //Deault minimum in scale from 0 to 255
#define SOUND_UPPER_MAX     255  //Default maximum in scale from 0 to 255

void scaleSound(uint8_t *bands, uint8_t numBands, uint8_t flags,
                uint8_t lower, uint8_t upper, 
                uint8_t min, uint8_t max, uint8_t average, uint8_t stdDev
              );


#endif //USE_SOUND

#endif //__SOUND_UTILS_H


