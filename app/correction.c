/* File Name: correction.c
 * Project: EE 396
 * Team: TSM
 * Members: Mark Miw
 *          Steven Chen
 *          Joseph Felix Jr.
 */

#include "correction.h"

/* Nice P values
 * PR = 0.01
 * PL = 0.02
 *
 * Good for Speed 100
 * DR = 0.1
 * DL = 0.1
 *
 * Decent for Speed 200
 * PR = 0.0037
 * PL = 0.0077
 * DR = 0.001
 * DL = 0.001
 *
 * Decent
 * .007
 */

extern TIM_HandleTypeDef htim2;

void frontCorrection() {

	double k = 0.3;
	setVelocity(0);
	bool right = false;
	bool left = false;

	double currentFrontRight;
	double currentFrontLeft;

	double errorR;
	double errorL;

	while(!right || !left)
	{
		currentFrontRight = readSensor(RIGHT_DET);
		currentFrontLeft = readSensor(LEFT_DET);

		errorR = currentFrontRight - getWall(CENTERRIGHTFRONT);
		errorL = currentFrontLeft - getWall(CENTERLEFTFRONT);

		if (abs(errorR) < CORRECTION_FRONT_THRESH) {
			errorR = 0;
			brakeRight();
			right = true;
		}
		else {
			setRightVelocity(errorR*k);
		}
		if (abs(errorL) < CORRECTION_FRONT_THRESH && !left) {
			errorL = 0;
			brakeLeft();
			left = true;
		}
		else {
			setLeftVelocity(errorL*k);
		}
	}

	HAL_Delay(100);
	HAL_TIM_Base_Stop_IT(&htim2);
	setSpeed(RIGHTMOTOR,0);
	setSpeed(LEFTMOTOR,0);
}

void brakeCorrection(int startL, int startR) {
	double k = 0.1;
	bool right = false;
	bool left = false;

	int currentFrontRight;
	int currentFrontLeft;

	double errorR;
	double errorL;

	while(!right || !left)
	{
		currentFrontRight = getEncoder(RIGHTENCODER);
		currentFrontLeft = getEncoder(LEFTENCODER);

		errorR = startR - currentFrontRight;
		errorL = startL - currentFrontLeft;

		if (abs(errorR) < CORRECTION_FRONT_THRESH) {
			errorR = 0;
			brakeRight();
			right = true;
		}
		else {
			setRightVelocity(errorR*k);
		}
		if (abs(errorL) < CORRECTION_FRONT_THRESH && !left) {
			errorL = 0;
			brakeLeft();
			left = true;
		}
		else {
			setLeftVelocity(errorL*k);
		}
	}
	HAL_Delay(100);
	HAL_TIM_Base_Stop_IT(&htim2);
}

void correction(uint8_t wall, double base_speed) {

	double errorLeft;
	double errorRight;

	const double kP = .2;
	const double kD = 0.0001;

	double left_side_sensor = readSensor(LEFT_CEN_DET);
	double right_side_sensor = readSensor(RIGHT_CEN_DET);

	HAL_TIM_Base_Start_IT(&htim2);

	if (wall == 0) {
		if (right_side_sensor > getWall(FARRIGHTWALL)) right_side_sensor = getWall(FARRIGHTWALL);
		errorRight = getWall(IDEALRIGHTCENTER) - right_side_sensor;
		setLeftVelocity(base_speed - (errorRight * kP - (errorRight - lastErrorRight) * kD));
		setRightVelocity(base_speed + (errorRight * kP - (errorRight - lastErrorRight) * kD));
	}
	else if (wall == 1) {
		if (left_side_sensor > getWall(FARLEFTWALL)) left_side_sensor = getWall(FARLEFTWALL);
		errorLeft = getWall(IDEALLEFTCENTER) - left_side_sensor;
		setLeftVelocity(base_speed + (errorLeft * kP - (errorLeft - lastErrorLeft) * kD));
		setRightVelocity(base_speed - (errorLeft * kP - (errorLeft - lastErrorLeft) * kD));
	}
	else {
		setVelocity(base_speed);
	}

	lastErrorLeft = errorLeft;
	lastErrorRight = errorRight;
}
