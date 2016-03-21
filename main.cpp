/**
 * Main source code of Breeze ARM project
 * ----------------------------------------
 * @author : Adrien Hadj Salah
 * @date : september 2015
 */
#include <stdio.h>


#include <string.h>
#include <stdlib.h>

#include <wirish/wirish.h>
#include <wirish/ext_interrupts.h>
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
#include "src/peripherals/IMU/Baro.h"
#include "src/peripherals/HAL/HAL.h"


/** Attitude and heading reference system */
Baro baro;
AHRS ahrs(&baro);

/** UAV brain */
Brain uavBrain;

/** RadioFreq controller */
RfControler rfControler;

/** RadioFreq router */
RfRouter rfRouter(&rfControler);

/** Radio controller */
RadioControler radioControler;

/** Transform radio signal into radio flight control */
FlightControl flightControl(&radioControler);

/** Sonar to measure distance */
Sonar sonar;

/** Flight stabilization controller */
FlightStabilization flightStabilization(&ahrs, &flightControl, &sonar);

/** Motor and servo control */
ActuatorControl actuatorControl(&flightStabilization);

/** Telemetry to keep GCS update */
Telemetry telemetry(&ahrs, &flightControl, &rfControler);


void calibration()
{
	int nbCalibrationMeasure = 100;

	for (int i = 0; i < nbCalibrationMeasure; i++)
	{
		uavBrain.loop();
		ahrs.calibrateOffset();

		HAL::delayMs(10);
	}
}


void setup()
{
	pinMode(13, OUTPUT);
	// Add dependency (TODO delete this, brain as singleton, call for processing from brain)
	//---------------------
	Conf::getInstance().setRfControler(&rfControler);

	// Initialize brain
	//---------------------
	uavBrain.enableI2C();

	// Add processings
	//----------------------
	uavBrain.addProcessing(&baro);
	uavBrain.addProcessing(&ahrs);
	uavBrain.addProcessing(&rfControler);
	uavBrain.addProcessing(&rfRouter);
	uavBrain.addProcessing(&radioControler);
	uavBrain.addProcessing(&flightControl);
	uavBrain.addProcessing(&sonar);
	uavBrain.addProcessing(&flightStabilization);
	uavBrain.addProcessing(&actuatorControl);
	uavBrain.addProcessing(&telemetry);

	// Initialize all processings
	//----------------------
	uavBrain.initProcessings();

	// Calibration on AHRS
	//----------------------
	calibration();
}

// Test interpolation on IR value
float approxDist(int irValue)
{
	float res = 0.0;

	if (irValue < 3500 && irValue >= 1450) {
		res = ((3500 - irValue) * 50 + (irValue - 1450) * 15) / (3500 - 1450);
	}
	else if (irValue < 1450 && irValue >= 950) {
		res = ((1450 - irValue) * 80 + (irValue - 950) * 50) / (1450 - 950);
	}
	else if (irValue < 950 && irValue >= 800) {
		res = ((950 - irValue) * 120 + (irValue - 800) * 80) / (950 - 800);
	}
	else if (irValue < 800 && irValue >= 700) {
		res = ((800 - irValue) * 150 + (irValue - 700) * 120) / (800 - 700);
	}
	else if (irValue < 700)
	{
		res = 150.0;
	}


	return res;
}


/**
 * Loop function called as fast as possible
 * Measure : from 3khz to minimum 700Hz
 */

int currentSize = 0;
char currentPacket[85];
void str_resetCharArray(char *p) {
	int nbBytes = strlen(p);
	memset(&p[0], 0, nbBytes);
}


void loop()
{
	// Call brain loop function to udpate the processings
	// ----
	uavBrain.loop();

	//	if (uavBrain.getTickId() % 50 == 0)
	//	{
	//		if (currentSize <= 70) {
	//			sprintf(currentPacket, "%s|%.2f", currentPacket, flightStabilization.getThrottle()); // flightStabilization.getThrottle()); // ahrs.getVz()
	//			currentSize += 8;
	//		}
	//		else
	//		{
	//			sprintf(currentPacket, "%s|END", currentPacket);
	//			RfPacket packet(Date::now(), "LOG", currentPacket);
	//			rfControler.addPacketToSend(packet);
	//
	//			currentSize = 0;
	//			str_resetCharArray(currentPacket);
	//		}
	//	}


	// Prints infos
	// ----
	if (uavBrain.getTickId() % 2000 == 0)
	{
		float rpy[3];
		ahrs.getAttitude().toRollPitchYaw(rpy);

		char str[90];

		//		int irValue = analogRead(13);
		//		float distanceCM = (4096-irValue) * 0.03662115 ; // 1/4096*150cm


//		sprintf(str, "r=%.1f|p=%.1f|alt=%.1f cm|T=%.1f deg|p=%.1f hPa|gp=%.1f hPa|t=%.2f", // |baroAlt = %.2f|Temp=%.2f , baro.getAltitudeMeters(), baro.getTemperature()
//				FastMath::toDegrees(rpy[0]), FastMath::toDegrees(rpy[1]),
//				baro.getAltitudeMeters()*100.0f,
//				baro.getTrueTemperature()/10.0f,
//				baro.getTruePressure()/100.0f, baro.getGroundPressure()/100.0f,
//				flightStabilization.getThrottle()) ;

		sprintf(str, "r=%.1f|p=%.1f|alt=%.1f cm|baro_alt=%.2f|error_alt=%.3f", // |baroAlt = %.2f|Temp=%.2f , baro.getAltitudeMeters(), baro.getTemperature()
				FastMath::toDegrees(rpy[0]), FastMath::toDegrees(rpy[1]),
				baro.getAltitudeMeters()*100.0f,
				sonar.getOutput(),
				flightStabilization.getErrorAltitude()) ;



		RfPacket packet(Date::now(), "LOG", str);
		rfControler.addPacketToSend(packet);
	}

	// Slow toggle led
	if (uavBrain.getTickId() % 8000 == 0)
	{
		//		Serial3.println("LOG|test|ok");
		toggleLED();
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
