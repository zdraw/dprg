#define MAX 40
#define MIN -40
#define LEFT 0
#define RIGHT 1

int PID(int error, int* accum_error, int* last_error, int p, int i, int d);
signed char saturate(signed long value);
