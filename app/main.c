/* File Name: main.c
 * Project: EE 396
 * Team: TSM
 * Members: Mark Miw
 *          Steven Chen
 *          Joseph Felix Jr.
 */

#include "adc.h"
#include "calibration.h"
#include "correction.h"
#include "defines.h"
#include "encoder.h"
#include "global_libs.h"
#include "HAL_includes.h"
#include "interface.h"
#include "motor.h"
#include "stm32f4xx_it.h"
#include "tracking.h"
#include "usart.h"
#include "system.h"

extern TIM_HandleTypeDef htim2;

void main(void) {
	HAL_Init();
	SystemClock_Config();
	initSystem();

	// Test USART Connection
	printString("Hello world!");
	printNL();

	calibrateSensors();

	// Start button.
	while(!getButton()) {
		toggleLED(WHITE);
		HAL_Delay(100);
	}

	resetLED(WHITE);

	// Start-up light sequence.
	testChaser(1,250);

	int temp = 0;

	// Base speed for this run.
	double base_speed = 25;

	// Initialize motors.
	setSpeed(LEFTMOTOR, 0);
	setSpeed(RIGHTMOTOR, 0);
	setVelocity(0);

	// Initialize encoders.
	resetEncoder(LEFTENCODER);
	resetEncoder(RIGHTENCODER);

	// Wall tracking flags.
	bool is_left_wall  = true;
	bool is_right_wall = true;

	int correction_state = 0;

//	const int wall_threshold = 25;

	// Sensor information variables.
	double left_front_sensor, right_front_sensor;
//	double last_left_front_sensor, last_right_fron
	double left_side_sensor, right_side_sensor;
//	double last_left_side_sensor, last_right_side_sensor;
//	double diff_left_side_sensor, diff_right_side_sensor;

	// Initial sensor readings
	left_front_sensor  = readSensor(LEFT_DET);
	right_front_sensor = readSensor(RIGHT_DET);
	left_side_sensor   = readSensor(LEFT_CEN_DET);
	right_side_sensor  = readSensor(RIGHT_CEN_DET);

	while(1) {
		// Check the condition of the battery.
		// Faults if battery is @ a lower voltage level.
		batteryFault();

		// Save last group of sensor readings.
//		last_left_front_sensor  = left_front_sensor;
//		last_right_front_sensor = right_front_sensor;
//		last_left_side_sensor   = left_side_sensor;
//		last_right_side_sensor  = right_side_sensor;

		// Take new sensor readings.
		left_front_sensor  = readSensor(LEFT_DET);
		right_front_sensor = readSensor(RIGHT_DET);
		left_side_sensor   = readSensor(LEFT_CEN_DET);
		right_side_sensor  = readSensor(RIGHT_CEN_DET);

		// Detect for front wall stop.
		if (right_front_sensor <= getWall(IDEALRIGHTFRONT) && left_front_sensor <= getWall(IDEALLEFTFRONT)) {
			hardBrake();
			frontCorrection();
			while(!getButton()) {
				batteryFault();
				toggleLED(RED);
				HAL_Delay(100);
			}
			resetLED(RED);
			HAL_Delay(500);
		}

		// Find the different from the last sensor readings.
//		diff_left_side_sensor  = left_side_sensor - last_left_side_sensor;
//		diff_right_side_sensor = right_side_sensor - last_right_side_sensor;

		// Update the state of the left and right walls
//		if (diff_left_side_sensor > wall_threshold && is_left_wall) {
//			is_left_wall = false;
//		}
//
//		if (diff_left_side_sensor < -wall_threshold && !is_left_wall) {
//			is_left_wall = true;
//		}
//
//		if (diff_right_side_sensor > wall_threshold && is_right_wall) {
//			is_right_wall = false;
//		}
//
//		if (diff_right_side_sensor < -wall_threshold && !is_right_wall) {
//			is_right_wall = true;
//		}

		if (left_side_sensor <= getWall(FARLEFTWALL)) {
			is_left_wall = true;
		}

		else {
			is_left_wall = false;
		}

		if (right_side_sensor <= getWall(FARRIGHTWALL)) {
			is_right_wall = true;
		}

		else {
			is_right_wall = false;
		}

		// Output the state via LED.
		if (is_left_wall) setLED(WHITE);
		else resetLED(WHITE);

		if (is_right_wall) setLED(BLUE);
		else resetLED(BLUE);

		// Convert wall states to correction.
		if (is_left_wall && is_right_wall) {
			correction_state = 0;
		}

		else if (!is_left_wall && is_right_wall) {
			correction_state = 0;
		}

		else if (is_left_wall && !is_right_wall) {
			correction_state = 1;
		}

		else {
			correction_state = 2;
		}

		// Update motors.
//		for (temp = 0; temp < 100; temp++) {
			correction(correction_state, base_speed);
//		}
	}
}
