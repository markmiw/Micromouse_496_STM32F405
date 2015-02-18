#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_gpio.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_rcc.h"

#include "led.h"

void main(void) {
	initLed();

	int timeConstant = HAL_RCC_GetSysClockFreq() / 1000;

	int mSec = 1000 * timeConstant;

	//Turn on LED
	setLed(0, 1);
	while (mSec--);
	setLed(1, 1);
	while (mSec--);
	setLed(2, 1);
	while (mSec--);
	setLed(3, 1);

    while (1) {}

    return;
}
