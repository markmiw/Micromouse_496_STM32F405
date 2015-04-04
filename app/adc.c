/* File Name: adc.c
 * Project: EE 396
 * Team: TSM
 * Members: Mark Miw
 *          Steven Chen
 *          Joseph Felix Jr.
 */

#include "adc.h"

extern ADC_HandleTypeDef ADCHandle;

uint16_t uhADCxConvertedValue = 0;
int convertedValue[6];
int currentTarget;

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
        case FLASH_MEM:
            sConfig.Channel = ADC_CHANNEL_14;
            sConfig.Rank = 1;
            sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
            sConfig.Offset = 0;
            HAL_ADC_ConfigChannel(&ADCHandle, &sConfig);
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

double readSensor(ADC_Channel channel) {
    return toLinear(ADC_getSampleAvgNDeleteX(5,2,channel));
}

uint16_t ADC_getSampleAvgNDeleteX(uint8_t N, uint8_t X, int channel) {
    uint32_t avg_sample = 0x00;
    uint16_t adc_sample[N];
    uint8_t index = 0x00;
    
    for (index = 0x00; index<N; index++) {
        adc_sample[index] = 0x00;
    }
    
    for (index = 0x00; index<N; index++) {
        adc_sample[index] = readADC(channel);
    }
    
    Sort_tab(adc_sample, N);
    
    for (index=X/2; index<N-X/2; index++) {
        avg_sample += adc_sample[index];
    }
    
    avg_sample /= N-X;
    
    return avg_sample;
}

void Sort_tab(uint16_t tab[], uint8_t length) {
    uint8_t l = 0x00;
    uint8_t exchange = 0x01;
    uint16_t tmp=0x00;
    
    while (exchange==1) {
        exchange = 0;
        for (l = 0; l < length-1; l++) {
            if (tab[l] > tab[l+1]) {
                tmp = tab[1];
                tab[l] = tab[l+1];
                tab[l+1] = tmp;
                exchange = 1;
            }
        }
    }
}
