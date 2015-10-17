/**
 * Main source code of Breeze ARM project
 */
#include <stdio.h>
#include <wirish/wirish.h>
#include <vector>
#include "libraries/Wire/Wire.h"
#include "src/core/Brain.h"
#include "src/math/common/FastMath.h"
#include "src/processing/ahrs/AHRS.h"
#include "src/processing/flightcontrol/FlightControl.h"
#include "src/core/Logger.h"
#include "src/peripherals/IMU/Baro.h"
#include "src/link/RfControler.h"
#include "src/link/RfRouter.h"

AHRS ahrs;
FlightControl flightControl;
Brain uavBrain;
Logger logger;
RfControler rfControler;
RfRouter rfRouter(&rfControler);



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
	uavBrain.addProcessing(&rfControler);
	uavBrain.addProcessing(&rfRouter);

	ahrs.initSensors();
}

void loop()
{
	toggleLED();

	uavBrain.loop();

	if (uavBrain.getTickId() % 600 == 0)
	{
		// FIXME : how to set input with generic processings ?????
		flightControl.setInputs(Quaternion(0.0, 0.0, 0.0), ahrs.getAttitude(), ahrs.getGyro().getGyroFiltered());

		float rpy[3];
		ahrs.getAttitude().toRollPitchYaw(rpy);

		float* gyro_correct_int = ahrs.getGyroCorr();


		char str[90];
		sprintf(str, "Roll = %.1f | Pitch = %.1f | Acc(x) = %.3f | Acc(y) = %.3f | Acc(z) = %.3f",
				rpy[0], rpy[1], ahrs.getAcc().getAccFiltered().getX(), ahrs.getAcc().getAccFiltered().getY(), ahrs.getAcc().getAccFiltered().getZ()
		);



//		sprintf(str, "Rollacc: %.1f | GyroX: %.1f | GyroY: %.1f",
//					FastMath::toDegrees(atan(-ahrs.getAcc().getAccFiltered().getX() / ahrs.getAcc().getAccFiltered().getZ())),
//					ahrs.getGyro().getGyroFiltered().getX(),
//					ahrs.getGyro().getGyroFiltered().getY()
//			);

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
