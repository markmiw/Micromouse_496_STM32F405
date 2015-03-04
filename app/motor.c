#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_gpio.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_rcc.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_tim.h"

#include "motor.h"
#include "led.h"

//Data structure for TIM configuration
static TIM_HandleTypeDef TIM_HandleStructure;
static TIM_HandleTypeDef TIM2_HandleStructure;
static TIM_OC_InitTypeDef sConfig;

typedef struct {
	int speed;
} motor;

motor leftMotor, rightMotor;

int count;

void initMotor() {
	leftMotor.speed = 0;
	rightMotor.speed = 0;
	count = 0;

	//Data structure for GPIO configuration
	GPIO_InitTypeDef GPIO_InitStructure;

	//Enable GPIO clock for LED module (B)
	__GPIOB_CLK_ENABLE();

	//Enable TIM clock for PWM (2 & 3 & 4)
	__TIM2_CLK_ENABLE();
	__TIM3_CLK_ENABLE();
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

	//Left motor PWM pin
	GPIO_InitStructure.Pin = GPIO_PIN_6;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

	//Right motor PWM pin
	GPIO_InitStructure.Pin = GPIO_PIN_8;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.Alternate = GPIO_AF2_TIM3;

	//Buzzer
	GPIO_InitStructure.Pin = GPIO_PIN_4;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

	//Configure data structure for TIM handler
	TIM_HandleStructure.Instance = TIM4;
	TIM_HandleStructure.Init.Period = PERIOD;
	TIM_HandleStructure.Init.Prescaler = 0;
	TIM_HandleStructure.Init.ClockDivision = 0;
	TIM_HandleStructure.Init.CounterMode = TIM_COUNTERMODE_DOWN;
	HAL_TIM_PWM_Init(&TIM_HandleStructure);

	TIM2_HandleStructure.Instance = TIM2;
	TIM2_HandleStructure.Init.Period = 16000;
	TIM2_HandleStructure.Init.Prescaler = 0;
	TIM2_HandleStructure.Init.ClockDivision = 0;
	TIM2_HandleStructure.Init.CounterMode = TIM_COUNTERMODE_DOWN;
	HAL_TIM_Base_Init(&TIM2_HandleStructure);

	sConfig.OCMode = TIM_OCMODE_PWM1;
	sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfig.Pulse = 0;

	HAL_TIM_PWM_ConfigChannel(&TIM_HandleStructure, &sConfig, TIM_CHANNEL_1);
	HAL_TIM_PWM_ConfigChannel(&TIM_HandleStructure, &sConfig, TIM_CHANNEL_3);

	HAL_TIM_PWM_Start(&TIM_HandleStructure, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&TIM_HandleStructure, TIM_CHANNEL_3);
	HAL_TIM_Base_Start_IT(&TIM2_HandleStructure);

	TIM_HandleStructure.Instance = TIM3;
	TIM_HandleStructure.Init.Period = 4000;
	HAL_TIM_PWM_Init(&TIM_HandleStructure);

	HAL_TIM_PWM_ConfigChannel(&TIM_HandleStructure, &sConfig, TIM_CHANNEL_1);

	HAL_TIM_PWM_Start(&TIM_HandleStructure, TIM_CHANNEL_1);

	setDirection(LEFTMOTOR, FORWARD);
	setDirection(RIGHTMOTOR, FORWARD);

	return;
}

void setBuzzer(int period) {
	TIM_HandleStructure.Instance = TIM3;
	TIM_HandleStructure.Init.Period = period;
	HAL_TIM_PWM_Init(&TIM_HandleStructure);

	__HAL_TIM_SetCompare(&TIM_HandleStructure, TIM_CHANNEL_1, period/2);
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
	TIM_HandleStructure.Instance = TIM4;
	TIM_HandleStructure.Init.Period = PERIOD;
	HAL_TIM_PWM_Init(&TIM_HandleStructure);
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

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	count++;

	if (count == 1) {
		setLED(BLUE, ON);
	}

	if (count == 2) {
		setLED(GREEN, ON);
	}

	if (count >= 1000) {
		count = 0;
		toggleLED(WHITE);
	}
}
