#include "inc/hw_types.h"               // tBoolean
#include "inc/hw_memmap.h"             // input/output over UART
#include "utils/uartstdio.h"    // input/output over UART
#include "driverlib/uart.h"             // input/output over UART
#include "driverlib/gpio.h"             // input/output over UART
#include "driverlib/adc.h"
#include "RASLib/init.h"
#include "RASLib/Timer.h"             // input/output over UART
#include "RASLib/servo.h"
#include "RASLib/uart.h"
#include "RASLib/motor.h"
#include "RASLib/encoder.h"
#include "TableTop.h"


extern void LineFollow(void);
extern void SquareDance(void);


void InitializeADC(char a) {
	ADCSequenceConfigure(ADC_BASE,a, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC_BASE, a, 0, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH0);
	ADCSequenceEnable(ADC_BASE, a);
}

long GetADCValue(char a) {
	unsigned long ADCValue = 0;
	ADCProcessorTrigger(ADC_BASE, a ); 
	while(!ADCIntStatus(ADC_BASE, a, false)); 
	ADCSequenceDataGet(ADC_BASE, a, &ADCValue);
	Wait(10);
	return ADCValue;
}

void waitForStartup(){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_3 | GPIO_PIN_2 );

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


char codeSelect(){ return GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5) >> 2; }
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
	InitializeEncoders(false, true);

	UARTprintf("%d\n", codeSelect());
	LineFollow();
	while(1);
    // Now we start the fun
	switch(2){
		case 1:
		    LineFollow();
			break;
		case 2:
		    SquareDance();
			break;
		case 4:
		    figureEight();
			break;
		case 9:
		    crossTable();
			break;
		case 10:			   
		    CubeAttack();
			break;
		case 12:			   
		    //tableTop(BOX);
			break;
	}
	while(1);	
}
