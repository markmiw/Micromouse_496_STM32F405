/* File Name: interface.c
 * Project: EE 396
 * Team: TSM
 * Members: Mark Miw
 *          Steven Chen
 *          Joseph Felix Jr.
 */

#include "interface.h"

extern TIM_HandleTypeDef buzzerHandler;

void setBuzzer(int state) {
	if (state) HAL_TIM_Base_Start_IT(&buzzerHandler);
	else HAL_TIM_Base_Stop_IT(&buzzerHandler);
}

void waitForTop() {
	uint8_t startFlag = 0;
	do {
		if (readADC(FLASH_MEM) < 50) {
			startFlag = 1;
		}
		toggleLED(WHITE);
		HAL_Delay(500);
	} while (!startFlag);
	setLED(WHITE);
}

void setLED(Led color) {
	switch (color) {
	case WHITE:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);
		break;
	case BLUE:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
		break;
	case GREEN:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
		break;
	case RED:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
		break;
	}
}

void resetLED(Led color) {
	switch (color) {
	case WHITE:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);
		break;
	case BLUE:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
		break;
	case GREEN:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		break;
	case RED:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
		break;
	}
}

//Control the state of all 4 LED at once
void setLEDAll(void) {
	setLED(WHITE);
	setLED(BLUE);
	setLED(GREEN);
	setLED(RED);
}

void resetLEDAll(void) {
	resetLED(WHITE);
	resetLED(BLUE);
	resetLED(GREEN);
	resetLED(RED);
}

void toggleLED(Led color) {
	switch (color) {
	case WHITE:
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_11);
		break;
	case BLUE:
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_10);
		break;
	case GREEN:
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		break;
	case RED:
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);
		break;
	}
}

//Toggle the state of all 4 LED at once
void toggleLEDAll(void) {
	toggleLED(WHITE);
	toggleLED(BLUE);
	toggleLED(GREEN);
	toggleLED(RED);
}

void testChaser(int mode, int period) {
 	switch (mode) {
 	case 0:
 		setLED(WHITE);
 		HAL_Delay(period);
 		setLED(BLUE);
 		HAL_Delay(period);
 		setLED(GREEN);
 		HAL_Delay(period);
 		setLED(RED);
 		HAL_Delay(period);
 		break;
 	case 1:
 		setLED(WHITE);
 		HAL_Delay(period);
 		setLED(BLUE);
 		HAL_Delay(period);
 		setLED(GREEN);
 		HAL_Delay(period);
 		setLED(RED);
 		HAL_Delay(period);
 		toggleLEDAll();
 		HAL_Delay(period);
 		toggleLEDAll();
 		HAL_Delay(period);
 		toggleLEDAll();
 		HAL_Delay(period);
 		toggleLEDAll();
 		HAL_Delay(period);
 		toggleLEDAll();
 		break;
 	case 2:
 		setLED(WHITE);
 		HAL_Delay(period);
 		resetLED(WHITE);
 		setLED(BLUE);
 		HAL_Delay(period);
 		resetLED(BLUE);
 		setLED(GREEN);
 		HAL_Delay(period);
 		resetLED(GREEN);
 		setLED(RED);
 		HAL_Delay(period);
 		break;
 	}
 }

void testMenu(int channel) {
	int count = getEncoder(channel);
	if (count < 0) {
		count = 0;
	} else if (count >= 0 && count <= 800) {
		setLED(RED);
		resetLED(GREEN);
	} else if (count > 800 && count <= (800*2)) {
		resetLED(RED);
		setLED(GREEN);
		resetLED(BLUE);
	} else if (count > (800*2) && count <= (800*3)) {
		resetLED(GREEN);
		setLED(BLUE);
		resetLED(WHITE);
	} else if (count > (800*3) && count <= (800*4)) {
		resetLED(BLUE);
		setLED(WHITE);
	} else if (count > (800*4)) {
		count = (800*4);
	}
}

void batteryFault() {
 	//Take a reading from the voltage detector
 	uint32_t batteryLevel = readBattery();
 	//Check to see if voltage level is above 7V
 	//Voltage detector is a voltage divider where 7V is measured as 2.3333V
 	//2.3333V translate to roughly 2333 from the 12 bit ADC
 	if (batteryLevel < 2333) {
 		//Disable all motors
 		brake();
 		//Enable buzzer
 		setBuzzer(ON);
 		//Disable all LEDs
 		resetLEDAll();
 		//Disable Motors
 		HAL_TIM_Base_Stop_IT(&htim2);
 		setSpeed(RIGHTMOTOR,0);
 		setSpeed(LEFTMOTOR,0);
 		//Flash red LED every half second.
 		while (1) {
 			//Invert the state of the red LED located closest to the STM
 			toggleLED(RED);
 			//Half second delay
 			HAL_Delay(500);
 		}
 	}
 }

bool getButton(void) {
	if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_SET) return true;
	else return false;
}
