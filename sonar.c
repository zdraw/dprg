#include <stdlib.h>
#include <ctype.h>
#include "inc/hw_types.h"		// tBoolean
#include "driverlib/adc.h"		// ADC functions
#include "driverlib/uart.h"
#include "utils/uartstdio.h"	// input/output over UART
#include "RASLib/init.h"
#include "RASLib/linesensor.h"
#include "RASLib/motor.h"
#include "RASLib/servo.h"

void Initialize_Sonar(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_6);
	GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_7);
}

/*unsigned long Sonar_Get(void)
{	 
	unsigned long timecount = 0;	
	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6, 0xFF); //set trigger high
	WaitUS(10);										 //wait 10us
	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6, 0x00); //set trigger low
	while(!(GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_7))); //wait until echo is high
	while(GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_7) && timecount < 100000)//wait until echo is low
		timecount++;
	return (timecount/500); //and divide it by 500 for inches and 200 for cm
}*/

long Sonar_Get()
{
 	unsigned long out = 0;
	unsigned long count = 0;
	GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_6,0xFF);			// send high to Trigger
	WaitUS(10);
	GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_6,0x00);			// send low to Trigger
	while(!GPIOPinRead(GPIO_PORTD_BASE,GPIO_PIN_7) && count < 100000)	// wait for Echo 
		count++;
	while(GPIOPinRead(GPIO_PORTD_BASE,GPIO_PIN_7))
		out++;
	return out;
}

