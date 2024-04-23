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

StepBeepLed sm(16,17,18,19, 5,4);      //define constructor for driver motors pins in1,in2,in3,in4, pin beep, pin led

void setup()
{
  sm.begin(0);                         //define the Nema17 step motor (mode 0)
                                       //max speed 3 for forward (clockwise, right)
                                       //max speed 5 for backward (counterclockwise, left) 
}

void loop()
{
  while (sm.where()>0){
    //wait until step motor arrives to its last destination
    //during this wait time, the microcontroller is free to do anything
    }
    
  sm.setms(500); while(sm.getms()>0){
    //wait 500ms
    //setms/getms is the way to wait some amount of time without blocking
    //during this wait time, the microcontroller is free to do anything
    }

  sm.run(200, 8, true);                //start 200 steps forward (clockwise, right) 
  while (sm.where()>0){}               //wait until step motor arrives to its last destination
  sm.setms(500); while(sm.getms()>0){} //wait 500ms
  sm.run(200, 8, false);               //start 200 steps backward (counterclockwise, left)
   
  sm.led(100, 50, 25);                 //short blink led for 100 times

  for (int i=0; i<3; i++){             //issue beeps  (3 sets of two different frequency)   
    sm.beep(1, 1000, 2000, 0); sm.setms(1000); while(sm.getms()>0){}
    sm.beep(1, 1000, 1600, 0); sm.setms(1000); while(sm.getms()>0){}
  } 
}
