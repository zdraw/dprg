#include "inc/hw_types.h"		// tBoolean
#include "inc/hw_memmap.h"
#include "utils/uartstdio.h"	// input/output over UART
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

#include "RASLib/servo.h"

#include "RobzDemo.h"

void initServo(void) {
	InitializeServos();
}

void servoDemo(void) {
	UARTprintf("Press:\n  a-'up' 10\n  w-'up' 1\n  s-'down' 1\n");
	UARTprintf("  d-'down' 10\n  enter-quit\n");
	
	{
		unsigned char position = 0;
		unsigned char temp = 0;
		char newline = 13;
		char ch;
		
		UARTprintf("position: %u  ",position);
		ch = getc();
		while(ch != newline) {
			if (ch == 'w')
				position++;
			else if (ch == 's')
				position--;	 
			else if (ch == 'a')
				position+=10;
			else if (ch == 'd')
				position-=10;
			temp = 0.45*position + 140;
			SetServoPosition(SERVO_0, temp); 
			SetServoPosition(SERVO_1, temp);
			SetServoPosition(SERVO_2, temp);
			SetServoPosition(SERVO_3, temp);
			UARTprintf("%u  ",position);	 
			ch = getc();
		}
				 
		UARTprintf("\n");
	}
}
