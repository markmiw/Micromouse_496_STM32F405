#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_adc.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_cortex.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_def.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_gpio.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_rcc.h"

#include "adc.h"
#include "led.h"

ADC_HandleTypeDef ADCHandle;

uint16_t uhADCxConvertedValue = 0;
int convertedValue[6];
int currentTarget;

void initADC() {
	currentTarget = 0;
	convertedValue[0] = 0;
	convertedValue[1] = 0;
	convertedValue[2] = 0;
	convertedValue[3] = 0;
	convertedValue[4] = 0;
	convertedValue[5] = 0;

	//Data structure for GPIO configuration
	GPIO_InitTypeDef GPIO_InitStructure;

	ADC_ChannelConfTypeDef sConfig;

	//Enable GPIO clock for LED module (C & H)
	__GPIOA_CLK_ENABLE();
	__GPIOB_CLK_ENABLE();
	__GPIOC_CLK_ENABLE();
	__GPIOH_CLK_ENABLE();

	//Enable ADC clock
	__ADC1_CLK_ENABLE();

	//Configure data structure for GPIO output
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_MEDIUM;

	//Left emitter
	GPIO_InitStructure.Pin = GPIO_PIN_1;
	HAL_GPIO_Init(GPIOH, &GPIO_InitStructure);

	//Left center emitter
	GPIO_InitStructure.Pin = GPIO_PIN_15;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

	//Right center emitter
	GPIO_InitStructure.Pin = GPIO_PIN_0;
	HAL_GPIO_Init(GPIOH, &GPIO_InitStructure);

	//Right emitter
	GPIO_InitStructure.Pin = GPIO_PIN_14;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;

	//Left detector
	GPIO_InitStructure.Pin = GPIO_PIN_0;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

	//Left center detector
	GPIO_InitStructure.Pin = GPIO_PIN_7;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

	//Right center detector
	GPIO_InitStructure.Pin = GPIO_PIN_4;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

	//Right detector
	GPIO_InitStructure.Pin = GPIO_PIN_5;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

	//Voltage detector
	GPIO_InitStructure.Pin = GPIO_PIN_1;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

	//Gyroscope

	ADCHandle.Instance = ADC1;

	ADCHandle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
	ADCHandle.Init.Resolution = ADC_RESOLUTION12b;
	ADCHandle.Init.ScanConvMode = ENABLE;
	ADCHandle.Init.ContinuousConvMode = DISABLE;
	ADCHandle.Init.NbrOfConversion = 1;
	ADCHandle.Init.DiscontinuousConvMode = DISABLE;
	ADCHandle.Init.NbrOfDiscConversion = 0;
	ADCHandle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	ADCHandle.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1;
	ADCHandle.Init.DMAContinuousRequests = DISABLE;
	ADCHandle.Init.EOCSelection = DISABLE;

	HAL_ADC_Init(&ADCHandle);

	//Left detector
	sConfig.Channel = ADC_CHANNEL_8;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
	sConfig.Offset = 0;
	HAL_ADC_ConfigChannel(&ADCHandle, &sConfig);

//	HAL_NVIC_EnableIRQ(ADC_IRQn);

	return;
}

uint32_t readBattery() {
    ADC_ChannelConfTypeDef sConfig;
    
    //Voltage detector
    sConfig.Channel = ADC_CHANNEL_9;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
    sConfig.Offset = 0;
    HAL_ADC_ConfigChannel(&ADCHandle, &sConfig);

    HAL_ADC_Start(&ADCHandle);
    //Wait for conversion
    while(HAL_ADC_PollForConversion(&ADCHandle, HAL_MAX_DELAY) != HAL_OK);

    return HAL_ADC_GetValue(&ADCHandle);
}

uint32_t readADC(int channel) {
	ADC_ChannelConfTypeDef sConfig;
	int x = 14444;
    int i = x;

	switch (channel) {
		case LEFT_DET:
			//Left detector
			sConfig.Channel = ADC_CHANNEL_8;
			sConfig.Rank = 1;
			sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
			sConfig.Offset = 0;
			HAL_ADC_ConfigChannel(&ADCHandle, &sConfig);
			HAL_GPIO_WritePin(GPIOH, GPIO_PIN_1, GPIO_PIN_SET);
			break;
		case LEFT_CEN_DET:
			//Left center detector
			sConfig.Channel = ADC_CHANNEL_7;
			sConfig.Rank = 1;
			sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
			sConfig.Offset = 0;
			HAL_ADC_ConfigChannel(&ADCHandle, &sConfig);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET);
			break;
		case RIGHT_CEN_DET:
			//Right center detector
			sConfig.Channel = ADC_CHANNEL_4;
			sConfig.Rank = 1;
			sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
			sConfig.Offset = 0;
			HAL_ADC_ConfigChannel(&ADCHandle, &sConfig);
			HAL_GPIO_WritePin(GPIOH, GPIO_PIN_0, GPIO_PIN_SET);
			break;
		case RIGHT_DET:
			//Right detector
			sConfig.Channel = ADC_CHANNEL_15;
			sConfig.Rank = 1;
			sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
			sConfig.Offset = 0;
			HAL_ADC_ConfigChannel(&ADCHandle, &sConfig);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET);
			break;
	}
    // Wait 8us
    while(i--);
    i = x;

	HAL_ADC_Start(&ADCHandle);

	//Wait for conversion
	while(HAL_ADC_PollForConversion(&ADCHandle, HAL_MAX_DELAY) != HAL_OK);
	HAL_GPIO_WritePin(GPIOH, GPIO_PIN_1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOH, GPIO_PIN_0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET);
    
    while(i--);
    
	return HAL_ADC_GetValue(&ADCHandle);
}

void sensorCalibrationPrint(int sensor, int distance) {
	int i = 0;
	if(sensor == RIGHT_DET) printStringUSART("Right Sensor");
	if(sensor == LEFT_DET) printStringUSART("Left Sensor");
	if(sensor == RIGHT_CEN_DET) printStringUSART("Right Center Sensor");
	if(sensor == LEFT_CEN_DET) printStringUSART("Left Center Sensor");
	printNL();
	int readings[distance+1];
	while(i < distance+1) {
		printStringUSART("Move to ");
		printUSART(i);
		printStringUSART("cm");
		printNL();
		HAL_Delay(10000);

		int value = 0;
		int j;
		for(j = 0; j < 10; j++)
			value+=readADC(sensor);
		value/=10;
		readings[i] = value;
		i++;

		if(i == distance+1) {
			printStringUSART("Printing CSV");
			printNL();
			HAL_Delay(5000);
			for(j = 0; j < distance+1; j++) {
				printUSART(j);
				printStringUSART(",");
				printUSART(readings[j]);
				printNL();
			}
		}
	}
	printStringUSART("Finished Generating");
	printNL();
	HAL_Delay(5000);
}

double rightSensorConversion(int x) {
	int y = (-4.936*0.000000001*x*x*x + 4.23*0.00001*x*x - 0.122*x + 121.158);

    if(x > RIGHT_MAX) {return 0.00;}
    else if(x < RIGHT_MIN) {return CALI_DIST;}
    else {return y;}
}

double leftSensorConversion(int x) {
	int y;

    if(x > LEFT_MAX) {return 0.00;}
    else if(x < LEFT_MIN) {return CALI_DIST;}
    else if(x <= LEFT_MAX && x > LEFT_5) y = -4.234*0.000000000000001*x*x*x*x*x + 5.694*0.00000000001*x*x*x*x - 3.042*0.0000001*x*x*x + 8.077*0.0001*x*x - 1.069*x + 570.707;
    else if(x <= LEFT_5 && x > LEFT_10) y = -8.796*0.000000001*x*x*x + 5.115*0.00001*x*x - 0.104*x + 77.862;
    else y = 4.603*0.00001*x*x - 0.132*x + 104.653;

    return y;
}

double rightCenterSensorConversion(int x) {
	int y = (-4.782*0.000000001*x*x*x + 4.196*0.00001*x*x - 0.125*x + 134.84);

    if(x > RIGHT_MAX) {return 0.00;}
    else if(x < RIGHT_CEN_MIN) {return 50.00;}
    else {return y;}
}

double leftCenterSensorConversion(int x) {
	int y = (-4.806*0.000000001*x*x*x + 4.285*0.00001*x*x - 0.129*x + 139.434);

    if(x > LEFT_MAX) {return 0.00;}
    else if(x < LEFT_CEN_MIN) {return 15.00;}
    else {return y;}
}

//void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
//
//	int ADCValue = HAL_ADC_GetValue(&ADCHandle);
//	HAL_ADC_Stop_IT(&ADCHandle);
//	return ADCValue;
//
//}
