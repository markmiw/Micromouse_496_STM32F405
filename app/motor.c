#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_gpio.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_rcc.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_tim.h"

#include "encoder.h"
#include "led.h"
#include "motor.h"

//Data structure for TIM configuration
TIM_HandleTypeDef buzzerHandler;
TIM_HandleTypeDef leftHandler;
TIM_HandleTypeDef rightHandler;
TIM_HandleTypeDef motorHandler;
TIM_OC_InitTypeDef sConfig;

static int leftMotorSpeed;
static int rightMotorSpeed;
static int countLeft;
static int countRight;
static int timeFactor;
static int currentTime;
static int targetDistance;

void initMotor() {
	leftMotorSpeed  = 0;
	rightMotorSpeed = 0;
	countLeft       = 0;
	countRight      = 0;
	timeFactor      = 0;
	targetDistance  = 0;

	//Data structure for GPIO configuration
	GPIO_InitTypeDef GPIO_InitStructure;

	//Enable GPIO clock for LED module (B)
	__GPIOB_CLK_ENABLE();

	//Enable TIM clock for PWM (2 & 3 & 4)
	__TIM2_CLK_ENABLE(); //TIM for countLeft
	__TIM3_CLK_ENABLE(); //TIM for buzzer
	__TIM4_CLK_ENABLE(); //TIM for motors
	__TIM5_CLK_ENABLE(); //TIM for countRight

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

	//Buzzer
	GPIO_InitStructure.Pin = GPIO_PIN_4;
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

	//Configure TIM for motors
	motorHandler.Instance = TIM4;
	motorHandler.Init.Period = PERIOD;
	motorHandler.Init.Prescaler = 0;
	motorHandler.Init.ClockDivision = 0;
	motorHandler.Init.CounterMode = TIM_COUNTERMODE_DOWN;
	HAL_TIM_PWM_Init(&motorHandler);

	sConfig.OCMode = TIM_OCMODE_PWM1;
	sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfig.Pulse = 0;

	HAL_TIM_PWM_ConfigChannel(&motorHandler, &sConfig, TIM_CHANNEL_1);
	HAL_TIM_PWM_ConfigChannel(&motorHandler, &sConfig, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&motorHandler, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&motorHandler, TIM_CHANNEL_3);

	//Configure TIM for buzzer
	buzzerHandler.Instance = TIM3;
	buzzerHandler.Init.Period = 42000;
	buzzerHandler.Init.Prescaler = 0;
	buzzerHandler.Init.ClockDivision = 0;
	buzzerHandler.Init.CounterMode = TIM_COUNTERMODE_DOWN;

	HAL_TIM_Base_Init(&buzzerHandler);
	HAL_TIM_Base_Stop_IT(&buzzerHandler);

	HAL_NVIC_EnableIRQ(TIM3_IRQn);

	//Configure TIM for countLeft
	leftHandler.Instance = TIM2;
	leftHandler.Init.Period = 42000;
	leftHandler.Init.Prescaler = 0;
	leftHandler.Init.ClockDivision = 0;
	leftHandler.Init.CounterMode = TIM_COUNTERMODE_DOWN;

	HAL_TIM_Base_Init(&leftHandler);
	HAL_TIM_Base_Stop_IT(&leftHandler);

	HAL_NVIC_EnableIRQ(TIM2_IRQn);

	//Configure TIM for countRight
	rightHandler.Instance = TIM5;
	rightHandler.Init.Period = 42000;
	rightHandler.Init.Prescaler = 0;
	rightHandler.Init.ClockDivision = 0;
	rightHandler.Init.CounterMode = TIM_COUNTERMODE_DOWN;

	HAL_TIM_Base_Init(&rightHandler);
	HAL_TIM_Base_Stop_IT(&rightHandler);

	setDirection(LEFTMOTOR, FORWARD);
	setDirection(RIGHTMOTOR, FORWARD);

	return;
}

void setBuzzer(int state) {
	if (state) {
		HAL_TIM_Base_Start_IT(&buzzerHandler);
	} else {
		HAL_TIM_Base_Stop_IT(&buzzerHandler);
	}
}

void setDirection(int channel, int direction) {
	switch (channel) {
	case 0: //Left motor
		if (direction) { //Forward
			if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == GPIO_PIN_RESET) {
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);

				setSpeed(channel, leftMotorSpeed);
			}
		} else { //Backward
			if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == GPIO_PIN_SET) {
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
				leftMotorSpeed = PERIOD - leftMotorSpeed;
				setSpeed(channel, leftMotorSpeed);
			}
		} break;
	case 1: //Right motor
		if (direction) { //Forward
			if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9) == GPIO_PIN_RESET) {
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);

				setSpeed(channel, rightMotorSpeed);
			}
		} else { //Backward
			if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9) == GPIO_PIN_SET) {
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
				rightMotorSpeed = PERIOD - rightMotorSpeed;
				setSpeed(channel, rightMotorSpeed);
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
		} leftMotorSpeed = speed;
		__HAL_TIM_SetCompare(&motorHandler, TIM_CHANNEL_1, speed);
		break;
	case 1: //Right motor
		if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9) == GPIO_PIN_RESET) {
			speed = PERIOD - speed;
		} rightMotorSpeed = speed;
		__HAL_TIM_SetCompare(&motorHandler, TIM_CHANNEL_3, speed);
		break;
	}


	return;
}

void toggleDirection(int channel) {
	switch (channel) {
	case 0: //Left motor
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
		setSpeed(channel, leftMotorSpeed);
		break;
	case 1: //Right motor
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_9);
		setSpeed(channel, rightMotorSpeed);
		break;
	}

	return;
}

/**
  * @brief  Move the robot a certain distance with ramp ups and ramp downs
  * @param  distance: 	Number of ticks for the robot to travel
  * @param  maxSpeed: 	Target speed limit that the robot will not pass
  * #oaram	dt:			Rate of time between speed increase
  * @retval Nothing
  */
void travelDistance(int distance, int maxSpeed, int dt) {
	countLeft = 0;
	currentTime = 0;
	targetDistance = distance;
	timeFactor = dt;
	setSpeed(LEFTMOTOR, 0);
	setSpeed(RIGHTMOTOR, 0);
	HAL_TIM_Base_Start_IT(&leftHandler);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	int rightRead;
	int bufferX, bufferY;

	if (htim->Instance == TIM3)
	{
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);
	}

	else if (htim->Instance == TIM2) //leftHandler
	{
		countLeft++;
		if (countLeft >= 4)
		{
			countLeft = 0;
			currentTime++;
		}

		if (currentTime >= timeFactor)
		{
			currentTime = 0;
			rightRead = readEncoder(RIGHTENCODER);
			if (rightRead < targetDistance/2)
			{
				leftMotorSpeed = leftMotorSpeed - 10;
				rightMotorSpeed = rightMotorSpeed - 10;
				setSpeed(LEFTMOTOR, PERIOD - leftMotorSpeed);
				setSpeed(RIGHTMOTOR, PERIOD - rightMotorSpeed);
			}
			else if (rightRead < targetDistance && rightRead >= targetDistance/2)
			{
				leftMotorSpeed = leftMotorSpeed + 10;
				rightMotorSpeed = rightMotorSpeed + 10;
				setSpeed(LEFTMOTOR, PERIOD - leftMotorSpeed);
				setSpeed(RIGHTMOTOR, PERIOD - rightMotorSpeed);
			}
			else
			{
				setSpeed(LEFTMOTOR, 0);
				setSpeed(RIGHTMOTOR, 0);
				HAL_TIM_Base_Stop_IT(&leftHandler);
			}
		}
	}

	else if (htim->Instance == TIM5) //rightHandler
	{
		countRight++;
		if (countRight >= 4)
		{
			countRight = 0;
		}
	}
}
