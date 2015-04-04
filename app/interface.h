/* File Name: interface.h
 * Project: EE 396
 * Team: TSM
 * Members: Mark Miw
 *          Steven Chen
 *          Joseph Felix Jr.
 */

#ifndef interface_H
#define interface_H

#include "global_libs.h"
#include "HAL_includes.h"
#include "defines.h"

#include "adc.h"
#include "motor.h"

void waitForTop(void);

void setLED(Led color);
void resetLED(Led color);
void setLEDAll(void);
void resetLEDAll(void);
void toggleLED(Led color);
void toggleLEDAll(void);

void setBuzzer(int state);
void testChaser(int mode, int period);
void testMenu(int channel);
void batteryFault();

bool getButton(void);

#endif
