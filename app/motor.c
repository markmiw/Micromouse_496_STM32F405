/* File Name: motor.c
 * Project: EE 396
 * Team: TSM
 * Members: Mark Miw
 *          Steven Chen
 *          Joseph Felix Jr.
 */

#include "motor.h"

//Data structure for TIM configuration
extern TIM_HandleTypeDef buzzerHandler;
extern TIM_HandleTypeDef brakeHandler;
extern TIM_HandleTypeDef motorHandler;
extern TIM_HandleTypeDef htim2;

static int leftMotorSpeed;
static int rightMotorSpeed;
static uint16_t counter;

static int oldEncoderR;
static int oldEncoderL;
static uint8_t thresh;

static double targetRightVelocity;
static double targetLeftVelocity;
static double currentRightVelocity;
static double currentLeftVelocity;

/* Private Functions */
static void setDirection(Motor channel, Direction state);
static void velocityCallBack();
extern uint16_t vel_k_R;
extern uint16_t vel_k_L;

void brake() {
	int right = getEncoder(RIGHTENCODER);
	int left = getEncoder(LEFTENCODER);
	thresh = 0;
	counter = 0;
	oldEncoderR = 0;
	oldEncoderL = 0;
	vel_k_R = BRAKE_k_R;
	vel_k_L = BRAKE_k_L;
	setVelocity(0);
	while(getCurrentVelocity(RIGHTMOTOR) != 0 && getCurrentVelocity(LEFTMOTOR) != 0);
	brakeCorrection(left,right);
	HAL_TIM_Base_Stop_IT(&htim2);
	setSpeed(RIGHTMOTOR,0);
	setSpeed(LEFTMOTOR,0);
	vel_k_R = VELOCITY_k_R;
	vel_k_L = VELOCITY_k_L;
}

void hardBrake() {
	thresh = 0;
	counter = 0;
	oldEncoderR = 0;
	oldEncoderL = 0;
	vel_k_R = BRAKE_k_R;
	vel_k_L = BRAKE_k_L;
	setVelocity(0);
	while(getCurrentVelocity(RIGHTMOTOR) != 0 && getCurrentVelocity(LEFTMOTOR) != 0);
	HAL_TIM_Base_Stop_IT(&htim2);
	setSpeed(RIGHTMOTOR,0);
	setSpeed(LEFTMOTOR,0);
	vel_k_R = VELOCITY_k_R;
	vel_k_L = VELOCITY_k_L;
}

void brakeRight() {
	setRightVelocity(0);
}

void brakeLeft() {
	setLeftVelocity(0);
}


void setVelocity(double velocity) {
	targetRightVelocity = velocity;
	targetLeftVelocity = velocity;
	HAL_TIM_Base_Start_IT(&htim2);
}

void setRightVelocity(double velocity) {
	targetRightVelocity = velocity;
}

void setLeftVelocity(double velocity) {
	targetLeftVelocity = velocity;
}

static void setDirection(Motor channel, Direction state) {
	if (channel == LEFTMOTOR) {
		if (state == FORWARD) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
		else HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
	}
	else {
		if (state == FORWARD) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
		else HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
	}
}

static int currentSpeed(Motor channel) {
	if (channel == LEFTMOTOR) return leftMotorSpeed;
	else return rightMotorSpeed;
}

void setSpeed(Motor channel, int speed) {
	int neg = 1;
	if (speed >= 0) {
		setDirection(channel, FORWARD);
	}
	else {
		neg = -1;
		speed = ~speed;
		setDirection(channel, BACKWARD);
	}

	if (channel == LEFTMOTOR) {
		if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == GPIO_PIN_SET) {
			speed = PERIOD - speed;
			leftMotorSpeed = (1000 - speed)*neg;
		}
		else {
			leftMotorSpeed = speed*neg;
		}
		__HAL_TIM_SetCompare(&motorHandler, TIM_CHANNEL_1, speed);
	}
	else {
		if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9) == GPIO_PIN_SET) {
			speed = PERIOD - speed;
			rightMotorSpeed = (1000 - speed)*neg;
		}
		else {
			rightMotorSpeed = speed*neg;
		}
		__HAL_TIM_SetCompare(&motorHandler, TIM_CHANNEL_3, speed);
	}
}

double getCurrentVelocity(Motor channel) {
	if (channel == RIGHTMOTOR) return currentRightVelocity;
	else return currentLeftVelocity;
}

double getTargetVelocity(Motor channel) {
	if (channel == RIGHTMOTOR) return targetRightVelocity;
	else return targetLeftVelocity;
}

static void velocityCallBack() {
	int currentEncoderR = getEncoder(RIGHTENCODER);
	int currentEncoderL = getEncoder(LEFTENCODER);

	int currRspeed = currentSpeed(RIGHTMOTOR);
	int currLspeed = currentSpeed(LEFTMOTOR);

	currentRightVelocity = (double)(currentEncoderR - oldEncoderR)*R_ENCODER_DIST/0.001;
	currentLeftVelocity = (double)(currentEncoderL - oldEncoderL)*L_ENCODER_DIST/0.001;

	if ( currentRightVelocity > targetRightVelocity ) setSpeed(RIGHTMOTOR, currRspeed-vel_k_R);
	else if ( currentRightVelocity < targetRightVelocity ) setSpeed(RIGHTMOTOR, currRspeed+vel_k_R);
	else setSpeed(RIGHTMOTOR,currRspeed);

	if ( currentLeftVelocity > targetLeftVelocity ) setSpeed(LEFTMOTOR, currLspeed-vel_k_L);
	else if ( currentLeftVelocity < targetLeftVelocity ) setSpeed(LEFTMOTOR, currLspeed+vel_k_L);
	else setSpeed(LEFTMOTOR,currLspeed);

	oldEncoderR = currentEncoderR;
	oldEncoderL = currentEncoderL;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	//Buzzer interrupt
	if (htim->Instance == TIM3) HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);

	else if (htim->Instance == TIM2) //Velocity Timer
	{
		velocityCallBack();
	}
	else if (htim->Instance == TIM5) //Millisecond Timer
	{

	}
}
