/* File Name: usart.c
 * Project: EE 396
 * Team: TSM
 * Members: Mark Miw
 *          Steven Chen
 *          Joseph Felix Jr.
 */

#include "usart.h"

#define TRANS_DELAY 200

//Data structure for USART configuration
extern USART_HandleTypeDef USART_HandleStructure;

void printInt(int value) {
    int k;
    int digLen = 1;
    char dig = '0';
    char digit[10];
    int negative = 1;

    if (value < 0) {
    	negative = -1;
    	value*=negative;
    }

    for(k = 10; k <= value; k*=10) {
        digLen++;
    }

    for(k = 0; k < digLen; k++) {
        digit[digLen-1-k] = dig + value%10;
        value/=10;
    }

    if (negative == -1) HAL_USART_Transmit(&USART_HandleStructure, (uint8_t *) "-", 1, TIMEOUT);
    HAL_USART_Transmit(&USART_HandleStructure, (uint8_t *)&digit, digLen, TIMEOUT);
    HAL_Delay(TRANS_DELAY);
}

void printFloat(float value) {
	int x = value;
	int y;
    int negative = 1;

    if (value < 0) {
    	negative = -1;
    	x*=negative;
    	value*=negative;
    }

    y = (value-x)*1000;

    if (negative == -1) HAL_USART_Transmit(&USART_HandleStructure, (uint8_t *) "-", 1, TIMEOUT);
	printInt(x);
	printString(".");
	printInt(y);
}

void printString(char *c) {
    int length;
    length = strlen(c);
    
    HAL_USART_Transmit(&USART_HandleStructure, (uint8_t *)c, length, TIMEOUT);
    HAL_Delay(TRANS_DELAY);
}

void printComma() {
	char c = ',';

	HAL_USART_Transmit(&USART_HandleStructure, (uint8_t *)&c, 1, TIMEOUT);
	HAL_Delay(TRANS_DELAY);
}

void printNL() {
    char c = 0x0d;
    
    HAL_USART_Transmit(&USART_HandleStructure, (uint8_t *)&c, 1, TIMEOUT);
    HAL_Delay(TRANS_DELAY);
}
