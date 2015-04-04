/* File Name: motor.h
 * Project: EE 396
 * Team: TSM
 * Members: Mark Miw
 *          Steven Chen
 *          Joseph Felix Jr.
 */

#ifndef motor_H
#define motor_H

#include "global_libs.h"
#include "HAL_includes.h"
#include "defines.h"

#include "adc.h"
#include "encoder.h"
#include "interface.h"
#include "correction.h"

void brake();
void hardBrake();
void brakeLeft();
void brakeRight();
void setSpeed(Motor channel, int speed);
void setVelocity(double velocity);
void setRightVelocity(double velocity);
void setLeftVelocity(double velocity);
double getCurrentVelocity(Motor channel);
double getTargetVelocity(Motor channel);

#endif
