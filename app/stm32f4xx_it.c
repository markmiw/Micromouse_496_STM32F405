#include "stm32f4xx_it.h"

void SysTick_Handler(void) {
  HAL_IncTick();
}
