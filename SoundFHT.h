#ifndef __SOUNDFHT_H
#define __SOUNDFHT_H

#include "pins.h"

//Enable FHT
#define FHT_N   256
#define LOG_OUT 1

#include <FHT.h>

//////////////////////////////////////////
//FHT Wrapper
class CFHT{
public:
  CFHT();
  ~CFHT();

  void init(uint8_t pin);    
  void reset();
  void process();

  const uint8_t *getLogOut() const;
    
public:
  static CFHT _instance;  //Single instance
  
private:
  uint8_t _pin;  //Microphone pin    
};


/////////////////////////////
// CFHT
CFHT CFHT::_instance;

inline CFHT::CFHT(): _pin(MIC_PIN) {
}


inline CFHT::~CFHT(){
}


inline void CFHT::init(uint8_t pin){
  _pin = pin;
}
    
inline void CFHT::reset(){
  //Erase everything
  memset(fht_input, 0x00, sizeof(fht_input));
}

inline void CFHT::process(){
  //Collectt samples
  
  cli();
  for (int i = 0 ; i < FHT_N ; i++) 
    fht_input[i] = analogRead(_pin) - 512;
  sei();

  //Process
  fht_window(); 
  fht_reorder();
  fht_run();
  fht_mag_log();
}

inline const uint8_t *CFHT::getLogOut() const{
  return fht_log_out;
}

#endif //__SOUNDFHT_H
