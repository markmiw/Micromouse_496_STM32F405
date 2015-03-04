#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_adc.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_cortex.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_def.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_gpio.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_rcc.h"

#include "adc.h"

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

    HAL_Delay(10);
    
    HAL_ADC_Start(&ADCHandle);
    
    //Wait for conversion
    while(HAL_ADC_PollForConversion(&ADCHandle, HAL_MAX_DELAY) != HAL_OK);
    
    return HAL_ADC_GetValue(&ADCHandle);
}

uint32_t readADC(int channel) {
	ADC_ChannelConfTypeDef sConfig;

    int i = 1280;

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
    i = 1280;

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

//void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
//
//	int ADCValue = HAL_ADC_GetValue(&ADCHandle);
//	HAL_ADC_Stop_IT(&ADCHandle);
//	return ADCValue;
//
//}
