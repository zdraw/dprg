#include "TableTop.h"				 
#include "inc/hw_memmap.h"             // input/output over UART
#include "inc/hw_types.h"		// tBoolean
#include "utils/uartstdio.h"	// input/output over UART
#include "driverlib/uart.h"		// input/output over UART
#include "RASLib/init.h"
#include "driverlib/adc.h"		// ADC functions
#include "RASLib/encoder.h"
#include "RASLib/motor.h"
#include "driverlib/uart.h"

#include "motorPID.h"
#include "sonar.h"
				   
unsigned long ulADCValue1;
int getDistance(void) // Make this function work. Gets value from IR sensor
{
	int distance;

	ADCProcessorTrigger(ADC0_BASE, 1); 
	while(!ADCIntStatus(ADC0_BASE, 1, false)); 
	ADCSequenceDataGet(ADC0_BASE, 1, &ulADCValue1);
	distance = (900 - ulADCValue1);
	
	UARTprintf("%u", distance);
	
	return distance; 
}

void crossTable(void)
{
	signed int LeftMotor = MAX;
	signed int RightMotor = MAX;
	int dist=0;
	int dist2=0;
	int spike = 30; // buffer to detect edge of table

	PresetEncoderCounts(0, 0); // Reset encoders

	// Go until end of table
	dist = getDistance();
	SetMotorPowers(LeftMotor, RightMotor);
	while (1)
	{
		dist2 = getDistance();
		if ( dist2 > (dist + spike) )
			break; // Edge of table
		
		dist = dist2;
	}
	
	// Reverse
	LeftMotor = MIN;
	RightMotor = MIN;
	SetMotorPowers(LeftMotor, RightMotor);

	// Interface with PID to make straight
	while ( GetEncoderCount(ENCODER_0) > 0 );

	SetMotorPowers(0,0);			
}

void CubeAttack(void) {
	Initialize_Sonar();
	UARTprintf("Testing Sonar\n");
	while(1) {
		UARTprintf("Sonar: %d\r", Sonar_Get());
	}
}
