/*
 * FlightControl.cpp
 *
 *  Created on: Oct 4, 2015
 *      Author: adrien
 */

#include "FlightControl.h"


/**
 * Flight control works on quaternion to prevent from glimbal lock.
 * Reference paper : stabilization using quaternion
 * http://www.nt.ntnu.no/users/skoge/prost/proceedings/ecc-2013/data/papers/0927.pdf
 */

FlightControl::FlightControl() :
_Pq(20), _Pw(4),
_targetAttitude(Quaternion::zero()), _currentAttitude(Quaternion::zero()),
_gyroRot(Vect3D::zero()),
_tau(Vect3D::zero())
{
	_freqHz = 50;
}


void FlightControl::setInputs(Quaternion pTargetAttitude, Quaternion pCurrentAttitude, Vect3D pGyroRot)
{
	_targetAttitude = pTargetAttitude;
	_currentAttitude = pCurrentAttitude;
	_gyroRot = pGyroRot.toRad(); // FIXME works with radians in gyro
}


void FlightControl::process()
{
	// Compute error from attitude commanded to current attitude using the combined rotation
	Quaternion qError = _targetAttitude * (_currentAttitude.conjugate());

	// Axis error
	Vect3D axisError = qError.getVect3DPart();

	// Compute tau rotation
	float rpy[3];
	qError.toRollPitchYaw(rpy);
	_tau = ((axisError * _Pq) + ( _gyroRot * _Pw)) * (-1); //  Vect3D(rpy[0], rpy[1], rpy[2]);

	//
	// Update PID output for attitude

	// Update PID output for thrust
}
