#include "inc/hw_types.h"
#include "motorPID.h"
#include "RASLib/motor.h"
#include "RASLib/timer.h"

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

signed char saturate(signed long value)
{
	if (value > MAX)
		return MAX;
	if (value < MIN)
		return MIN;
	return value;
}

void AccelerateToMax(void) {
	power_t motor1 = 0;
	power_t motor2 = 0;
	
	while(1) {
		motor1++;
		motor2++;
		SetMotorPowers(motor1, motor2);
		if(motor1 == MAX || motor2 == MAX) {
			break;
		}
		Wait(10);
	}
}
