#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_gpio.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_rcc.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_tim.h"

void initMotor() {
	//Data structure for GPIO configuration
	GPIO_InitTypeDef GPIO_InitStructure;

	//Data structure for TIM configuration
	TIM_Base_InitTypeDef TIM_InitStructure;
	TIM_HandleTypeDef    TIM_HandleStructure;
	TIM_OC_InitTypeDef   TIM_OCInitStructure;

	//Enable GPIO clock for LED module (B)
	__GPIOB_CLK_ENABLE();

	//Enable TIM clock for PWM (4)
	__TIM4_CLK_ENABLE();

	//Configure data structure for GPIO output
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_MEDIUM;

	//Left motor directional bit
	GPIO_InitStructure.Pin = GPIO_PIN_7;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

	//Right motor directional bit
	GPIO_InitStructure.Pin = GPIO_PIN_9;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

	//Configure data structure for GPIO AF
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Alternate = GPIO_AF2_TIM4;

	// motor PWM pin
	GPIO_InitStructure.Pin = GPIO_PIN_6;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

	// motor PWM pin
	GPIO_InitStructure.Pin = GPIO_PIN_8;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

//	//Configure data structure for PWM output
//	TIM_InitStructure.Prescaler = 0;
//	TIM_InitStructure.Period = 600;
//	TIM_InitStructure.CounterMode = TIM_COUNTERMODE_DOWN;
//	TIM_InitStructure.ClockDivision = TIM_CLOCKDIVISION_DIV1;
//
//	TIM_OCInitStructure.Mode = TIM_OCMODE_PWM1;
//	TIM_OCInitStructure.OCPolarity = TIM_OCPOLARITY_HIGH;

//	TIM_HandleStructure.Init = &TIM_InitStructure;
//
//	//
//	TIM_HandleStructure.Channel = HAL_TIM_ACTIVE_CHANNEL_1;
//
//	//
//	TIM_HandleStructure.Channel = HAL_TIM_ACTIVE_CHANNEL_3;

	return;
}
