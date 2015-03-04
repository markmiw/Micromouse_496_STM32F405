#define SENSOR 4
#define LEFT_DET      0
#define LEFT_CEN_DET  1
#define RIGHT_CEN_DET 2
#define RIGHT_DET     3

void initUSART();
void sensorUSART(int sensor);
void fullSensorUSART();
void printUSART(int value);
void printStringUSART();
void printNL();
