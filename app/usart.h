/* File Name: usart.h
 * Project: EE 396
 * Team: TSM
 * Members: Mark Miw
 *          Steven Chen
 *          Joseph Felix Jr.
 */

#ifndef usart_H
#define usart_H

#include "global_libs.h"
#include "HAL_includes.h"
#include "defines.h"

#include "system.h"

/*
 * These functions are meant to allow communication with
 * Alex? via USART.
 */

/* */
void initUSART(void);
void printInt(int value);
void printString(char *c);
void printNL();
void printFloat(float value);
void printComma();

#endif
