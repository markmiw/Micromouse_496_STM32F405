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

float rightSensorConversion(int x) {
    if (x >= 2335) {
        return (-3*0.000000001*x*x*x + 3*0.00001*x*x - 0.0801*x + 86.366);
    }
    else if (x >= 1823 && x < 2335) {
        return (0.00001*x*x - 0.0622*x + 81.166);
    }
    else if (x >= 1642 && x < 1823) {
        return (6*0.00001*x*x - 0.2513*x + 253.42);
    }
    else if (x >= 1568 && x < 1642) {
        return (-0.0666*x + 124.07);
    }
    else if (x >= 1533 && x < 1568) {
        return (-0.1388*x + 237.54);
    }
    else {
        return (-0.2857*x + 462.79);
    }
}

float leftSensorConversion(int x) {
    if (x >= 1915) {
        return (-2*0.000000001*x*x*x + 0.00001*x*x -0.0377*x + 40.632);
    }
    else if (x >= 1348 && x < 1915) {
        return (8*0.000001*x*x - 0.0342*x + 41.765);
    }
    else if (x >= 1137 && x < 1348) {
        return (5*0.00001*x*x - 0.1475*x + 118.19);
    }
    else if (x >= 1049 && x < 1137) {
        return (-0.0561*x + 78.584);
    }
    else if ( x >= 1006 && x < 1049) {
        return (-0.113*x + 138.43);
    }
    else {
        return (-0.2249*x + 251.06);
    }
}

float rightCenterSensorConversion(int x) {
    if (x >= 3660) {
        return (-8*0.00001*x*x + 0.5542*x - 1008.6);
    }
    else if (x >= 2533 && x < 3660) {
        return (-4*0.000000001*x*x*x + 4*0.00001*x*x - 0.1163*x + 132.56);
    }
    else if (x >= 2023 && x < 2533) {
        return (0.00001*x*x - 0.0554*x + 86.04);
    }
    else if (x >= 1833 && x < 2023) {
        return (5*0.00001*x*x - 0.2281*x + 261.94);
    }
    else if (x >= 1742 && x < 1833) {
        return (0.0546*x + 119.97);
    }
    else {
        return (-0.1*x + 199.01);
    }
}

float leftCenterSensorConversion(int x) {
    if (x >= 3493) {
        return (-0.0000001*x*x*x + 0.0012*x*x - 4.46*x + 5399.2);
    }
    else if (x >= 2221 && x < 3493) {
        return (-3*0.000000001*x*x*x + 2*0.00001*x*x - 0.0749*x + 87.392);
    }
    else if (x >= 1812 && x < 2221) {
        return (2*0.00001*x*x - 0.0744*x + 99.119);
    }
    else if (x >= 1666 && x < 1812) {
        return (0.0001*x*x - 0.3747*x + 372.42);
    }
    else if (x >= 1601 && x < 1666) {
        return (-0.076*x + 146.38);
    }
    else {
        return (-0.1315*x + 235.41);
    }
}

//void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
//
//	int ADCValue = HAL_ADC_GetValue(&ADCHandle);
//	HAL_ADC_Stop_IT(&ADCHandle);
//	return ADCValue;
//
//}
