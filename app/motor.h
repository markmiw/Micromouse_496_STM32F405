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
void setBuzzer();
void setDirection(Motor channel, Direction state);
void setSpeed(Motor channel, uint32_t speed);
void toggleDirection(Motor channel);
void travelDistance(int distance, int maxSpeed, int dt);
