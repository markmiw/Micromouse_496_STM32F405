typedef enum {
	WHITE,
	BLUE,
	GREEN,
	RED
}Led;

typedef enum {
	ON,
	OFF
}State;

/* Public Functions */
void initLED(void);
void setLED(Led color);
void resetLED(Led color);
void setLEDAll(void);
void resetLEDAll(void);
void toggleLEDAll(void);
void toggleLED(Led color);

/* Private Functions */
