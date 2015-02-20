#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_gpio.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_rcc.h"

#include "led.h"
#include "motor.h"
#include "stm32f4xx_it.h"

static RCC_ClkInitTypeDef RCC_ClkInitStruct;
static RCC_OscInitTypeDef RCC_OscInitStruct;

void main(void) {
	HAL_Init();
	initLed();
	initMotor();

	int x;

	//Turn on LED
	setLed(0, 1);
	HAL_Delay(1000);
	setLed(1, 1);
	HAL_Delay(1000);
	setLed(2, 1);
	HAL_Delay(1000);
	setLed(3, 1);

	setDirection(LEFTMOTOR, FORWARD);
	setDirection(RIGHTMOTOR, FORWARD);

	for(x = 0; x < 150; x++) {
		setSpeed(LEFTMOTOR, x);
		setSpeed(RIGHTMOTOR, x);
		HAL_Delay(10);
	}

	for(x = 150; x >=0; x--) {
		setSpeed(LEFTMOTOR, x);
		setSpeed(RIGHTMOTOR, x);
		HAL_Delay(10);
	}

	setSpeed(LEFTMOTOR, 200);
	setSpeed(RIGHTMOTOR, 200);
	HAL_Delay(1000);
	setDirection(LEFTMOTOR, BACKWARD);
	setDirection(RIGHTMOTOR, BACKWARD);
	HAL_Delay(1000);
	setSpeed(LEFTMOTOR, 0);
	setSpeed(RIGHTMOTOR, 0);

    while (1) {}

    return;
}
