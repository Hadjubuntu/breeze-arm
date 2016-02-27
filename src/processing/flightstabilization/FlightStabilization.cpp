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
FlightStabilization::FlightStabilization(AHRS *ahrs, FlightControl *flightControl) :
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

	// Note that we use radian angles. It means 5 * 0.01 for integral means 2.86° correction for integral terms
	_pidRoll.init(_Krate->getValue(), 0.01, 0.01, 5);
	_pidPitch.init(_Krate->getValue(), 0.01, 0.01, 5);
	_pidAltitude.init(0.06 , 0.0, 0.04, 6);

	_ahrs = ahrs;
	_throttleOut = 0.0;
	_flightControl = flightControl;
}


void FlightStabilization::updateInputParameters()
{
	_targetAttitude = _flightControl->getAttitudeDesired();
	_throttle = _flightControl->getThrottleOut(); // Throttle is contained between [0; 1]

	_currentAttitude = _ahrs->getAttitude();
	_yawFromGyro = _ahrs->getYawFromGyro();
	_gyroRot = _ahrs->getGyro().getGyroFiltered();

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

	updateInputParameters();

	// DEBUG simple PID
	float rpyTarget[3];
	_targetAttitude.toRollPitchYaw(rpyTarget);
	float rpyCurrent[3];
	_currentAttitude.toRollPitchYaw(rpyCurrent);


	// Define angle rate from angle error
	float rollRate = (rpyTarget[0] - rpyCurrent[0]) * _Kangle->getValue();
	float pitchRate = (rpyTarget[1] - rpyCurrent[1]) * _Kangle->getValue();
	float yawRate = 1.6 * (rpyTarget[2] - _yawFromGyro) * _Kangle->getValue();

	BoundAbs(rollRate, 3.14);
	BoundAbs(pitchRate, 3.14);


	_pidRoll.setGainParameters(_Krate->getValue(), 0.01, 0.01);
	_pidPitch.setGainParameters(_Krate->getValue(), 0.01, 0.01);

	_pidRoll.update(rollRate - _gyroRot[0], 1/_freqHz);
	_pidPitch.update(pitchRate - _gyroRot[1], 1/_freqHz);

	_tau = Vect3D(_pidRoll.getOutput(),
			_pidPitch.getOutput(),
			1.6 *_Krate->getValue() * (yawRate - _gyroRot[2]));

	// Manual mode
	if (!_flightControl->isAutoMode()) {

		_pidAltitude.reset();

		if (Conf::getInstance().useBoostMotors)
		{
			_throttleOut = boostThrottleCompensateTiltAngle(_throttle);
		}
		else
		{
			_throttleOut = _throttle;
		}
	}
	// Auto mode
	else {
		if (_dt == 0.0) {
			_dt = 1.0/_freqHz;
		}
		float altitudeSetpointMeters = 0.6; // Test to 60 centimeters
		float errorAltMeters = altitudeSetpointMeters - _ahrs->getAltitudeMeters();
		float errorVz = 0.8*(errorAltMeters - _ahrs->getVz());
		Bound(errorVz, -1.0, 1.0); // -1 to 1 m/s climbrate

		_pidAltitude.update(errorVz, _dt);

		float estimThrottle = 0.4 + _pidAltitude.getOutput();
		Bound(estimThrottle, 0.0, 0.85); // Limit to 85% max throttle
		_throttleOut = estimThrottle;
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


