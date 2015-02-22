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

void testChaser(int mode);
void testMenu();
void testRamp(int maxSpeed, int period);

void main(void) {
	HAL_Init();
	initLED();
	initMotor();
	initEncoder();

	setDirection(LEFTMOTOR, FORWARD);
	setDirection(RIGHTMOTOR, FORWARD);

//	int leftCount = 0;

	while (1) {
//		leftCount = readEncoder(LEFTENCODER);
//
//		if (leftCount > 3413) {
//			setLED(BLUE, ON);
//		} else {
//			setLED(BLUE, OFF);
//		}

//		testMenu(LEFTENCODER);
	}

    return;
}

testChaser(int mode, int period) {
	switch (mode) {
	case 0:
		setLED(WHITE, ON);
		HAL_Delay(period);
		setLED(BLUE, ON);
		HAL_Delay(period);
		setLED(GREEN, ON);
		HAL_Delay(period);
		setLED(RED, ON);
		HAL_Delay(period);
		break;
	case 1:
		setLED(WHITE, ON);
		HAL_Delay(period);
		setLED(BLUE, ON);
		HAL_Delay(period);
		setLED(GREEN, ON);
		HAL_Delay(period);
		setLED(RED, ON);
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
		setLED(WHITE, ON);
		HAL_Delay(period);
		setLED(WHITE, OFF);
		setLED(BLUE, ON);
		HAL_Delay(period);
		setLED(BLUE, OFF);
		setLED(GREEN, ON);
		HAL_Delay(period);
		setLED(GREEN, OFF);
		setLED(RED, ON);
		HAL_Delay(period);
		break;
	}
}

void testMenu(int channel) {
	int count = readEncoder(channel);
	if (count < 0) {
		count = 0;
	} else if (count >= 0 && count <= 800) {
		setLED(RED, ON);
		setLED(GREEN, OFF);
		setSpeed(1 - channel, 0);
	} else if (count > 800 && count <= (800*2)) {
		setLED(RED, OFF);
		setLED(GREEN, ON);
		setLED(BLUE, OFF);
		setSpeed(1 - channel, 30);
	} else if (count > (800*2) && count <= (800*3)) {
		setLED(GREEN, OFF);
		setLED(BLUE, ON);
		setLED(WHITE, OFF);
		setSpeed(1 - channel, 60);
	} else if (count > (800*3) && count <= (800*4)) {
		setLED(BLUE, OFF);
		setLED(WHITE, ON);
		setSpeed(1 - channel, 90);
	} else if (count > (800*4)) {
		count = (800*4);
	}
}

void testRamp(int maxSpeed, int period) {
	int x;
	for(x = 0; x < maxSpeed; x++) {
		setSpeed(LEFTMOTOR, x);
		setSpeed(RIGHTMOTOR, x);
		HAL_Delay(period);
	}

	for(x = maxSpeed; x >=0; x--) {
		setSpeed(LEFTMOTOR, x);
		setSpeed(RIGHTMOTOR, x);
		HAL_Delay(period);
	}
}
