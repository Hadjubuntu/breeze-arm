/**
 * Main source code of Breeze ARM project
 */
#include <stdio.h>
#include <wirish/wirish.h>
#include <vector>
#include "libraries/Wire/Wire.h"
#include "src/core/Brain.h"
#include "src/processing/ahrs/AHRS.h"
#include "src/processing/flightcontrol/FlightControl.h"
#include "src/core/Logger.h"

AHRS ahrs;
FlightControl flightControl;
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
	uavBrain.addProcessing(&flightControl);

	ahrs.initSensors();
}

void loop()
{
	toggleLED();

	uavBrain.loop();

	if (uavBrain.getTickId() % 100 == 0)
	{
		// FIXME : how to set input with generic processings ?????
		flightControl.setInputs(Quaternion(1.0, 0.0, 0.0, 0.0), ahrs.getAttitude(), ahrs.getGyro().getGyroFiltered());

		float rpy[3];
		ahrs.getAttitude().toRollPitchYaw(rpy);

		char str[90];
		sprintf(str, "Roll = %.2f | Tau(x) = %.2f | Tau(y) = %.2f",
				rpy[1], flightControl.getTau().getX(), flightControl.getTau().getY()
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
