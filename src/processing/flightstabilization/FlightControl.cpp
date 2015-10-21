/*
 * FlightControl.cpp
 *
 *  Created on: Oct 21, 2015
 *      Author: adrien
 */

#include "FlightControl.h"

FlightControl::FlightControl(RadioControler *radioController,
		FlightStabilization *flightStabilization,
		AHRS *ahrs)
{
	_radioController = radioController;
	_flightStabilization = flightStabilization;
	_ahrs = ahrs;

	// Runs at 50Hz
	_freqHz = 50;
}

void FlightControl::process()
{
	// Check radio state mode: auto or manual
//	_radioController->isAutoMode();

	// In manual mode
	// Compute roll, pitch, yaw desired by using the radio values
	float roll = 0.0;
	float pitch = 0.0;
	float yaw = 0.0;

	// Transform RPY to quaternion
	Quaternion attitudeDesired = Quaternion::fromEuler(roll, pitch, yaw);

	// Update input parameters of flight stabilization function
	_flightStabilization->setInputs(attitudeDesired, _ahrs->getAttitude(), _ahrs->getGyro().getGyroFiltered());
}

