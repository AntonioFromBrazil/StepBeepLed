//
// library StepBeepLed.h
// Simple example of use with 28byj-48 step motor
// move 2048 steps forward (clockwise, right) following by 2048 steps backward (counterclockwise, left)
// 
// function used:
// begin() - init the timer library and stepmotor operation mode
// run()   - cause step motor to move according to parameters informed
// 

#include <StepBeepLed.h>

StepBeepLed sm(16,17,18,19, 5,2);      //define constructor for driver motors pins in1,in2,in3,in4, pin beep, pin led

void setup()
{
  sm.begin(2);                         //define the 28byj-48 step motor mode 2 operation
                                       //1 = full step, 2048 steps per turn, low torque, low consumption
                                       //2 = full step, 2048 steps per turn, high torque, high consumption
                                       //3 = half step, 4096 steps per turn, middle torque, middle consumption

                                       //the max speed depending on mode selected:
                                       //mode 1 - max speed 3 for clockwise and 5 for counterclockwise
                                       //mode 2 - max speed 3 for clockwise and 5 for counterclockwise
                                       //mode 3 - max speed 2 for clockwise and 2 for counterclockwise
}

void loop()
{
  while (sm.xsteps>0){
    //wait until step motor arrives to its last destination
    //during this wait time, the microcontroller is free to do anything
    }
    
  sm.xms=250; while(sm.xms>0){
    //wait 250ms
    //ms is the way to wait some amount of time without blocking
    //during this wait time, the microcontroller is free to do anything
    }

  sm.run(2048, 3, true);               //start 2048 steps forward (clockwise, right) 
  while (sm.xsteps>0){}                //wait until step motor arrives to its last destination
  sm.xms=250; while(sm.xms>0){}        //wait 250ms
  sm.run(2048, 6, false);              //start 2048 steps backward (counterclockwise, left)

  sm.ldur=50;sm.linter=25;sm.lnum=100; //short blink led for 100 times
  
  for (int i=0; i<3; i++){             //issue beeps  (3 sets of two different frequency)   
    sm.bdur=1200; sm.bfreq=2000; sm.binter=0; sm.bnum=1; sm.xms=1200; delay(1); while(sm.xms>0){}
    sm.bdur=1200; sm.bfreq=1600; sm.binter=0; sm.bnum=1; sm.xms=1200; delay(1); while(sm.xms>0){}
  }  
}
