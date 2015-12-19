/*
 * FlightControl.cpp
 *
 *  Created on: Oct 21, 2015
 *      Author: adrien
 */

#include "FlightControl.h"

#include <wirish_math.h>
#include "../../data/conf/Conf.h"
#include "../../math/common/FastMath.h"

FlightControl::FlightControl(RadioControler *radioController,
		FlightStabilization *flightStabilization,
		AHRS *ahrs) : Processing()
{
	_radioController = radioController;
	_flightStabilization = flightStabilization;
	_ahrs = ahrs;
	_yawInt = 0.0;

	// Runs at 50Hz
	_freqHz = 50;
	_throttleInitUs = 370;

	// Retrieve conf parameters
	_maxAbsRollAngle = Conf::getInstance().get("maxAbsRollAngle");
	_maxAbsPitchAngle = Conf::getInstance().get("maxAbsPitchAngle");
	_maxAbsCombinedAngle = Conf::getInstance().get("maxAbsCombinedAngle");
}

/**
 * Initialize flight controller
 */
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
	float roll = radioToRad(_radioController->getHandler().getChannelNormed(1), _maxAbsRollAngle->getValue());
	float pitch = radioToRad(_radioController->getHandler().getChannelNormed(2), _maxAbsPitchAngle->getValue());
	float yaw = radioToRad(_radioController->getHandler().getChannelNormed(4), _maxAbsCombinedAngle->getValue());
	// Throttle from 0 to 1
	float throttle = (_radioController->getHandler().Channel(3) - _throttleInitUs) / 1310.0;

	rpy[0] = roll;
	rpy[1] = pitch;

	// Integrate desired yaw
	const float Kyaw = 4.0;
	_yawInt += Kyaw * yaw * 1/_freqHz;
	_yawInt = _yawInt * 0.995;
	Bound(_yawInt, -PI, PI); //FIXME when max left and turns left, go other side ..

	// Transform RPY to quaternion
	Quaternion attitudeDesired = Quaternion::fromEuler(roll, pitch, _yawInt);


	// Flight stabilization
	// ------------------
	// Update input parameters of flight stabilization function
	_flightStabilization->setInputs(attitudeDesired, _ahrs->getAttitude(), _ahrs->getYawFromGyro(), _ahrs->getGyro().getGyroFiltered(), throttle);
}


/**
 * Converts radio signal to angle in radians.
 * Radio signal must be contained in [-1; 1] interval.
 */
float FlightControl::radioToRad(float radioNormed, float maxAngle)
{
	return radioNormed * maxAngle;
}
