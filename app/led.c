#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_gpio.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_rcc.h"

#include "led.h"

void initLED() {
	//Data structure for GPIO configuration
	GPIO_InitTypeDef GPIO_InitStructure;

	//Enable GPIO clock for LED module (A & B)
	__GPIOA_CLK_ENABLE();
	__GPIOB_CLK_ENABLE();

	//Configure data structure for GPIO output
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_MEDIUM;

	//0 - Front LED
	GPIO_InitStructure.Pin = GPIO_PIN_11;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

	//1 -
	GPIO_InitStructure.Pin = GPIO_PIN_10;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

	//2 -
	GPIO_InitStructure.Pin = GPIO_PIN_5;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

	//3 - Back LED
	GPIO_InitStructure.Pin = GPIO_PIN_6;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void setLED(int channel, int state) {
	switch (channel) {
	case WHITE:
		if (state) {
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);
		} else {
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);
		} break;
	case BLUE:
		if (state) {
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
		} else {
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
		} break;
	case GREEN:
		if (state) {
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
		} else {
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		} break;
	case RED:
		if (state) {
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
		} else {
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
		}
	}
}

//Control the state of all 4 LED at once
void setLEDAll(int state) {
	if (state) { //On state
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
	} else { //Off state
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
	}
}

void toggleLED(int channel) {
	switch (channel) {
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
void toggleLEDAll() {
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_11);
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_10);
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);
}
