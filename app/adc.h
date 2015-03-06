#define LEFT_DET      0
#define LEFT_CEN_DET  1
#define RIGHT_CEN_DET 2
#define RIGHT_DET     3
#define VOLT_DET      4
#define GYRO          5

void initADC();
uint32_t readADC(int channel);
uint32_t readBattery(void);
float rightSensorConversion(uint32_t reading);
float leftSensorConversion(uint32_t reading);
float rightCenterSensorConversion(uint32_t reading);
float leftCenterSensorConversion(uint32_t reading);
//void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);
