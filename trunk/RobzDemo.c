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
char codeSelect(){
    unsigned char num;



	return 0;
}
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
	while(1){
		switch(codeSelect()){
			case 0:
				break;
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				break;
			case 5:
				break;
			case 6:
				break;
			case 7:
				break;
			case 8:
				break;
		}	
	}
	
	
			
	//while(1) {	
	/*	UARTprintf("\nROBZ DEMO\n");
		UARTprintf("  0=UART Demo\n  1=Motor Demo\n");
		UARTprintf("  2=Servo Demo\n  3=Line Sensor\n");
		UARTprintf("  4=IR Sensor Demo\n  5=Encoders Demo\n");
		UARTprintf("Motor PWM Pulses Per Second: %d",g_ulPWMTicksPerSecond / 256);
		//UARTprintf("enc0:%d  enc1:%d      \r\n",GetEncoderCount(ENCODER_0),GetEncoderCount(ENCODER_1));
		
		UARTprintf(">> ");
		ch = getc();
		putc(ch);
		UARTprintf("\n");

		if (ch == '0') {
			UARTprintf("\nUART Demo\n");
			uartDemo();	 
		}
		else if (ch == '1') {
			UARTprintf("\nMotor Demo\n");
			initMotors();
			motorDemo(); 
		}
		else if (ch == '2') {
			UARTprintf("\nServo Demo\n");
			initServo();
			servoDemo();   
		}
		else if (ch == '3') {			   
			UARTprintf("\nLine Sensor Demo\n");
			initLineSensor();		  
			lineSensorDemo();	  
		}
		else if (ch == '4') {	   
			UARTprintf("\nIR Sensor Demo\n");
			initIRSensor();
			IRSensorDemo();	 
		}
		else if (ch == '5') {
			UARTprintf("\nEncoders Demo\n");
			initEncoders();
			encoderDemo();
		}	   
	}		   */
}
