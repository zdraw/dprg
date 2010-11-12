#include "inc/hw_types.h"		// tBoolean
#include "RobzDemo.h"
#include "utils/uartstdio.h"	// input/output over UART
#include "driverlib/uart.h"		// input/output over UART
#include "RASLib/init.h"
#include "RASLib/encoder.h"


void waitForStartup(){
	// Start w/ gate up
	SetServoPosition(SERVO_1,255);
	Wait(500);

    // Wait for hand to cover IR
	while(GetADCValue(0) < 500);
	Wait(100);

    // Wait for hand to leave IR
	while(GetADCValue(0) > 500);
	
	SetServoPosition(SERVO_1,140);
	Wait(10);
}
char codeSelect(){ return GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5) }
int main(void)
{		
	LockoutProtection();
	InitializeMCU();
	InitializeUART();	
	InitializeServos();

	// Initialize ADC 
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC);
	InitializeADC(0);
	InitializeADC(1);

	// Wave hand to start run
	waitForStartup();

	// Init motors (whine...)
	InitializeMotors(true, true);
	InitializeEncoders(true, false);

    // Now we start the fun
	switch(codeSelect()){
		case 1:
		    lineFollow();
			break;
		case 2:
		    squareDance();
			break;
		/*case 4:
		    figureEight();
			break;*/
		case 9:
		    tableTop(EDGE);
			break;
		case 10:			   
		    tableTop(CUBE);
			break;
		case 12:			   
		    tableTop(BOX);
			break;
	}	
}
