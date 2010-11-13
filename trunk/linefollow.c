#include "inc/hw_types.h"	// tBoolean				
#include "driverlib/uart.h"		// input/output over UART
#include "utils/uartstdio.h"	// input/output over UART
#include "RASLib/init.h"
#include "RASLib/encoder.h"
#include "RASLib/motor.h"
#include "RASLib/uart.h"
#include "RASLib/linesensor.h"
#include "driverlib/gpio.h"	
#include <stdlib.h>
#include <ctype.h>

#include "motorPID.h"

#define PRINT_LS(ls)	for(i=0; i<NUM_SENSORS; ++i)\
							UARTputc((ls&(1<<i)) ? '1' : '0')


#define WHITE '0'
#define BLACK '1'

extern void InitializeLineSensor(void);

signed int getLineError(linesensor_t raw, int dir)
{
	static signed int last = 0;
	unsigned int lineWidth = 0;	                          //width
	signed int firstSensor = -1;
	int i;
	signed err;	

	for(i=0;i<NUM_SENSORS;++i) {
		if(raw&(1<<i)) {
			if(!lineWidth) {
				if(firstSensor == -1)
					firstSensor = i;
			}
			lineWidth++;
		}
	}
	
	if(firstSensor == -1)
		return last;
	
	if(firstSensor >= 4)
		firstSensor++;

	err = firstSensor + lineWidth/2 - 4;
	last = err;
	
	return err;

}

void LineFollow(void)
{	
	signed int pid;		 
	static signed int accum_error;
	static signed int last_error;
	signed int LeftMotor;
	signed int RightMotor;

	LockoutProtection();  //lockout protection
	InitializeMCU();
	InitializeUART();	
	InitializeMotors(true, false);
	InitializeEncoders(true, false);
	InitializeLineSensor();
	SetDischargeTime(400);

	while(1)
	{
		pid = PID(getLineError(ReadLineSensor(), LEFT), &accum_error, &last_error, 30, 0, 20);
	
		LeftMotor = saturate(MAX + pid);
		RightMotor = saturate(MAX - pid);
		
		SetMotorPowers(LeftMotor, RightMotor);
	}			  
}
