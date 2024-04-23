#include "StepBeepLed.h"

//----------------------------------------------------------------------
StepBeepLed::StepBeepLed(uint8_t pin4, uint8_t pin3, uint8_t pin2, uint8_t pin1, uint8_t pinB, uint8_t pinL)
{
  pinMode(pin4,OUTPUT);                              //define the driver motor pins as output
  pinMode(pin3,OUTPUT);                              //
  pinMode(pin2,OUTPUT);                              //
  pinMode(pin1,OUTPUT);                              //
  pinMode(pinB,OUTPUT);                              //
  pinMode(pinL,OUTPUT);                              //

  digitalWrite(pin4,0);                              //initialize then with zeroes
  digitalWrite(pin3,0);                              //
  digitalWrite(pin2,0);                              //
  digitalWrite(pin1,0);                              //
  digitalWrite(pinB,0);                              //
  digitalWrite(pinL,0);                              //

  p4=pin4, p3=pin3, p2=pin2, p1=pin1;                //copy pins to internal variables
  pB=pinB, pL=pinL;                                  //

  ledcAttachPin(pB, 0);ledcWrite(0, 0);              //define channel 0 for beep on pin pB and write 0 to it
  ledcSetup(0, bfreq, 8);                            //
}


//----------------------------------------------------------------------
void StepBeepLed::begin(uint8_t model) {
  xmodel=model;
  const uint8_t timerNumber = 0;
  hw_timer_t *timer1ms = NULL;
  timer1ms = timerBegin(timerNumber, 80, true);
  isrTable[timerNumber] = this;
  auto isr = getIsr(timerNumber);
  timerAttachInterrupt(timer1ms, isr, false);
  timerAlarmWrite(timer1ms, 1000, true);
  timerAlarmEnable(timer1ms);
}


//----------------------------------------------------------------------
void StepBeepLed::run(uint32_t steps, uint8_t vel, boolean cw)
{
  inivel=vel;xvel=vel;xcw=cw;xsteps=steps;
  if (cw){xfase=-1;}else{xfase=4;}
}


//----------------------------------------------------------------------
uint32_t StepBeepLed::where()
{
  return xsteps;
}


//----------------------------------------------------------------------
void StepBeepLed::beep(int xbnum, int xbdur, int xbfreq, int xbinter)
{
  bnum=xbnum; bdur=xbdur; bfreq=xbfreq; binter=xbinter;
}


//----------------------------------------------------------------------
void StepBeepLed::led(int xlnum, int xldur, int xlinter)
{
  lnum=xlnum; ldur=xldur; linter=xlinter;
}


//----------------------------------------------------------------------
void StepBeepLed::setms(uint32_t yms)
{
  xms=yms;
}


//----------------------------------------------------------------------
uint32_t StepBeepLed::getms()
{
  return xms;
}


//----------------------------------------------------------------------
void IRAM_ATTR StepBeepLed::onTimer1ms()
{
  if (xms>0){xms--;}
  xvel--;
  if (xvel==0){
    xvel=inivel;
    if (xsteps>0){
      int nf=3;if (xmodel==3){nf=7;}
      if (xcw){xfase++;if (xfase>nf){xfase=0;}}else{xfase--;if (xfase<0){xfase=nf;}}
      StepBeepLed::go();
      xsteps--;
    }
    if (xsteps==0){digitalWrite(p1, 0);digitalWrite(p2, 0);digitalWrite(p3, 0);digitalWrite(p4, 0);}
  }


  if (bnum>0){
    if (bxpri){                           //if is the beginning of cycle to beep,
      bxinter=binter+1; bxdur=bdur;       //init the time variables
      bxpausa=false; bxpri=false;         //with default values or user values
      ledcSetup(0, bfreq, 8);             //
    }                                     // 
    if (!bxpausa && (bxdur>0)) {          //if it is beeping 
      ledcWrite(0, 127);bxdur--;          //keep the beep beeping for bxdur ms
      if(bxdur==0){                       //at end,
        ledcWrite(0, 0);                  //stop the beep and advise 
        bxpausa=true;                     //that pause fase is to be initiated
      }
    }
    if (bxpausa && (bxinter>0)){          //if it is in pause
      ledcWrite(0, 0);bxinter--;          //keep the beep stoped for bxinter ms
      if(bxinter==0){                     //at end, exit from pause, subtract 1 from quantity of desired 
        bxpausa=false;bnum--;bxpri=true;  //beeps and advise to reload the variables for a new cycle
      }
    }
  }


  if (lnum>0){
    if (lxpri){                           //if is the beginning of cycle to blink led,
      lxinter=linter+1; lxdur=ldur;       //init the time variables
      lxpausa=false; lxpri=false;         //with default values or user valuess
    }                                     // 
    if (!lxpausa && (lxdur>0)) {          //if the led is on (out of pause fase)
      digitalWrite(pL, 1);lxdur--;        //keep the led on for lxdur ms
      if(lxdur==0){                       //at end,
        digitalWrite(pL, 0);              //turn off the led and advise
        lxpausa=true;                     //that pause fase is to be initiated
      }
    }
    if (lxpausa && (lxinter>0)){          //if the led is off (pause fase)
      digitalWrite(pL, 0);lxinter--;      //keep the led off for lxinter ms
      if(lxinter==0){                     //at end, exit from pause, subtract 1 from quantity of desired
        lxpausa=false;lnum--;lxpri=true;  //blinks and advise to reload the variables for a new cycle
      }
    }
  }

}

StepBeepLed *StepBeepLed::isrTable[SOC_TIMER_GROUP_TOTAL_TIMERS];


//----------------------------------------------------------------------
void StepBeepLed::go()
{
  switch (xmodel) {
    case 0: StepBeepLed::move0(); break;   //Nema17, 200 steps
    case 1: StepBeepLed::move1(); break;   //28byj-48, 2048 steps, full step, low torque, low consumption
    case 2: StepBeepLed::move2(); break;   //28byj-48, 2048 steps, full step, high torque, high consumption
    case 3: StepBeepLed::move3(); break;   //28byj-48, 4096 steps, half step, high torque, high consumption
  }
}


//----------------------------------------------------------------------
void StepBeepLed::move0(){   //Nema17, 200 steps
  switch (xfase) {
    case 0: StepBeepLed::writ(1,0,1,0); break;   //0x0A
    case 1: StepBeepLed::writ(0,1,1,0); break;   //0x06
    case 2: StepBeepLed::writ(0,1,0,1); break;   //0x05
    case 3: StepBeepLed::writ(1,0,0,1); break;   //0x09
  }
}

void StepBeepLed::move1(){   //28byj-48, 2048 steps, full step, low torque, low consumption
  switch (xfase) {
    case 0: StepBeepLed::writ(0,0,0,1); break;   //0x01
    case 1: StepBeepLed::writ(0,0,1,0); break;   //0x02
    case 2: StepBeepLed::writ(0,1,0,0); break;   //0x04
    case 3: StepBeepLed::writ(1,0,0,0); break;   //0x08
  }
}

void StepBeepLed::move2(){   //28byj-48, 2048 steps, full step, high torque, high consumption
  switch (xfase) {
    case 0: StepBeepLed::writ(1,0,0,1); break;   //0x09
    case 1: StepBeepLed::writ(0,0,1,1); break;   //0x03
    case 2: StepBeepLed::writ(0,1,1,0); break;   //0x06
    case 3: StepBeepLed::writ(1,1,0,0); break;   //0x0C    
  }
}

void StepBeepLed::move3(){   //28byj-48, 4096 steps, half step, high torque, high consumption
  switch (xfase) {
    case 0: StepBeepLed::writ(1,0,0,1); break;   //0x09
    case 1: StepBeepLed::writ(0,0,0,1); break;   //0x01
    case 2: StepBeepLed::writ(0,0,1,1); break;   //0x03
    case 3: StepBeepLed::writ(0,0,1,0); break;   //0x02
    case 4: StepBeepLed::writ(0,1,1,0); break;   //0x06
    case 5: StepBeepLed::writ(0,1,0,0); break;   //0x04
    case 6: StepBeepLed::writ(1,1,0,0); break;   //0x0C
    case 7: StepBeepLed::writ(1,0,0,0); break;   //0x08
  } 
}


//----------------------------------------------------------------------
void StepBeepLed::writ(uint8_t px1, uint8_t px2, uint8_t px3, uint8_t px4)
{
  digitalWrite(p1, px1);
  digitalWrite(p2, px2);
  digitalWrite(p3, px3);
  digitalWrite(p4, px4);
}

//----------------------------------------------------------------------


