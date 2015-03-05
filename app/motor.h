#define LEFTMOTOR  0
#define RIGHTMOTOR 1

#define FORWARD  1
#define BACKWARD 0

#define PERIOD 600

void initMotor();
void setBuzzer();
void setDirection(int channel, int direction);
void setSpeed(int channel, int speed);
void toggleDirection(int channel);
void travelDistance(int distance, int maxSpeed, int dt);
