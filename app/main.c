#include "components/coocox-master/STM32F405xx_cmsisboot/source/stm32f4xx.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/stm32f405xx.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_conf.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_gpio.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_rcc.h"

#include "encoder.h"
#include "led.h"
#include "motor.h"
#include "stm32f4xx_it.h"

static int leftCount;
static int rightCount;

void main(void) {
	HAL_Init();
	initLED();
	initMotor();
	initEncoder();

	leftCount = 0;
	rightCount = 0;

//	//Turn on LED
//	setLED(WHITE, ON);
//	HAL_Delay(1000);
//	setLED(BLUE, ON);
//	HAL_Delay(1000);
//	setLED(GREEN, ON);
//	HAL_Delay(1000);
//	setLED(RED, ON);
//
	setDirection(LEFTMOTOR, FORWARD);
	setDirection(RIGHTMOTOR, FORWARD);
//
//	for(x = 0; x < 150; x++) {
//		setSpeed(LEFTMOTOR, x);
//		setSpeed(RIGHTMOTOR, x);
//		HAL_Delay(10);
//	}
//
//	for(x = 150; x >=0; x--) {
//		setSpeed(LEFTMOTOR, x);
//		setSpeed(RIGHTMOTOR, x);
//		HAL_Delay(10);
//	}
//
//	setSpeed(LEFTMOTOR, 200);
//	setSpeed(RIGHTMOTOR, 200);
//	HAL_Delay(1000);
//	setDirection(LEFTMOTOR, BACKWARD);
//	setDirection(RIGHTMOTOR, BACKWARD);
//	HAL_Delay(1000);
//	setSpeed(LEFTMOTOR, 0);
//	setSpeed(RIGHTMOTOR, 0);

    while (1) {
//    	if (leftCount > 3413) {
//    		setLED(BLUE, ON);
//    	}
//
//    	if (rightCount > 3413) {
//			setLED(GREEN, ON);
//		}

//    	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_SET) {
//    		setLED(WHITE, ON);
//    	} else {
//    		setLED(WHITE, OFF);
//    	}

//    	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15) == GPIO_PIN_SET) {
//    		setLED(RED, ON);
//    	} else {
//    		setLED(RED, OFF);
//    	}
    	if (rightCount < 0) {
    		rightCount = 0;
    	} else if (rightCount > 0 && rightCount <= 800) {
    		setLED(RED, ON);
    		setLED(GREEN, OFF);
    	} else if (rightCount > 800 && rightCount <= (800*2)) {
    		setLED(RED, OFF);
    		setLED(GREEN, ON);
    		setLED(BLUE, OFF);
    	} else if (rightCount > (800*2) && rightCount <= (800*3)) {
    		setLED(GREEN, OFF);
			setLED(BLUE, ON);
			setLED(WHITE, OFF);
    	} else if (rightCount > (800*3) && rightCount <= (800*4)) {
    		setLED(BLUE, OFF);
			setLED(WHITE, ON);
    	} else if (rightCount > (800*4)) {
    		rightCount = (800*4);
    	}
    }

    return;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == GPIO_PIN_0) {
		if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_SET) {
			rightCount++;
		} else {
			rightCount--;
		}
	} else if (GPIO_Pin == GPIO_PIN_3) {
//		if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15) == GPIO_PIN_SET) {
//			leftCount++;
//		} else {
//			leftCount--;
//		} STILL GIVING ME PROBLEMS!!!
		leftCount++;
	} return;
}
