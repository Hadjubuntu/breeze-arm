/*
 * FlightControl.cpp
 *
 *  Created on: Oct 21, 2015
 *      Author: adrien
 */

#include "FlightControl.h"
#include "../../math/common/FastMath.h"

FlightControl::FlightControl(RadioControler *radioController,
		FlightStabilization *flightStabilization,
		AHRS *ahrs)
{
	_radioController = radioController;
	_flightStabilization = flightStabilization;
	_ahrs = ahrs;

	// Runs at 50Hz
	_freqHz = 50;

	_throttleInitUs = 400;
}

void FlightControl::init()
{
	// Init throttle at minimum value [us]
	_throttleInitUs = _radioController->getHandler().Channel(3);
}

void FlightControl::process()
{
	// Check radio state mode: auto or manual
	//	_radioController->isAutoMode();

	// AUTO mode
	// ------------------

	// Update mission naviguation

	// MANUAL mode
	// ------------------

	// Compute roll, pitch, yaw desired by using the radio values
	float roll = radioToRad(_radioController->getHandler().getChannelNormed(1));
	float pitch = radioToRad(_radioController->getHandler().getChannelNormed(2));
	float yaw = radioToRad(_radioController->getHandler().getChannelNormed(4));
	// Throttle from 0 to 1
	float throttle = (_radioController->getHandler().Channel(3) - _throttleInitUs) / 1000.0;

	// Transform RPY to quaternion
	Quaternion attitudeDesired = Quaternion::fromEuler(roll, pitch, yaw);

	// Update input parameters of flight stabilization function
	_flightStabilization->setInputs(attitudeDesired, _ahrs->getAttitude(), _ahrs->getGyro().getGyroFiltered(), throttle);
}


float FlightControl::radioToRad(float radioNormed)
{
	float maxAngle = FastMath::toRadians(40);
	return  radioNormed * maxAngle;
}
