#include <stdlib.h>
#include <ctype.h>
#include "inc/hw_types.h"		// tBoolean
#include "driverlib/adc.h"		// ADC functions
#include "driverlib/uart.h"
#include "utils/uartstdio.h"	// input/output over UART
#include "RASLib/init.h"
#include "RASLib/encoder.h"
#include "RASLib/linesensor.h"
#include "RASLib/motor.h"
#include "RASLib/servo.h"

void Initialize_Sonar(void)
{	//uses trig = PA2 as output and echo = PA3 as input
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_6);
	GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_7);
	//SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
	//TimerConfigure(TIMER2_BASE, TIMER_CFG_32_RTC);
}
unsigned long Sonar_Get(void)
{	 
	unsigned long timecount = 0;	
	GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, 0xFF); //set trigger high
	WaitUS(10);										 //wait 10us
	GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, 0x00); //set trigger low
	while(!(GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_7))); //wait until echo is high
	//TimerLoadSet(TIMER2_BASE, TIMER_BOTH, 0); 
	//TimerRTCEnable(TIMER2_BASE);								  //start timer
	while(GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_7) && timecount < 100000)//wait until echo is low
		timecount++;
	//TimerRTCDisable(TIMER2_BASE); //stop timer
	//timecount = TimerValueGet(TIMER2_BASE, TIMER_BOTH);//take timer val
	//UARTprintf("%u", timecount);
	return (timecount/500); //and divide it by 500 for inches and 200 for cm
}
