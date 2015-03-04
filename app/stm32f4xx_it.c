#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_adc.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_gpio.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_rcc.h"

#include "stm32f4xx_it.h"

#include "encoder.h"

extern TIM_HandleTypeDef buzzerHandler;
extern TIM_HandleTypeDef leftHandler;
extern TIM_HandleTypeDef rightHandler;

void SysTick_Handler(void) {
  HAL_IncTick();
}

//void ADC_IRQHandler(void) {
//	HAL_ADC_IRQHandler(&ADCHandle);
//}

void EXTI3_IRQHandler(void) {
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
}

void EXTI0_IRQHandler(void) {
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

void TIM2_IRQHandler(void) {
	HAL_TIM_IRQHandler(&leftHandler);
}

void TIM3_IRQHandler(void) {
	HAL_TIM_IRQHandler(&buzzerHandler);
}

void TIM5_IRQHandler(void) {
	HAL_TIM_IRQHandler(&rightHandler);
}
