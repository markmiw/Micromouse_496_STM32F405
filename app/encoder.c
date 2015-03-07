#include "components/coocox-master/STM32F405xx_cmsisboot/source/stm32f405xx.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_cortex.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_gpio.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_rcc.h"

#include "encoder.h"
#include "led.h"

static int leftCount, rightCount;

void initEncoder()
{
	resetEncoder(LEFTENCODER);
	resetEncoder(RIGHTENCODER);

	//Data structure for GPIO configuration
	GPIO_InitTypeDef GPIO_InitStructure;

	//Enable GPIO clock for LED module (A & B)
	__GPIOA_CLK_ENABLE();
	__GPIOB_CLK_ENABLE();

	//Configure data structure for GPIO input
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;

	//Left encoder channel 2
	GPIO_InitStructure.Pin = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

	//Right encoder channel 2
	GPIO_InitStructure.Pin = GPIO_PIN_1;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

	//Configure data structure for GPIO interrupt
	GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;


	//Left encoder channel 1
	GPIO_InitStructure.Pin = GPIO_PIN_15;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

	//Right encoder channel 1
	GPIO_InitStructure.Pin = GPIO_PIN_0;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

int readEncoder(Encoder channel)
{
	if (channel == LEFTENCODER) return leftCount;
	else return rightCount;
}

void resetEncoder(Encoder channel)
{
	if (channel == LEFTENCODER) leftCount = 0;
	else rightCount = 0;
}

static void incrementEncoder(Encoder channel)
{
	if (channel == LEFTENCODER) leftCount++;
	else rightCount++;
}

static void decrementEncoder(Encoder channel)
{
	if (channel == LEFTENCODER) leftCount--;
	else rightCount--;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == GPIO_PIN_0)
	{
		if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_SET) incrementEncoder(RIGHTENCODER);
		else decrementEncoder(RIGHTENCODER);
	}
	else if (GPIO_Pin == GPIO_PIN_15)
	{
		if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3) == GPIO_PIN_SET) incrementEncoder(LEFTENCODER);
		else decrementEncoder(LEFTENCODER);
	}
}
