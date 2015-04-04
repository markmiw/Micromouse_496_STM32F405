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
	printString("left sensor");
	printComma();
	printString("left center sensor");
	printComma();
	printString("right center sensor");
	printComma();
	printString("right sensor");
	printComma();
	printNL();

	while(1) {
		batteryFault();
		while(!getButton());

		printFloat(readSensor(LEFT_DET));
		printComma();
		printFloat(readSensor(LEFT_CEN_DET));
		printComma();
		printFloat(readSensor(RIGHT_CEN_DET));
		printComma();
		printFloat(readSensor(RIGHT_DET));
		printNL();
	}
}
