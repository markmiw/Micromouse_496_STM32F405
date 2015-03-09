#define PERIOD 600

typedef enum
{
	LEFTMOTOR,
	RIGHTMOTOR
}Motor;

typedef enum
{
	FORWARD,
	BACKWARD
}Direction;

void initMotor();
void setBuzzer(int state);
void setTimer_ms(int state);
void setDirection(Motor channel, Direction state);
uint32_t currentSpeed(Motor channel);
void setSpeed(Motor channel, uint32_t speed);
void toggleDirection(Motor channel);
void travelDistance(uint32_t distance, uint32_t maxSpeed, uint32_t dt);

extern int timer;
