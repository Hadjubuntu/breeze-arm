/**
 * Main source code of Breeze ARM project
 * ----------------------------------------
 * @author : Adrien Hadj Salah
 * @date : september 2015
 */
#include <stdio.h>
#include <wirish/wirish.h>
#include <vector>
#include "src/data/conf/Conf.h"
#include "libraries/Wire/Wire.h"
#include "src/core/Brain.h"
#include "src/math/time/Date.h"
#include "src/math/common/FastMath.h"
#include "src/processing/ahrs/AHRS.h"
#include "src/processing/link/RadioControler.h"
#include "src/peripherals/IMU/Baro.h"
#include "src/link/RfControler.h"
#include "src/link/RfRouter.h"
#include "src/processing/actuator/ActuatorControl.h"
#include "src/processing/flightstabilization/FlightStabilization.h"
#include "src/processing/flightstabilization/FlightControl.h"
#include "src/processing/nav/sonar/Sonar.h"
#include "src/processing/link/Telemetry.h"


/** Attitude and heading reference system */
AHRS ahrs;

/** Flight controller */
FlightStabilization flightStabilization;

/** UAV brain */
Brain uavBrain;


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

/** Telemetry to keep GCS update */
Telemetry telemetry(&ahrs);

/** Sonar to measure distance */
Sonar sonar;

void setup() {
	/* Set up the LED to blink  */
	pinMode(BOARD_LED_PIN, OUTPUT);

	// Add dependency (TODO delete this, brain as singleton, call for processing from brain)
	//---------------------
	Conf::getInstance().setRfControler(&rfControler);

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
	uavBrain.addProcessing(&telemetry);
	uavBrain.addProcessing(&sonar);

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
	if (uavBrain.getTickId() % 1000 == 0)
	{
		toggleLED();

		float rpy[3];
		ahrs.getAttitude().toRollPitchYaw(rpy);

		// throttle radio channel
		int chThrottle = radioControler.getHandler().Channel(3);

		char str[90];

		//		sprintf(str, "throttle: %.2f", flightStabilization.getThrottle());

		//		Quaternion targetAtt = flightStabilization.getTargetAttitude();
		//		targetAtt.toRollPitchYaw(rpy);

		Vect3D accelRaw = ahrs.getAcc().getAccRaw();
		sprintf(str, "acc_x=%.2f | acc_y=%.2f | acc_z=%.2f | norm=%.2f",
				accelRaw.getX(), accelRaw.getY(), accelRaw.getZ(), accelRaw.getNorm2());
		// Print tau
//				sprintf(str, "tau_x = %.1f | tau_y = %.1f | sonar = %.0f | sonar_raw = %.0f",
//						flightStabilization.getTau().getX(),
//						flightStabilization.getTau().getY(),
//						sonar.getOutput(), (float) analogRead(13) * 0.3175);

//		sprintf(str, "r = %.1f | p = %.1f | gyro_x = %.1f | gyro_y = %.1f | acc_x = %.1f | acc_y = %.1f",
//				FastMath::toDegrees(rpy[0]), FastMath::toDegrees(rpy[1]),
//				FastMath::toDegrees(ahrs.getGyro().getGyroFiltered().getX()),
//				FastMath::toDegrees(ahrs.getGyro().getGyroFiltered().getY()),
//				FastMath::toDegrees(ahrs.getAcc().getAccFiltered().getX()),
//				FastMath::toDegrees(ahrs.getAcc().getAccFiltered().getY())
//			);

		// 	radioControler.getHandler().getChannelNormed(1),
//		radioControler.getHandler().getChannelNormed(2)

// flightStabilization.getTau().getX(), flightStabilization.getTau().getY(),

		RfPacket packet(Date::now(), "LOG", str);
		rfControler.addPacketToSend(packet);
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
