#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_gpio.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_rcc.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_usart.h"

#include "usart.h"
#include "adc.h"

//Data structure for USART configuration
USART_HandleTypeDef USART_HandleStructure;

void initUSART() {
	//Data structure for GPIO configuration
	GPIO_InitTypeDef GPIO_InitStructure;

	//Enable USART clock (1)
	__USART1_CLK_ENABLE();

	//Enable GPIO clock for USART module (A)
	__GPIOA_CLK_ENABLE();

	//Configure data structure for GPIO alt
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
	GPIO_InitStructure.Alternate = GPIO_AF7_USART1;

	//TX
	GPIO_InitStructure.Pin = GPIO_PIN_9;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

	//RX
	GPIO_InitStructure.Pin = GPIO_PIN_10;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_HandleStructure.Instance = USART1;

	USART_HandleStructure.Init.BaudRate = 9600;
	USART_HandleStructure.Init.WordLength = USART_WORDLENGTH_8B;
	USART_HandleStructure.Init.StopBits = USART_STOPBITS_1;
	USART_HandleStructure.Init.Parity = USART_PARITY_NONE;
	USART_HandleStructure.Init.Mode = USART_MODE_TX;
	USART_HandleStructure.Init.CLKPolarity = USART_POLARITY_HIGH;
	USART_HandleStructure.Init.CLKPhase = USART_PHASE_1EDGE;
	USART_HandleStructure.Init.CLKLastBit = USART_LASTBIT_ENABLE;

	HAL_USART_Init(&USART_HandleStructure);
}

void sensorUSART(int sensor) {

	int test;
	int i;
	int digLen = 1;
	char c = 0x0d;
	char dig = '0';
	char digit[SENSOR];

	/* Load 'x' values into all array locations */
	for(i = 0; i < SENSOR; i++) {
		digit[i] = 'x';
	}

	test = readADC(sensor);

	for(i = 10; i < 1000000000; i*=10) {
		if(test > i) digLen++;
	}

	/* break down integer value to single digits to place into array */
	for(i = 0; i < digLen; i++) {
		digit[SENSOR-1-i] = dig + test%10;
		test/=10;
	}
    switch (sensor) {
        case 0:
            HAL_USART_Transmit(&USART_HandleStructure, (uint8_t *)&"Left Sensor:", 12, 1000);
            HAL_Delay(500);
            break;
        case 1:
            HAL_USART_Transmit(&USART_HandleStructure, (uint8_t *)&"Left Middle Sensor:", 19, 1000);
            HAL_Delay(500);
            break;
        case 2:
            HAL_USART_Transmit(&USART_HandleStructure, (uint8_t *)&"Right Middle Sensor:", 20, 1000);
            HAL_Delay(500);
            break;
        case 3:
            HAL_USART_Transmit(&USART_HandleStructure, (uint8_t *)&"Right Sensor:", 13, 1000);
            HAL_Delay(500);
            break;
        case 4:
            HAL_USART_Transmit(&USART_HandleStructure, (uint8_t *)&"Voltage:", 8, 1000);
            HAL_Delay(500);
            break;
        case 5:
            HAL_USART_Transmit(&USART_HandleStructure, (uint8_t *)&"Gyroscope:", 10, 1000);
            HAL_Delay(500);
            break;
            
        default:
            HAL_USART_Transmit(&USART_HandleStructure, (uint8_t *)&"Wrong Input", 11, 1000);
            HAL_USART_Transmit(&USART_HandleStructure, (uint8_t *)&c, 1, 1000);
            HAL_Delay(500);
            return;
            break;
    }

	HAL_USART_Transmit(&USART_HandleStructure, (uint8_t *)&digit, SENSOR, 1000);
	HAL_Delay(500);
	HAL_USART_Transmit(&USART_HandleStructure, (uint8_t *)&c, 1, 1000);
	HAL_Delay(500);
}
