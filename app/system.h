/* File Name: system.h
 * Project: EE 396
 * Team: TSM
 * Members: Mark Miw
 *          Steven Chen
 *          Joseph Felix Jr.
 */

#ifndef system_H
#define system_H

#include "global_libs.h"
#include "HAL_includes.h"
#include "defines.h"

#include "encoder.h"
#include "interface.h"
#include "correction.h"

// Data structure for TIM configuration
TIM_HandleTypeDef buzzerHandler;
TIM_HandleTypeDef countHandler;
TIM_HandleTypeDef rightHandler;
TIM_HandleTypeDef motorHandler;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef brakeHandler;

//Data structure for USART configuration
USART_HandleTypeDef USART_HandleStructure;

// Data structure for ADC configuration
ADC_HandleTypeDef ADCHandle;

void initSystem(void);
void initGPIO(void);
void initTIM(void);
void initADC(void);
void initUSART(void);
void SystemClock_Config(void);

#endif
