#define LEFTENCODER  0
#define RIGHTENCODER 1

void initEncoder();
int readEncoder(int channel);
void resetEncoder(int channel);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
