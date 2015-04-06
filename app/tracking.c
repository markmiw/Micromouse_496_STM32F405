/* File Name: tracking.c
 * Project: EE 396
 * Team: TSM
 * Members: Mark Miw
 *          Steven Chen
 *          Joseph Felix Jr.
 */

#include "tracking.h"


bool hasRightWall(double value) {
	if (value < getWall(FARRIGHTWALL)+20) return true;
	else return false;
}

bool hasLeftWall(double value) {
	if (value < getWall(FARLEFTWALL)+20) return true;
	else return false;
}

bool hasFrontWall(double valueRight, double valueLeft) {
	if ((valueRight < getWall(IDEALRIGHTFRONT)+20) && (valueLeft < getWall(IDEALLEFTCENTER)+20)) return true;
	else return false;
}

void moveForward(int cells, Wall_Correction wall, double base_speed) {
	int x;
	for (x = 0; x < cells; x++) {

		int startL = getEncoder(LEFTENCODER);
		double left_front_sensor;
		double right_front_sensor;
		resetLEDAll();

		while(1) {
			left_front_sensor = readSensor(LEFT_DET);
			right_front_sensor = readSensor(RIGHT_DET);

			if (right_front_sensor <= getWall(IDEALRIGHTFRONT) && left_front_sensor <= getWall(IDEALLEFTFRONT)) {
				hardBrake();
				frontCorrection();
				break;
			}

			correction(wall, base_speed);

			if (getEncoder(LEFTENCODER) > startL + CELL_L) {
				brake();
				break;
			}
		}
	}
}

void moveCells(int num, double base_speed, int wall) {

	double left_front_sensor;
	double right_front_sensor;
	int location;
	double speed = base_speed;
	int startL = getEncoder(LEFTENCODER);
	int distance = startL + CELL_L*(num);
//	setLED(GREEN);

	while(1) {
		left_front_sensor = readSensor(LEFT_DET);
		right_front_sensor = readSensor(RIGHT_DET);
		location = getEncoder(LEFTENCODER);

		if (right_front_sensor <= getWall(IDEALRIGHTFRONT) && left_front_sensor <= getWall(IDEALLEFTFRONT)) {
//			setLED(RED);
//			resetLED(GREEN);
			hardBrake();
			frontCorrection();
			break;
		}

		if (location > distance) {
//			setLED(BLUE);
//			resetLED(GREEN);
//			speed = base_speed*(distance-location)/CELL_L;
			brake();
			break;
		}

		correction(wall, speed);
	}
}
