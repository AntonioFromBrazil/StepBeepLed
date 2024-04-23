<html>

<head>
<meta charset="utf-8">
</head>

<body>

<table border="0" width="100%" id="table1" cellspacing="0" cellpadding="0">
	<tr>
		<td align="center">
		<p align="left"><font size="4" face="Courier">
        <hr align="left">
		</font>
		<p align="left"><b><font face="Courier" size="5"><br>
		This is an ESP-32 nonblocking 
		step motor library based on interrupts for Nema17 and 28byj-48<br>
		combined with beep and led control<br>
		</font></b><font size="4" face="Courier">
		&nbsp;<br>
		By Antonio Testa, April/2024<br>
		<br>
		Special thanks to @gfvalvo from https://forum.arduino.cc who fixed the 
		error: invalid-use-of-non-static-member-function and gave all tricks to 
		use hardware timers inside the library. Also thanks to @b707 from same forum for valuable suggestions<br>
		<br>
		https://forum.arduino.cc/t/error-invalid-use-of-non-static-member-function/1240240<br>
		<br>
        </p>
        <hr align="left">
		<p align="left"><br>
		In the main program is mandatory to define the following:<br>
		<br>
		#include &lt;StepBeepLed.h&gt;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;//include the 
		StepBeepLed.h library <br>
		<br>
		StepBeepLed name(in1,in2,in3,in4,pin_beep,pin_led); //define constructor 
		for driver motors, beep and led pins<br>
		<br>
		name.begin(mode);&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; //At setup(), issue begin() command to init the 
		library timer and set stepmotor mode operation<br>
		<br>
        </p>
        <hr align="left">
		<p align="left">
		</font><b><font face="Courier" size="5">CONSTRUCTOR:</font></b><font size="4" face="Courier"><br>
		<br>
		<b>StepBeepLed name(pin1, pin2, pin3, pin4, pin_beep, pin_led);</b> <br>
		name is any name for instance stepmotor<br>
		uint8_t pin1,2,3,4 reffers to ESP-32 pins connected to in1,2,3,4 of 
		motor driver<br>
                uint8_t pin_beep, pin_led reffers to ESP-32 pins connected to beep and led</font></p><font size="4" face="Courier">
        <hr align="left">
		</font>
		<p align="left">
		<font face="Courier" size="5"><b>METHODS:</b></font><font size="4" face="Courier"><br>
		<br>
		<b>name.begin(mode);</b><br>
		at setup() section, to init the library timer and step motor model and 
		mode of operation<br>
		uint8_t mode should be:<br>
		0 = For Nema17, 200 steps per turn<br>
		1 = For 28byj-48 full step, 2048 steps per turn, low torque, low consumption<br>
		2 = For 28byj-48 full step, 2048 steps per turn, high torque, high consumption<br>
		3 = For 28byj-48 half step, 4096 steps per turn, middle torque, middle 
		consumption<br>
		<br>
		<b>name.run(steps,vel,cw);</b><br>
		uint32_t steps is the number of steps to travel<br>
		uint8_t vel is the speed (from 2 to ...) higher values lower speed<br>
		boolean cw define direction of turn (true for right, clockwise; false 
		for left, counterclockwise)<br>
		IMPORTANT: the name.run can only be called if the name.where()==0<br>
		<br>
		<b>name.beep(bnum,bdur,bfreq,binter);</b><br>
		int bnum is the quantity of beeps to be issued<br>
		int bdur is the duration of each beep (in ms)<br>
		int bfreq is the beep frequency (in Hz)<br>
		int binter is the silence duration between each beep (in ms)<br>
		<br>
		<b>name.led(lnum,ldur,linter);</b><br>
		int lnum is the quantity of led blinking desired<br>
		int ldur is the duration of led (in ms)<br>
		int linter is the duration of led off (in ms)<br>
		<br>
		<b>name.where();<br>
		</b>return in an uint32_t value the number of steps remainding to the 
		motor arrives to its destination<br>
		if name.where()==0 means the motor is already in the last destination 
		and is stopped at this moment</font></p>
		<p align="left">
		<font size="4" face="Courier"><b>name.setms(ms);<br>
		</b>set the milliseconds counter down with the uint32_t ms value (nonblock, to be used in substitution of 
		delay function)<br>
		As soon as initiated this milliseconds counter is decremented each millisecond<b><br>
		<br>
		name.getms();<br>
		</b>return the ms counter at moment (initial ms value setted by 
		previously name.setms)</font></p><font size="4" face="Courier">
        <hr align="left">
		</font>
		<p align="left">
		<b><font face="Courier" size="5">STEP MOTOR VARIABLES:</font></b><font face="Courier" size="4"><br>
		<br>
		<b>name.xsteps</b><br>
		is an uint32_t value with the number of steps remainding to motor 
		arrives to destination<br>
		if name.xsteps==0 means the motor is already in the last destination and 
		is stopped at this moment</font></p><font size="4" face="Courier">
        <hr align="left">
		</font>
		<p align="left">
		<font face="Courier" size="5"><b>TIMER VARIABLES:</b></font><font face="Courier" size="4"><br>
		<br>
		<b>name.xms</b><br>
		is an uint32_t value of ms counter down (nonblock to be used in 
		substitution of delay function).<br>
		As soon as initiated this ms counter is decremented each millisecond</font></p>
		<font size="4" face="Courier">
        <hr align="left">
		</font>
		<p align="left">
		<b><font face="Courier" size="5">BEEP VARIABLES:</font></b><font face="Courier" size="4"><br>
		<br>
		<b>name.bnum</b><br>
		this an integer variable to define how many beeps the user wants<br>
		As soon as a value bigger than zero is informed, the beep starts to be 
		issued<br>
		Default=0<br>
		<br>
		<b>name.bdur</b><br>
		this an integer variable to define the beep duration in ms. Default=500<br>
		<br>
		<b>name.bfreq</b><br>
		this an integer variable to define the beep frequency in Hz. 
		Default=2000<br>
		<br>
		<b>name.binter</b><br>
		this an integer variable to define the pause beetwen beeps, in ms. 
		Default=500</font></p><font size="4" face="Courier">
        <hr align="left">
		</font>
		<p align="left">
		<font face="Courier" size="5"><b>LED VARIABLES:</b></font><font face="Courier" size="4"><br>
		<br>
		<b>name.lnum</b><br>
		this an integer variable to define how many led blink the user wants<br>
		As soon as a value bigger than zero is informed, the led starts to 
		blinking<br>
		Default=0<br>
		<br>
		<b>name.ldur</b><br>
		this an integer variable to define the led on duration in ms. 
		Default=500<br>
		<br>
		<b>name.linter</b><br>
		this an integer variable to define the led off duration beetwen blinking, 
		in ms. Default=500</p>
        <hr align="left"></font><br>
&nbsp;</td>
	</tr>
	<tr>
		<td align="center">
		<b><font face="Courier" size="4"><br>
		correct way to wire step motors to it's driver:<br>
&nbsp;</font></b></td>
	</tr>
	<tr>
		<td align="center">
		<img border="0" src="/images/Nema17.jpg" width="300" height="224" align="middle">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
		<img border="0" src="/images/28byj-48.jpg" width="300" height="224" align="middle"></td>
	</tr>
	<tr>
		<td align="center">
		<b><font size="4" face="Courier"><br>
		popular drivers for Nema17 and 28byj-48 step motors:<br>
&nbsp;</font></b></td>
	</tr>
	<tr>
		<td align="center">
		<img border="0" src="/images/D_L298.jpg" width="240" height="240" align="middle">&nbsp;&nbsp;&nbsp;&nbsp;
		<img border="0" src="/images/D_ULN2003.jpg" width="240" height="240" align="middle">&nbsp;&nbsp;&nbsp;&nbsp;
		<img border="0" src="/images/D_L293.jpg" width="240" height="240" align="middle"></td>
	</tr>
		<tr>
		<td align="center">
		<b><font size="4" face="Courier"><br>
		typical beep and led schematics:<br>
&nbsp;</font></b></td>
	</tr>
	<tr>
		<td align="center">
		<img border="0" src="/images/beep.jpg" width="240" height="240" align="middle">&nbsp;&nbsp;&nbsp;&nbsp;
		<img border="0" src="/images/led.jpg" width="240" height="240" align="middle">&nbsp;&nbsp;&nbsp;&nbsp;
        </td>
	</tr>
</table>
    <br><hr align="left">
</body>

</html>
