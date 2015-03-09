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
#include <string.h>

//Private prototypes
void testChaser(int mode, int period);
void testMenu();
void testRamp(int maxSpeed, int period);
void batteryFault();
void SystemClock_Config(void);

int errorP_right;
int errorD_right;
int oldErrorP_right;
int totalError_right;
int P_right;
int D_right;

int errorP_left;
int errorD_left;
int oldErrorP_left;
int totalError_left;
int P_left;
int D_left;

void PIDencoderRight();
void PIDencoderLeft();

void main(void) {
	HAL_Init();
	SystemClock_Config();
	initLED();
	initUSART();
	initMotor();
	initEncoder();
	initADC();
	setTimer_ms(ON);

	errorP_left = 0;
	errorD_left = 0;
	oldErrorP_left = 0;
	totalError_left = 0;
	P_left = 20;
	D_left = 10;

	errorP_right = 0;
	errorD_right = 0;
	oldErrorP_right = 0;
	totalError_right = 0;
	P_right = 20;
	D_right = 10;

	//LED start up sequence
	testChaser(1, 250);

	printStringUSART("Hello world!");
	printNL();

	timer = 0;

	while (1) {
		//Check for a low battery fault
		batteryFault();

		PIDencoderRight();
		PIDencoderLeft();
	}

    return;
}

void PIDencoderRight(void)
{
	int speed = currentSpeed(RIGHTMOTOR);

    errorP_right = readEncoder(RIGHTMOTOR);
    errorD_right = errorP_right - oldErrorP_right;

    totalError_right = P_right * errorP_right + D_right * errorD_right;
    oldErrorP_right = errorP_right;

    if( (speed - totalError_right) < 0) {
    	setDirection(RIGHTMOTOR,BACKWARD);
    	if ( (speed - totalError_right) > -100) speed = 100;
    	else speed = speed - totalError_right;
    }
    else {
    	setDirection(RIGHTMOTOR,FORWARD);
    	if ( (speed - totalError_right) < 100) speed = 100;
    	else speed = speed - totalError_right;
    }

    setSpeed(RIGHTMOTOR, speed);
}

void PIDencoderLeft(void)
{
	int speed = currentSpeed(LEFTMOTOR);

    errorP_left = readEncoder(LEFTMOTOR);
    errorD_left = errorP_left - oldErrorP_left;

    totalError_left = P_left * errorP_left + D_left * errorD_left;
    oldErrorP_left = errorP_left;

    if( (speed - totalError_left) < 0) {
    	setDirection(LEFTMOTOR,BACKWARD);
    	if ( (speed - totalError_left) > -100) speed = 100;
    	else speed = speed - totalError_left;
    }
    else {
    	setDirection(LEFTMOTOR,FORWARD);
    	if ( (speed - totalError_left) < 100) speed = 100;
    	else speed = speed - totalError_left;
    }

    setSpeed(LEFTMOTOR, speed);
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
	} else if (count > 800 && count <= (800*2)) {
		setLED(RED, OFF);
		setLED(GREEN, ON);
		setLED(BLUE, OFF);
	} else if (count > (800*2) && count <= (800*3)) {
		setLED(GREEN, OFF);
		setLED(BLUE, ON);
		setLED(WHITE, OFF);
	} else if (count > (800*3) && count <= (800*4)) {
		setLED(BLUE, OFF);
		setLED(WHITE, ON);
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
	uint32_t batteryLevel = readBattery();
	//Check to see if voltage level is above 7V
	//Voltage detector is a voltage divider where 7V is measured as 2.3333V
	//2.3333V translate to roughly 2333 from the 12 bit ADC
	if (batteryLevel < 2333) {
		//Disable all motors
		setSpeed(LEFTMOTOR, 0);
		setSpeed(RIGHTMOTOR, 0);
		//Enable buzzer
		setBuzzer(ON);
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

void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  __PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 10;
  RCC_OscInitStruct.PLL.PLLN = 210;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1
                              |RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

}
