/* File Name: calibration.h
 * Project: EE 396
 * Team: TSM
 * Members: Mark Miw
 *          Steven Chen
 *          Joseph Felix Jr.
 */

#ifndef calibration_H
#define calibration_H

#include "global_libs.h"
#include "HAL_includes.h"
#include "defines.h"

#include "adc.h"
#include "interface.h"

/*=Public Functions===========================================================*/

void calibrateSensors(void);
void calibrateWall(Wall wall);

/* Rename getFarWall functions */
double getWall(Wall wall);
double toLinear(uint16_t input);

#endif
