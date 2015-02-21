#define WHITE 0
#define BLUE  1
#define GREEN 2
#define RED   3

#define ON  1
#define OFF 0

void initLED();
void setLED(int channel, int state);
void toggleLED(int channel);
