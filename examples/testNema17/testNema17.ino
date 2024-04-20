//
// library StepBeepLed.h
// Simple example of use with Nema17 step motor
// move 200 steps forward (clockwise, right) following by 200 steps backward (counterclockwise, left)
// 
// function used:
// begin() - init the timer library and stepmotor operation mode
// run()   - cause step motor to move according to parameters informed
// 

#include <StepBeepLed.h>

StepBeepLed sm(16,17,18,19, 5,2);      //define constructor for driver motors pins in1,in2,in3,in4, pin beep, pin led

void setup()
{
  sm.begin(0);                         //define the Nema17 step motor (mode 0)
                                       //max speed 3 for forward (clockwise, right)
                                       //max speed 5 for backward (counterclockwise, left) 
}

void loop()
{
  while (sm.xsteps>0){
    //wait until step motor arrives to its last destination
    //during this wait time, the microcontroller is free to do anything
    }
    
  sm.xms=500;while(sm.xms>0){
    //wait 500ms
    //setms/getms is the way to wait some amount of time without blocking
    //during this wait time, the microcontroller is free to do anything
    }

  sm.run(200, 8, true);                //start 200 steps forward (clockwise, right) 
  while (sm.xsteps>0){}                //wait until step motor arrives to its last destination
  sm.xms=500;while(sm.xms>0){}         //wait 500ms
  sm.run(200, 8, false);               //start 200 steps backward (counterclockwise, left)
   
  sm.ldur=50;sm.linter=25;sm.lnum=100; //short blink led for 100 times
  
  for (int i=0; i<3; i++){             //issue beeps  (3 sets of two different frequency)   
    sm.bdur=1000; sm.bfreq=2000; sm.binter=0; sm.bnum=1; sm.xms=1000; delay(1); while(sm.xms>0){}
    sm.bdur=1000; sm.bfreq=1600; sm.binter=0; sm.bnum=1; sm.xms=1000; delay(1); while(sm.xms>0){}
  }  
}
