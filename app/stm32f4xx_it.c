/* File Name: stm32fxx_it.c
 * Project: EE 396
 * Team: TSM
 * Members: Mark Miw
 *          Steven Chen
 *          Joseph Felix Jr.
 */

#include "stm32f4xx_it.h"


extern TIM_HandleTypeDef buzzerHandler;
extern TIM_HandleTypeDef brakeHandler;
extern TIM_HandleTypeDef velocityHandler;
extern TIM_HandleTypeDef htim2;

void SysTick_Handler(void) {
  HAL_IncTick();
}

//void EXTI3_IRQHandler(void) {
//	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
//}

void EXTI15_10_IRQHandler(void) {
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
}

void EXTI0_IRQHandler(void) {
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

void TIM2_IRQHandler(void) {
	HAL_TIM_IRQHandler(&htim2);
}

void TIM3_IRQHandler(void) {
	HAL_TIM_IRQHandler(&buzzerHandler);
}

void TIM5_IRQHandler(void) {
	HAL_TIM_IRQHandler(&brakeHandler);
}
