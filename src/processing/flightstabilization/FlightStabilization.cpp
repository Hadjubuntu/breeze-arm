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
_targetAttitude(Quaternion::zero()), _currentAttitude(Quaternion::zero()),
_gyroRot(Vect3D::zero()),
_tau(Vect3D::zero())
{
	_freqHz = 100;
	_throttle = 0;
	_yawFromGyro = 0.0;
	_Pq = Conf::getInstance().get("flightStabilization_Pq");
	_Pw = Conf::getInstance().get("flightStabilization_Pw");
	_Kangle = Conf::getInstance().get("flightStabilization_Kangle");
	_Krate = Conf::getInstance().get("flightStabilization_Krate");
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

#if REAL_VERSION
	Vect3D currentAttVect3D = _currentAttitude.toRollPitchYawVect3D();
	_currentAttitude = Quaternion(currentAttVect3D.getX(), currentAttVect3D.getY(), _yawFromGyro);

	// Compute error from attitude commanded to current attitude using the combined rotation
	Quaternion qError = _targetAttitude * (_currentAttitude.conjugate());

	// Axis error
	Vect3D axisError = qError.getVect3DPart();

	// Compute tau from error and gyro rate
	_tau = ((axisError * _Pq->getValue()) * (-1)) + ( _gyroRot * _Pw->getValue());

	if (Conf::getInstance().useBoostMotors)
	{
		_throttleOut = boostThrottleCompensateTiltAngle(_throttle);
	}
	else
	{
		_throttleOut = _throttle;
	}
#endif

	// DEBUG simple PID
	float rpyTarget[3];
	_targetAttitude.toRollPitchYaw(rpyTarget);
	float rpyCurrent[3];
	_currentAttitude.toRollPitchYaw(rpyCurrent);

	// Define angle rate from angle error
	float rollRate = (rpyTarget[0] - rpyCurrent[0]) * _Kangle->getValue();
	float pitchRate = (rpyTarget[1] - rpyCurrent[1]) * _Kangle->getValue();
	float yawRate = 2.0 * (rpyTarget[2] - _yawFromGyro) * _Kangle->getValue();

	BoundAbs(rollRate, 3.14);
	BoundAbs(pitchRate, 3.14);

	_tau = Vect3D(_Krate->getValue() * (rollRate - _gyroRot[0]),
			_Krate->getValue() * (pitchRate - _gyroRot[1]),
			2.0 *_Krate->getValue() * (yawRate - _gyroRot[2]));

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


