#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_gpio.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_rcc.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_tim.h"

#include "motor.h"

//Data structure for TIM configuration
static TIM_HandleTypeDef TIM_HandleStructure;
static TIM_OC_InitTypeDef sConfig;

typedef struct {
	int speed;
} motor;

motor leftMotor, rightMotor;

void initMotor() {
	leftMotor.speed = 0;
	rightMotor.speed = 0;

	//Data structure for GPIO configuration
	GPIO_InitTypeDef GPIO_InitStructure;

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

	//left motor PWM pin
	GPIO_InitStructure.Pin = GPIO_PIN_6;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

	//right motor PWM pin
	GPIO_InitStructure.Pin = GPIO_PIN_8;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

	//Configure data structure for TIM handler
	TIM_HandleStructure.Instance = TIM4;
	TIM_HandleStructure.Init.Period = PERIOD;
	TIM_HandleStructure.Init.Prescaler = 0;
	TIM_HandleStructure.Init.ClockDivision = 0;
	TIM_HandleStructure.Init.CounterMode = TIM_COUNTERMODE_DOWN;
	HAL_TIM_PWM_Init(&TIM_HandleStructure);

	sConfig.OCMode = TIM_OCMODE_PWM1;
	sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfig.Pulse = 0;

	HAL_TIM_PWM_ConfigChannel(&TIM_HandleStructure, &sConfig, TIM_CHANNEL_1);
	HAL_TIM_PWM_ConfigChannel(&TIM_HandleStructure, &sConfig, TIM_CHANNEL_3);

	HAL_TIM_PWM_Start(&TIM_HandleStructure, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&TIM_HandleStructure, TIM_CHANNEL_3);

	setDirection(LEFTMOTOR, FORWARD);
	setDirection(RIGHTMOTOR, FORWARD);

	return;
}

void setDirection(int channel, int direction) {
	switch (channel) {
	case 0: //Left motor
		if (direction) { //Forward
			if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == GPIO_PIN_RESET) {
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);

				setSpeed(channel, leftMotor.speed);
			}
		} else { //Backward
			if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == GPIO_PIN_SET) {
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
				leftMotor.speed = PERIOD - leftMotor.speed;
				setSpeed(channel, leftMotor.speed);
			}
		} break;
	case 1: //Right motor
		if (direction) { //Forward
			if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9) == GPIO_PIN_RESET) {
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);

				setSpeed(channel, rightMotor.speed);
			}
		} else { //Backward
			if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9) == GPIO_PIN_SET) {
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
				rightMotor.speed = PERIOD - rightMotor.speed;
				setSpeed(channel, rightMotor.speed);
			}
		} break;

	}

	return;
}

void setSpeed(int channel, int speed) {
	speed = PERIOD - speed;
	switch (channel) {
	case 0: //Left motor
		if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == GPIO_PIN_RESET) {
			speed = PERIOD - speed;
		} leftMotor.speed = speed;
		__HAL_TIM_SetCompare(&TIM_HandleStructure, TIM_CHANNEL_1, speed);
		break;
	case 1: //Right motor
		if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9) == GPIO_PIN_RESET) {
			speed = PERIOD - speed;
		} rightMotor.speed = speed;
		__HAL_TIM_SetCompare(&TIM_HandleStructure, TIM_CHANNEL_3, speed);
		break;
	}


	return;
}

void toggleDirection(int channel) {
	switch (channel) {
	case 0: //Left motor
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
		setSpeed(channel, leftMotor.speed);
		break;
	case 1: //Right motor
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_9);
		setSpeed(channel, rightMotor.speed);
		break;
	}

	return;
}

