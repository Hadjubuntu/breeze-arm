/*
 * FlightControl.cpp
 *
 *  Created on: Oct 4, 2015
 *      Author: adrien
 */

#include "../../data/conf/Conf.h"
#include "../flightstabilization/FlightStabilization.h"


/**
 * Flight control works on quaternion to prevent from glimbal lock.
 * Reference paper : stabilization using quaternion
 * http://www.nt.ntnu.no/users/skoge/prost/proceedings/ecc-2013/data/papers/0927.pdf
 */

// strong value 12 | 1.0
FlightStabilization::FlightStabilization() :
Processing(),
_Pq(20.0), _Pw(3.0),
_targetAttitude(Quaternion::zero()), _currentAttitude(Quaternion::zero()),
_gyroRot(Vect3D::zero()),
_tau(Vect3D::zero())
{
	_freqHz = 50;
	_throttle = 0;
}

/**
 * Set input parameters
 * @param pTargetAttitude
 * @param pCurrentAttitude
 * @param pGyroRot
 * @param pThrottle given between 0 and 1
 */
void FlightStabilization::setInputs(Quaternion pTargetAttitude, Quaternion pCurrentAttitude, Vect3D pGyroRot, float pThrottle)
{
	_targetAttitude = pTargetAttitude;
	_currentAttitude = pCurrentAttitude;
	_gyroRot = pGyroRot;
	_throttle = pThrottle;
}


void FlightStabilization::process()
{
	// Compute error from attitude commanded to current attitude using the combined rotation
	Quaternion qError = _targetAttitude * (_currentAttitude.conjugate());

	// Axis error
	Vect3D axisError = qError.getVect3DPart();

	// Compute tau from error and gyro rate
	// Note:
	// axisError positive roll right, pitch up
	// and gyroRot positive pitching up, rolling right (see Gyro.cpp for sign from raw data)
	float rpy[3];
	qError.toRollPitchYaw(rpy);
	_tau = ((axisError * _Pq) * (-1)) + ( _gyroRot * _Pw);

	if (Conf::getInstance().useBoostMotors)
	{
		// TODO compute delta angle from quaternion attitude and quat ez axis

		_throttleOut = _throttle;
	}
	else {
		// Keep throttle as given
		_throttleOut = _throttle;
	}
}


