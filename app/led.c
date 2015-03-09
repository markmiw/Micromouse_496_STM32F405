#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_gpio.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_rcc.h"

#include "led.h"

void initLED(void) {
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


/**
  * @brief  Function to turn a single LED on
  * @param  color: Color of the LED you want to enable
  * @retval Nothing
  */
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

/**
  * @brief  Function to turn a single LED off
  * @param  color: Color of the LED you want to enable
  * @retval Nothing
  */
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

/**
  * @brief  Function to turn all LEDs on
  * @retval Nothing
  */
void setLEDAll(void) {
	setLED(WHITE);
	setLED(BLUE);
	setLED(GREEN);
	setLED(RED);
}

/**
  * @brief  Function to turn all LEDs off
  * @retval Nothing
  */
void resetLEDALL(void) {
	resetLED(WHITE);
	resetLED(BLUE);
	resetLED(GREEN);
	resetLED(RED);
}

/**
  * @brief  Function to toggle a LED on/off
  * @param: color: Color of the LED you want to enable
  * @retval Nothing
  */
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

/**
  * @brief  Function to toggle all LEDs on/off
  * @retval Nothing
  */
void toggleLEDAll(void) {
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_11);
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_10);
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);
}
