/* File Name: stm32f4xx_it.h
 * Project: EE 396
 * Team: TSM
 * Members: Mark Miw
 *          Steven Chen
 *          Joseph Felix Jr.
 */

#ifndef stm32f4xx_it_H
#define stm32f4xx_it_H

#include "global_libs.h"
#include "HAL_includes.h"
#include "defines.h"

void SysTick_Handler(void);
void EXTI3_IRQHandler(void);
void EXTI0_IRQHandler(void);
void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);
void TIM5_IRQHandler(void);

#endif
