------------------------------------------------------------------------------
This is an ESP-32 nonblocking step motor library based on interrupts for Nema17 and 28byj-48
combined with beep and led control
 
By Antonio Testa, April/2024

Special thanks to @gfvalvo from https://forum.arduino.cc who fixed the error: invalid-use-of-non-static-member-function and gave all tricks to use hardware timers inside the library. Also thanks to @b707 from same forum for valuable suggestions

https://forum.arduino.cc/t/error-invalid-use-of-non-static-member-function/1240240

------------------------------------------------------------------------------
In the main program is mandatory to define the following:

#include <StepBeepLed.h>                            //include the StepBeepLed.h library                   

StepBeepLed name(in1,in2,in3,in4,pin_beep,pin_led); //define constructor for driver motors, beep and led pins

name.begin(mode);                                   //At setup(), issue begin() command to init the library timer and set stepmotor mode operation

------------------------------------------------------------------------------
CONSTRUCTOR:

StepBeepLed name(pin1, pin2, pin3, pin4, pin_beep, pin_led); 
name is any name for instance stepmotor
uint8_t pin1,2,3,4 - reffers to ESP-32 pins connected to in1,2,3,4 of motor driver;
uint8_t pin_beep,pin_led - reffers to ESP-32 pins connected to pin_beep and pin_led

------------------------------------------------------------------------------
METHODS:

name.begin(uint8_t mode);
at setup() section, to init the library timer and step motor model and mode of operation
uint8_t mode should be:
0 = For Nema17, 200 steps per turn
1 = For 28byj-48 full step, 2048 steps per turn, low torque, low consumption
2 = For 28byj-48 full step, 2048 steps per turn, high torque, high consumption
3 = For 28byj-48 half step, 4096 steps per turn, middle torque, middle consumption

name.run(steps,vel,cw);
uint32_t steps is the number of steps to travel
uint8_t vel is the speed (from 2 to ...) higher values lower speed
boolean cw define direction of turn (true for right, clockwise; false for left, counterclockwise)
IMPORTANT: the name.run can only be called if the name.where()==0  

name.beep(bnum,bdur,bfreq,binter);
int bnum is the quantity of beeps to be issued
int bdur is the duration of each beep (in ms)
int bfreq is the beep frequency (in Hz)
int binter is the silence duration between each beep (in ms)

name.led(lnum,ldur,linter);
int lnum is the quantity of led blinking desired
int ldur is the duration of led (in ms)
int linter is the duration of led off (in ms)

name.where();
return in an uint32_t value the number of steps remainding to the motor arrives to its destination
if name.where()==0 means the motor is already in the last destination and is stopped at this moment

name.setms(uint32_t ms);
set the ms counter down (nonblock, to be used in substitution of delay function)
As soon as initiated this ms counter is decremented each millisecond

name.getms();
return the ms counter at moment (initial ms value setted by previously name.setms)

------------------------------------------------------------------------------
STEP MOTOR VARIABLES:

name.xsteps
is an uint32_t value with the number of steps remainding to motor arrives to destination
if name.xsteps==0 means the motor is already in the last destination and is stopped at this moment

------------------------------------------------------------------------------
TIMER VARIABLES:

name.xms
is an uint32_t value of ms counter down (nonblock to be used in substitution of delay function).
As soon as initiated this ms counter is decremented each millisecond

------------------------------------------------------------------------------
BEEP VARIABLES:

name.bnum
this an integer variable to define how many beeps the user wants
As soon as a value bigger than zero is informed, the beep starts to be issued
Default=0

name.bdur
this an integer variable to define the beep duration in ms. Default=500

name.bfreq
this an integer variable to define the beep frequency in Hz. Default=2000

name.binter
this an integer variable to define the pause beetwen beeps, in ms. Default=500

------------------------------------------------------------------------------
LED VARIABLES:

name.lnum
this an integer variable to define how many led blink the user wants
As soon as a value bigger than zero is informed, the led starts to blinking
Default=0

name.ldur
this an integer variable to define the led on duration in ms. Default=500

name.linter
this an integer variable to define the led off duration beetwen blinking, in ms. Default=500

------------------------------------------------------------------------------
