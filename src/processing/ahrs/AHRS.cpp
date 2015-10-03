/*
 * AHRS.cpp
 *
 *  Created on: Sep 22, 2015
 *      Author: adrien
 */

#include "AHRS.h"

inline float _fast_invsqrtf(float v)   // rel. err. < 0.07%
{                               // Jan Kaldec, http://rrrola.wz.cz/inv_sqrt.html
	union {float f; unsigned int u;} i = {v};
	i.u = 0x5F1FFFF9u - (i.u >> 1);
	return 0.703952253f * i.f * (2.38924456f - v * i.f * i.f);
}


float gyro_correct_int[2];

AHRS::AHRS() : _grot(Vect3D::zero()), _attitude(Quaternion::zero()),
		_gyro(Gyro::create()),
		_accelerometer(Accelerometer::create())
{
	// 400 Hz update
	_freqHz = 400;
}

void AHRS::initSensors()
{
	_accelerometer.init();
	_gyro.init();
}

void Quaternion2RPY(Quaternion q, float rpy[3])
{
    float R13, R11, R12, R23, R33;
    float q0s = q[0] * q[0];
    float q1s = q[1] * q[1];
    float q2s = q[2] * q[2];
    float q3s = q[3] * q[3];

    R13    = 2 * (q[1] * q[3] - q[0] * q[2]);
    R11    = q0s + q1s - q2s - q3s;
    R12    = 2 * (q[1] * q[2] + q[0] * q[3]);
    R23    = 2 * (q[2] * q[3] + q[0] * q[1]);
    R33    = q0s - q1s - q2s + q3s;

    float RAD2DEG = 57.29577951;

    rpy[1] = RAD2DEG * asinf(-R13); // pitch always between -pi/2 to pi/2
    rpy[2] = RAD2DEG * atan2f(R12, R11);
    rpy[0] = RAD2DEG * atan2f(R23, R33);

    // TODO: consider the cases where |R13| ~= 1, |pitch| ~= pi/2
}

void AHRS::process()
{
	const float accelKi = 0.0005;
	const float accelKp = 0.004;

	_accelerometer.update();
	_gyro.update();

	// TODO as https://github.com/openpilot/OpenPilot/blob/master/flight/modules/Attitude/attitude.c
	//---------------------

	// TODO accel filter
	_grot.setX(-(2.0 * (_attitude[1] * _attitude[3] - _attitude[0] * _attitude[2])));
	_grot.setY(-(2.0 * (_attitude[2] * _attitude[3] + _attitude[0] * _attitude[1])));
	_grot.setZ(-(_attitude[0] * _attitude[0] - _attitude[1] * _attitude[1] - _attitude[2] * _attitude[2] + _attitude[3] * _attitude[3]));

	// filter g rot
	Vect3D accelError = _accelerometer.getAccFiltered().crossProduct(_grot);
	float invAccelMag = _fast_invsqrtf(_accelerometer.getAccFiltered().getNorm());

	if (invAccelMag > 1e3f) {
		return;
	}
	float  invGRotMag = _fast_invsqrtf(_gyro.getGyroFiltered().getNorm());

	if (invGRotMag > 1e3f) {
		return;
	}
	const float invMag = (invAccelMag * invGRotMag);
	accelError *= invMag;

	// Accumulate integral of error.  Scale here so that units are (deg/s) but Ki has units of s
	gyro_correct_int[0] += accelError[0] * accelKi;
	gyro_correct_int[1] += accelError[1] * accelKi;

	// Correct rates based on error, integral component dealt with in updateSensors
	const float kpInvdT = accelKp / _dt;
	Vect3D gyros = _gyro.getGyroFiltered();
	gyros += (accelError * kpInvdT);

	float qdot[4];
	float M_PI_F = 3.141592653;
	qdot[0] = (-_attitude[1] * gyros[0] - _attitude[2] * gyros[1] - _attitude[3] * gyros[2]) * _dt * (M_PI_F / 180.0f / 2.0f);
	qdot[1] = (_attitude[0] * gyros[0] - _attitude[3] * gyros[1] + _attitude[2] * gyros[2]) * _dt * (M_PI_F / 180.0f / 2.0f);
	qdot[2] = (_attitude[3] * gyros[0] + _attitude[0] * gyros[1] - _attitude[1] * gyros[2]) * _dt * (M_PI_F / 180.0f / 2.0f);
	qdot[3] = (-_attitude[2] * gyros[0] + _attitude[1] * gyros[1] + _attitude[0] * gyros[2]) * _dt * (M_PI_F / 180.0f / 2.0f);

	// Take a time step
	_attitude += qdot;

	if (_attitude[0] < 0) {
		_attitude = -_attitude;
	}

	float inv_qmag = _fast_invsqrtf(_attitude.getNorm());

	// If quaternion has become inappropriately short or is nan reinit.
	// THIS SHOULD NEVER ACTUALLY HAPPEN
	if ((fabsf(inv_qmag) > 1e3f) || isnan(inv_qmag)) {
		_attitude = Quaternion::zero();
	} else {
		_attitude *= inv_qmag;
	}

	float rpy[3];
	Quaternion2RPY(_attitude, rpy);

	Serial3.print("roll:");
	Serial3.println(rpy[0]);
}
