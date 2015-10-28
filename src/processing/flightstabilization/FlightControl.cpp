/*
 * FlightControl.cpp
 *
 *  Created on: Oct 21, 2015
 *      Author: adrien
 */

#include "FlightControl.h"

#include "../../data/conf/Conf.h"
#include "../../math/common/FastMath.h"

FlightControl::FlightControl(RadioControler *radioController,
		FlightStabilization *flightStabilization,
		AHRS *ahrs) : Processing()
{
	_radioController = radioController;
	_flightStabilization = flightStabilization;
	_ahrs = ahrs;

	// Runs at 50Hz
	_freqHz = 50;

	_throttleInitUs = 370;
}

void FlightControl::init()
{
	// Init throttle at minimum value [us]
	//TODO wait until value between 300 and 500 ..
	//	_throttleInitUs = _radioController->getHandler().Channel(3);
}

void FlightControl::process()
{
	// Check radio state mode: auto or manual
	//	_radioController->isAutoMode();

	// AUTO mode
	// ------------------

	// Update mission navigation

	// MANUAL mode
	// ------------------

	// Compute roll, pitch, yaw desired by using the radio values
	float roll = radioToRad(_radioController->getHandler().getChannelNormed(1), Conf::getInstance().maxAbsRollAngle);
	float pitch = radioToRad(_radioController->getHandler().getChannelNormed(2), Conf::getInstance().maxAbsPitchAngle);
	float yaw = radioToRad(_radioController->getHandler().getChannelNormed(4), Conf::getInstance().maxAbsCombinedAngle);
	// Throttle from 0 to 1
	float throttle = (_radioController->getHandler().Channel(3) - _throttleInitUs) / 1310.0;

	// Transform RPY to quaternion
	Quaternion attitudeDesired = Quaternion::fromEuler(roll, pitch, yaw);


	// Flight stabilization
	// ------------------

	// Update input parameters of flight stabilization function
	_flightStabilization->setInputs(attitudeDesired, _ahrs->getAttitude(), _ahrs->getGyro().getGyroFiltered(), throttle);
}


float FlightControl::radioToRad(float radioNormed, float maxAngle)
{
	return  radioNormed * maxAngle;
}
