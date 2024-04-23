#ifndef STEPBEEPLED_H
#define STEPBEEPLED_H

#include "Arduino.h"

class StepBeepLed {

  public:

  StepBeepLed(uint8_t pin4, uint8_t pin3, uint8_t pin2, uint8_t pin1, uint8_t pinB, uint8_t pinL);
  void begin(uint8_t model);
  void run(uint32_t steps, uint8_t vel, boolean cw);
  void beep(int xbnum, int xbdur, int xbfreq, int xbinter);
  void led(int xlnum, int xldur, int xlinter);
  void setms(uint32_t yms);
  uint32_t getms();
  uint32_t where();

  int bdur=500, binter=500, bfreq=500;      //duration, interval and frequency of beeps (values can be changed by user)
  volatile int bnum=0;                      //quantity of beeps to be issued

  int ldur=500, linter=500;                 //duration and interval of led blinking (values can be changed by user)
  volatile int lnum=0;                      //quantity of blinking to be issued

  volatile uint32_t xsteps=0, xms=0;


  private:

  void onTimer1ms();
  void go();
  void move0();
  void move1();
  void move2();
  void move3();
  void writ(uint8_t px1, uint8_t px2, uint8_t px3, uint8_t px4);
  static StepBeepLed *isrTable[];
  using isrFunct = void (*)();
  template<uint8_t NUM_INTERRUPTS = SOC_TIMER_GROUP_TOTAL_TIMERS>
  static isrFunct getIsr(uint8_t timerNumber);
  uint8_t p4,p3,p2,p1,pB,pL, inivel=5,xmodel=2;

  //variaveis de controle do stepmotor
  volatile uint8_t xvel=5;
  volatile int xfase=0;
  volatile boolean xcw=true;

  //variaveis de controle do beep
  volatile bool bxpausa=false, bxpri=true;
  volatile int bxinter=0, bxdur=0;

  //variaveis de controle do led
  volatile bool lxpausa=false, lxpri=true;
  volatile int lxinter=0, lxdur=0;

};


template<uint8_t NUM_INTERRUPTS>
StepBeepLed::isrFunct StepBeepLed::getIsr(uint8_t timerNumber) {
  if (timerNumber == (NUM_INTERRUPTS - 1)) {
    return [] {
      isrTable[NUM_INTERRUPTS - 1]->onTimer1ms();
    };
  }
  return getIsr < NUM_INTERRUPTS - 1 > (timerNumber);
}

template<>
inline StepBeepLed::isrFunct StepBeepLed::getIsr<0>(uint8_t timerNumber) {
  (void) timerNumber;
  return nullptr;
}

#endif

