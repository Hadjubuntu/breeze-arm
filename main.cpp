/**
 * Main source code of Breeze ARM project
 */
#include <stdio.h>
#include <wirish/wirish.h>
#include <vector>
#include "libraries/Wire/Wire.h"
#include "src/core/Brain.h"
#include "src/processing/ahrs/AHRS.h"
#include "src/core/Logger.h"

AHRS ahrs;
Brain uavBrain;
Logger logger;


void setup() {
	/* Set up the LED to blink  */
	pinMode(BOARD_LED_PIN, OUTPUT);

	logger.info("BREEZE-UAV-ARM");

	// Initialize brain
	//---------------------
	uavBrain.init();

	// Add processings
	//----------------------
	uavBrain.addProcessing(&ahrs);
	ahrs.initSensors();
}

void loop()
{
	toggleLED();

	uavBrain.loop();

	if (uavBrain.getTickId() % 100 == 0)
	{
		char str[90];
		sprintf(str, "Roll: %.2f",
				ahrs.getRoll()
		);

		logger.info(str);
	}
}

// Force init to be called *first*, i.e. before static object allocation.
// Otherwise, statically allocated objects that need libmaple may fail.
__attribute__((constructor)) void premain() {
	init();
}

int main(void) {
	setup();

	while (true) {
		loop();
	}

	return 0;
}
