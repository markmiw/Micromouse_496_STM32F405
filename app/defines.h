/* File Name: defines.h
 * Project: EE 396
 * Team: TSM
 * Members: Mark Miw
 *          Steven Chen
 *          Joseph Felix Jr.
 */

#ifndef defines_H
#define defines_H

#define PR	.01		// P constant for correction on right motor
#define PL	.01		// P constant for correction on left motor
#define DR	.005	// D constant for correction on right motor
#define DL	.005	// D constant for correction on left motor
#define BR	.0003	// B constant for brake system on right motor
#define BL	.0005	// B constant for brake system on left motor

#define TIMEOUT 100

#define ON  1		// Used for on/off states
#define OFF 0		// Used for on/off states

#define PERIOD 1000	// Motor period

#define TRANS_DELAY 200	// Delay between USART transmissions

#define BREAK_k	50					// Brake slow-down/speedup rate
#define brakeThresh	100				// Brake time wait in milliseconds
#define CORRECTION_FRONT_THRESH	10	// Threshold for stopping infront of walls
#define R_ENCODER_DIST	0.00200001	// Right encoder tick per cm 0.0020000 is left bias
#define L_ENCODER_DIST	0.00205		// Left encoder tick per cm
#define VELOCITY_k_R	25			// Velocity slow-down/speedup rate right motor
#define VELOCITY_k_L	25			// Velocity slow-down/speedup rate left motor
#define BRAKE_k_R		50
#define BRAKE_k_L		50
#define CELL_R			4395
#define CELL_L			4377

/* Enumeration Variable for ADC Channels */
typedef enum
{
	LEFT_DET,
	LEFT_CEN_DET,
	RIGHT_CEN_DET,
	RIGHT_DET,
	VOLT_DET,
	GYRO,
	FLASH_MEM
}ADC_Channel;

typedef enum
{
	RIGHTWALL,
	LEFTWALL,
	NOWALL
}Wall_Correction;

typedef enum {
    LEFT,
    RIGHT,
    CENTER,
    FRONT,
    FRONTCENTER,
    FARLEFTWALL,
    FARRIGHTWALL,
    IDEALLEFTFRONT,
    IDEALRIGHTFRONT,
    IDEALLEFTCENTER,
    IDEALRIGHTCENTER,
    CENTERRIGHTFRONT,
    CENTERLEFTFRONT
}Wall;

/* Enumeration Variable for Encoder Channels */
typedef enum
{
	LEFTENCODER,
	RIGHTENCODER
}Encoder;

/* Enumeration for Motor Channels */
typedef enum
{
	LEFTMOTOR,
	RIGHTMOTOR
}Motor;

/* Enumeration for LEDs */
typedef enum
{
	WHITE,
	BLUE,
	GREEN,
	RED
}Led;

/* Enumeration for Motor Direction */
typedef enum
{
	FORWARD,
	BACKWARD
}Direction;

#endif
