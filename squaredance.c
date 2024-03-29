#include "TableTop.h"				 
#include "inc/hw_memmap.h"             // input/output over UART
#include "inc/hw_types.h"		// tBoolean
#include "utils/uartstdio.h"	// input/output over UART
#include "driverlib/uart.h"		// input/output over UART
#include "RASLib/init.h"
#include "driverlib/adc.h"		// ADC functions
#include "RASLib/motor.h"
#include "driverlib/uart.h"
#include "RASLib/timer.h"
#include "encoder.h"

#include "motorPID.h"

void UpdateMotors(signed long error)
{
	signed int pid;		 
	static signed int accum_error;
	static signed int last_error;
	signed int LeftMotor;
	signed int RightMotor;

	pid = PID(error, &accum_error, &last_error, 30, 0, 5);

	LeftMotor = saturate(MAX + pid);
	RightMotor = saturate(MAX - pid);
	
	SetMotorPowers(LeftMotor, RightMotor);
}

void SquareDance(void) {
	UARTprintf("WHEEEE\n");
	AccelerateToMax();
	PresetEncoderCounts(0, 0);
	while(1) {
		signed long enc1 = GetEncoderCount(ENCODER_0);
		signed long enc2 = GetEncoderCount(ENCODER_1);

		UpdateMotors((enc2-enc1)/1000);

		/*if(enc1 > 56000 || enc2 > 56000) {
			SetMotorPowers(0,0);
			while(1);
		}*/
		
	}
}
