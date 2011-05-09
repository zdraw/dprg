#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "utils/uartstdio.h"

#include "encoder.h"

volatile static encoder_count_t enc0 = 0, enc1 = 0;
static signed dir0, dir1;		// direction to count (up/down) for each encoder

void InitializeEncoders(tBoolean invert0, tBoolean invert1)
{
	// enable and configure the GPIO pins
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);	// enable the peripheral
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);	// enable the peripheral
	GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_6);	// configure pins as inputs
	GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6);	// configure pins as inputs
	
	// enable and configure the interrupts
	IntEnable(INT_GPIOB);					// enable interrupts for the periph
	IntEnable(INT_GPIOC);					// enable interrupts for the periph
	GPIOIntTypeSet(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_6, GPIO_BOTH_EDGES);	// configure the interrupts
	GPIOIntTypeSet(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6, GPIO_BOTH_EDGES);	// configure the interrupts
	GPIOPinIntEnable(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_6);	// enable the interrupt for the pins
	GPIOPinIntEnable(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6);	// enable the interrupt for the pins
	
	dir0 = invert0 ? -1 : 1;
	dir1 = invert1 ? -1 : 1;
}

void GetEncoderCounts(encoder_count_t *pEncCount0, encoder_count_t *pEncCount1)
{
	*pEncCount0 = enc0;
	*pEncCount1 = enc1;
}

encoder_count_t GetEncoderCount(encoder_t encoder)
{
	switch(encoder)
	{
	case ENCODER_0:
		return enc0;
	case ENCODER_1:
		return enc1;
	default:
		return 0;
	}
}

void PresetEncoderCounts(encoder_count_t new_count0, encoder_count_t new_count1)
{
	enc0 = new_count0;
	enc1 = new_count1;
}

void PresetEncoderCount(encoder_t encoder, encoder_count_t new_count)
{
	switch(encoder)
	{
	case ENCODER_0:
		enc0 = new_count;
		break;
	case ENCODER_1:
		enc1 = new_count;
		break;
	}
}

void EncoderInterruptHandler(void)
{
	unsigned static char lastState0 = 0;
	unsigned static char lastState1 = 0;
	unsigned char pin;
	unsigned port;

	// ENC1A PB4
	// ENC1B PC5
	// ENC2A PB6
	// ENC2B PC6
	/*
	0 => 1
	1 => 3
	2 => 0
	3 => 2

	0 => 2
	1 => 0
	2 => 3
	3 => 1
	*/
	
	if((GPIOPinIntStatus(port=GPIO_PORTC_BASE, false) & (pin=GPIO_PIN_5))
		|| GPIOPinIntStatus(port=GPIO_PORTB_BASE, false) & (pin=GPIO_PIN_4))
	{
		// encoder 0 fired!
		unsigned char state = (GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_4) ? 2 : 0);
		state += (GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_5) ? 1 : 0);
		if((lastState0 == 0 && state == 1)
	    || (lastState0 == 1 && state == 3)
	    || (lastState0 == 2 && state == 0)
	    || (lastState0 == 3 && state == 2)) {
			enc0 += dir0;
		}		 
		else if((lastState0 == 0 && state == 2)
	    || (lastState0 == 1 && state == 0)
	    || (lastState0 == 2 && state == 3)
	    || (lastState0 == 3 && state == 1)) {
			enc0 -= dir0;
		}	  
		lastState0 = state;
	}
	else if(GPIOPinIntStatus(port=GPIO_PORTC_BASE, false) & (pin=GPIO_PIN_6)
			|| GPIOPinIntStatus(port=GPIO_PORTB_BASE, false) & (pin=GPIO_PIN_6))
	{											  
		unsigned char state = (GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_6) ? 2 : 0);
		state += (GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_6) ? 1 : 0);
		if((lastState1 == 0 && state == 1)
	    || (lastState1 == 1 && state == 3)
	    || (lastState1 == 2 && state == 0)
	    || (lastState1 == 3 && state == 2)) {
			enc1 += dir1;
		}		 
		else if((lastState1 == 0 && state == 2)
	    || (lastState1 == 1 && state == 0)
	    || (lastState1 == 2 && state == 3)
	    || (lastState1 == 3 && state == 1)) {
			enc1 -= dir1;
		}
		lastState1 = state;
	} else return;
	
	GPIOPinIntClear(port, pin);
}
	
