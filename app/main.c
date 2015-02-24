#include "components/coocox-master/STM32F405xx_cmsisboot/source/stm32f4xx.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/stm32f405xx.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_conf.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_gpio.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_rcc.h"

#include "adc.h"
#include "encoder.h"
#include "led.h"
#include "motor.h"
#include "stm32f4xx_it.h"

void testChaser(int mode, int period);
void testMenu();
void testRamp(int maxSpeed, int period);

void main(void) {
	uint32_t batteryLevel;

	HAL_Init();
	initLED();
	initMotor();
	initEncoder();
	initADC();

	setDirection(LEFTMOTOR, FORWARD);
	setDirection(RIGHTMOTOR, FORWARD);

	testChaser(1, 500);

	while (1) {
		batteryLevel = readADC(VOLT_DET);

		if (batteryLevel < 2300) {
			setSpeed(LEFTMOTOR, 0);
			setSpeed(RIGHTMOTOR, 0);
			setBuzzer(200);
			setLEDAll(OFF);

			while (1) {
				toggleLED(RED);
				HAL_Delay(500);
			}
		}
//
//		if (test > 3300) {
//			setLED(WHITE, ON);
//		} else {
//			setLED(WHITE, OFF);
//		}
//
//		if (test > 3200) {
//			setLED(BLUE, ON);
//		} else {
//			setLED(BLUE, OFF);
//		}
//
//		if (test > 3100) {
//			setLED(GREEN, ON);
//		} else {
//			setLED(GREEN, OFF);
//		}
//
//		if (test > 300) {
//			setLED(RED, ON);
//		} else {
//			setLED(RED, OFF);
//		}

	}

    return;
}

void testChaser(int mode, int period) {
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
