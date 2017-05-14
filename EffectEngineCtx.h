#ifndef __EFFECTENGINECTX_H
#define __EFFECTENGINECTX_H

//Leds
#define MAX_LEDS    300

//Speed delay
#define SPEED_DELAY_MIN 1
#define SPEED_DELAY_MAX 1000

//Change commands
#define EEMC_NONE        0x00   //Nothing changed
#define EEMC_MODE        0x01   //Mode changed
#define EEMC_EFFECT      0x02   //Effect changed
#define EEMC_COLOR_HUE   0x04   //Color hue changed
#define EEMC_COLOR_SAT   0x05   //Color saturation changed
#define EEMC_COLOR_VAL   0x06   //Color value changed   
#define EEMC_SPEED       0x08   //Speed changed
#define EEMC_NUMLEDS     0x10   //Number of leds changed

//Modes
#define EEM_OFF      0x0000    //Off
#define EEM_STATIC   0x0001    //static
#define EEM_EFFECT   0x0002    //Effect
#define EEM_RANDOM   0x0003    //Random effect change after random interval bettwen 2 and 5 minutes - not implemented
#define EEM_SETUP    0x0004    //Setup
#define EEM_MODEMAX  EEM_EFFECT


///////////////////////////////////
// Control flags
#define CTF_NONE        0x00  //Nothing, value is absolute number
#define CTF_VAL_ABS     0x00  //Absolute number
#define CTF_VAL_DELTA   0x01  //Value is delta
#define CTF_VAL_NEXT    0x02  //Go next - in cycles
#define CTF_VAL_PREV    0x03  //Go previous - in cycles

////////////////////////////////////
// Control queue data
struct CtrlQueueData{
  uint8_t flag;   //Flag that shows how to interpret the value: absolute number, inrement, etc 
  int     value;  //Value
  int     min;    //Value minimum
  int     max;    //Value maximum   
  
  CtrlQueueData(){
      flag   = CTF_NONE;
      value  = 0;
      min    = 0;
      max    = 0;
  }

  int translate(int base, int vmin, int vmax) const{
    
     switch(flag){ 
      case CTF_VAL_ABS:  //Absolute value
        base = map(value, min, max, vmin, vmax);
      break;
      case CTF_VAL_NEXT: //Go next
        base ++;
        if(base > vmax) base = vmin;
      break;
      case CTF_VAL_PREV: //Go Prev
        base --;
        if(base < vmin) base = vmax;
      break;
      case CTF_VAL_DELTA://Delta
        base += value;
        if(base < vmin)      base = vmin;
        else if(base > vmax) base = vmax;
      break;
    }

    return base;
  }
};



////////////////////////////////////
// Control queue element
struct CtrlQueueItem {
  uint8_t       cmd;    // Command
  CtrlQueueData data;   // Data

  CtrlQueueItem(){
     cmd = EEMC_NONE;
  }
};


#endif //__EFFECTENGINECTX_H
