typedef enum
{
	LEFTENCODER,
	RIGHTENCODER
}Encoder;

/* Public Functions */
void initEncoder();
int readEncoder(Encoder channel);
void resetEncoder(Encoder channel);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

/* Private Functions */
static void incrementEncoder(Encoder channel);
static void decrementEncoder(Encoder channel);
