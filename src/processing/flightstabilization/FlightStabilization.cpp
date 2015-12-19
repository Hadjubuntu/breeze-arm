/*
 * FlightControl.cpp
 *
 *  Created on: Oct 4, 2015
 *      Author: adrien
 */

#include "../../data/conf/Conf.h"
#include "../../math/common/FastMath.h"
#include "../flightstabilization/FlightStabilization.h"


/**
 * Flight control works on quaternion to prevent from glimbal lock.
 * Reference paper : stabilization using quaternion
 * http://www.nt.ntnu.no/users/skoge/prost/proceedings/ecc-2013/data/papers/0927.pdf
 */

// good values : Pq=20; Pw=3
FlightStabilization::FlightStabilization() :
Processing(),
_Pq(16.0), _Pw(1.5),
_targetAttitude(Quaternion::zero()), _currentAttitude(Quaternion::zero()),
_gyroRot(Vect3D::zero()),
_tau(Vect3D::zero())
{
	_freqHz = 100;
	_throttle = 0;
	_yawFromGyro = 0.0;
	_Pq = Conf::getInstance().get("flightStabilization_Pq");
	_Pw = Conf::getInstance().get("flightStabilization_Pw");
}

/**
 * Set input parameters
 * @param pTargetAttitude
 * @param pCurrentAttitude
 * @param pGyroRot
 * @param pThrottle given between 0 and 1
 */
void FlightStabilization::setInputs(Quaternion pTargetAttitude, Quaternion pCurrentAttitude, float yawGyro, Vect3D pGyroRot, float pThrottle)
{
	_targetAttitude = pTargetAttitude;
	_currentAttitude = pCurrentAttitude;
	_gyroRot = pGyroRot;
	_throttle = pThrottle;
	_yawFromGyro = yawGyro;
}


void FlightStabilization::process()
{
	// DEBUG - tricks on yaw
	Vect3D currentAttVect3D = _currentAttitude.toRollPitchYawVect3D();
	_currentAttitude = Quaternion(currentAttVect3D.getX(), currentAttVect3D.getY(), _yawFromGyro);

	// Compute error from attitude commanded to current attitude using the combined rotation
	Quaternion qError = _targetAttitude * (_currentAttitude.conjugate());

	// Axis error
	Vect3D axisError = qError.getVect3DPart();

	// Compute tau from error and gyro rate
	// Note:
	// axisError positive roll right, pitch up
	// and gyroRot positive pitching up, rolling right (see Gyro.cpp for sign from raw data)
	_tau = ((axisError * _Pq->getValue()) * (-1)) + ( _gyroRot * _Pw->getValue());

	if (Conf::getInstance().useBoostMotors)
	{
		_throttleOut = boostThrottleCompensateTiltAngle(_throttle);
	}
	else
	{
		_throttleOut = _throttle;
	}
}

float FlightStabilization::boostThrottleCompensateTiltAngle(float throttle)
{
	Vect3D rpy = _currentAttitude.toRollPitchYawVect3D();
	float combinedTilt = abs(cos(rpy.getX()) * cos(rpy.getY()));
	float factor = 1.0;

	if (isSafeToUseBoost(throttle, combinedTilt))
	{
		factor = 1.0 / combinedTilt;
	}
	Bound(factor, 1.0, 1.5);

	return factor * throttle;
}

bool FlightStabilization::isSafeToUseBoost(float throttle, float combinedTilt)
{
	return throttle > 50 && combinedTilt > 0.0;
}


