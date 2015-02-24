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
#include "usart.h"

//Private prototypes
void testChaser(int mode, int period);
void testMenu();
void testRamp(int maxSpeed, int period);
void batteryFault();



void main(void) {
	HAL_Init();
	initLED();
	initMotor();
	initEncoder();
	initADC();
	initUSART();

	extern USART_HandleTypeDef USART_HandleStructure;

	int x;
	uint8_t test = 0xFF;

	//LED start up sequence
	testChaser(1, 250);

	HAL_USART_Transmit(&USART_HandleStructure, (uint8_t *)&"Hello", 5, 1000);

	//Main program loop
	while (1) {
		//Check for a low battery fault
		batteryFault();

		//User code

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

void batteryFault() {
	//Take a reading from the voltage detector
	uint32_t batteryLevel = readADC(VOLT_DET);
	//Check to see if voltage level is above 7V
	//Voltage detector is a voltage divider where 7V is measured as 2.3333V
	//2.3333V translate to roughly 2333 from the 12 bit ADC
	if (batteryLevel < 2333) {
		//Disable all motors
		setSpeed(LEFTMOTOR, 0);
		setSpeed(RIGHTMOTOR, 0);
		//Enable buzzer
		setBuzzer(4000);
		//Disable all LEDs
		setLEDAll(OFF);
		//Flash red LED every half second.
		while (1) {
			//Invert the state of the red LED located closest to the STM
			toggleLED(RED);
			//Half second delay
			HAL_Delay(500);
		}
	}
}
