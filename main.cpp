/**
 * Main source code of Breeze ARM project
 * ----------------------------------------
 * @author : Adrien Hadj Salah
 * @date : september 2015
 */
#include <stdio.h>
#include <wirish/wirish.h>
#include <vector>
#include "libraries/Wire/Wire.h"
#include "src/core/Brain.h"
#include "src/math/time/Date.h"
#include "src/math/common/FastMath.h"
#include "src/processing/ahrs/AHRS.h"
#include "src/processing/link/RadioControler.h"
#include "src/core/Logger.h"
#include "src/peripherals/IMU/Baro.h"
#include "src/link/RfControler.h"
#include "src/link/RfRouter.h"
#include "src/processing/actuator/ActuatorControl.h"
#include "src/processing/flightstabilization/FlightStabilization.h"
#include "src/processing/flightstabilization/FlightControl.h"

/** Attitude and heading reference system */
AHRS ahrs;

/** Flight controller */
FlightStabilization flightStabilization;

/** UAV brain */
Brain uavBrain;

/** Logger */
Logger logger;

/** RadioFreq controller */
RfControler rfControler;

/** RadioFreq router */
RfRouter rfRouter(&rfControler);

/** Radio controller */
RadioControler radioControler;

/** Route flight mode to mission / stabilization */
FlightControl flightControl(&radioControler, &flightStabilization, &ahrs);

/** Motor and servo control */
ActuatorControl actuatorControl(&flightStabilization);

void setup() {
	/* Set up the LED to blink  */
	pinMode(BOARD_LED_PIN, OUTPUT);

	logger.info("BREEZE-UAV-ARM");

	// Initialize brain
	//---------------------
	uavBrain.enableI2C();

	// Add processings
	//----------------------
	uavBrain.addProcessing(&ahrs);
	uavBrain.addProcessing(&flightStabilization);
	uavBrain.addProcessing(&rfControler);
	uavBrain.addProcessing(&rfRouter);
	uavBrain.addProcessing(&radioControler);
	uavBrain.addProcessing(&flightControl);
	uavBrain.addProcessing(&actuatorControl);

	// Initialize all processings
	//----------------------
	uavBrain.initProcessings();
}

/**
 * Loop function called as fast as possible
 */
void loop()
{
	// Call brain loop function to udpate the processings
	// ----
	uavBrain.loop();

	// Prints infos
	// ----
	if (uavBrain.getTickId() % 600 == 0)
	{
		toggleLED();


		float rpy[3];
		ahrs.getAttitude().toRollPitchYaw(rpy);

		float* gyro_correct_int = ahrs.getGyroCorr();

		// TODO map channels to roll pitch yaw throttle and optionnal channels
		int ch1 = radioControler.getHandler().channels[1];


		char str[90];
		//		sprintf(str, "Roll = %.1f | Pitch = %.1f",
		//				rpy[0], rpy[1]
		//		);

		//		sprintf(str, "throttle: %.2f", flightStabilization.getThrottle());


		// Print tau
				sprintf(str, "tau_x = %.2f | tau_y = %.2f | rpy_x = %.1f | rpy_y = %.1f",
						flightStabilization.getTau().getX(),
						flightStabilization.getTau().getY(),
						flightControl.rpy[0], flightControl.rpy[1]);
//				sprintf(str, "X1 = %d | X2 = %d | X3 = %d | X4 = %d",
//								actuatorControl.motorMap[0],
//								actuatorControl.motorMap[1],
//								actuatorControl.motorMap[2],
//								actuatorControl.motorMap[3]
//								);

		logger.info(str);
	}
}

// Force init to be called *first*, i.e. before static object allocation.
// Otherwise, statically allocated objects that need libmaple may fail.
__attribute__((constructor)) void premain() {
	init();
}

// Startup function
int main(void) {
	setup();

	while (true) {
		loop();
	}

	return 0;
}
