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

#define PRINT_LS(ls)	for(i=0; i<NUM_SENSORS; ++i)\
							UARTputc((ls&(1<<i)) ? '1' : '0')


#define WHITE '0'
#define BLACK '1'
#define MAX 127
#define MIN -128
#define LEFT 0
#define RIGHT 1

extern void InitializeLineSensor(void);

int PID(int error, int* accum_error, int* last_error, int p, int i, int d)
{
  int output = 0;
  (*accum_error)=((*accum_error)+error);
  output = (p*error) + d*(error-(*last_error)) + i*(*accum_error);
  
  #ifdef DEBUG_PID
  printf("PID e:%d o:%d\r\n",error,output);
  #endif 
   
  (*last_error)=error;

  return output;
}

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

signed char saturate(signed int value)
{
	if (value > MAX)
		return MAX;
	if (value < MIN)
		return MIN;
	return value;
}

void UpdateMotors(char lineSensor)
{
	signed int pid;		 
	static signed int accum_error;
	static signed int last_error;
	signed int LeftMotor;
	signed int RightMotor;

	pid = PID(getLineError(lineSensor, LEFT), &accum_error, &last_error, 30, 0, 20);

	LeftMotor = saturate(MAX + pid);
	RightMotor = saturate(MAX - pid);
	
	SetMotorPowers(LeftMotor, RightMotor);
}

void LineFollow(void)
{
	LockoutProtection();  //lockout protection
	InitializeMCU();
	InitializeUART();	
	InitializeMotors(true, false);
	InitializeEncoders(true, false);
	InitializeLineSensor();
	SetDischargeTime(400);
	while(1)
	{
		//int i;
		//char lineSensor = ReadLineSensor();	 //bit 7 is left side
		//PRINT_LS(lineSensor);
		//UARTprintf("  Error = %d", getLineError(lineSensor, LEFT));
		//NL;
		UpdateMotors(ReadLineSensor());
		/*for(i=0; i<NUM_SENSORS; ++i)\
				UARTputc((ls&(1<<i)) ? '1' : '0');*/
		//PID(getError(linesensor))	returns correction for motor values, still need to saturate 
		//before setting new values
	}			  
}
