#ifndef __MSGEQ7SC_H
#define __MSGEQ7SC_H

////////////////////////////
// MSGEQ7 sound captupre
class SoundCaptureMSGEQ7: public SoundCapture{
  public:
    SoundCaptureMSGEQ7(unsigned char pinAnalog, uint8_t pinStrobe, uint8_t pinReset);
    ~SoundCaptureMSGEQ7();

    void init();
    void reset();
    void idle();
    void getData(SoundCaptureData &data) const;


  private:
    uint8_t _pinAnalog;
    uint8_t _pinStrobe:4;
    uint8_t _pinReset:4;
};

#endif //__MSGEQ7SC_H
