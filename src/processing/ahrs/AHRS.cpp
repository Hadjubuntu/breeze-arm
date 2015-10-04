/*
 * AHRS.cpp
 *
 *  Created on: Sep 22, 2015
 *      Author: adrien
 */

#include <stdio.h>
#include "AHRS.h"

inline float _fast_invsqrtf(float v)   // rel. err. < 0.07%
{                               // Jan Kaldec, http://rrrola.wz.cz/inv_sqrt.html
	union {float f; unsigned int u;} i = {v};
	i.u = 0x5F1FFFF9u - (i.u >> 1);
	return 0.703952253f * i.f * (2.38924456f - v * i.f * i.f);
}


float gyro_correct_int[2];

AHRS::AHRS() : Processing(), _grot(Vect3D::zero()),
		_attitude(Quaternion::zero()),
		_gyro(Gyro::create()),
		_accelerometer(Accelerometer::create())
{
	// 400 Hz update
	_freqHz = 400;

	gyro_correct_int[0] = 0.0;
	gyro_correct_int[1] = 0.0;
	_roll = 0.0;
	_pitch = 0.0;
	_yaw = 0.0;
}


void AHRS::initSensors()
{
	_accelerometer.init();
	_gyro.init();
}



void AHRS::process()
{
	const float accelKi = 0.0001;
	const float accelKp = 0.03;
	const float rollPitchBiasRate = 0.01;

	_accelerometer.update();
	_gyro.update();

	// Retrieve Gyro
	Vect3D gyros = _gyro.getGyroFiltered();
	// Correct value
	gyros.setX(gyros.getX() + gyro_correct_int[0]);
	gyros.setY(gyros.getY() + gyro_correct_int[1]);
	// TODO zeroing the z axis gyro val

	// Zeroing error
	// Force the roll & pitch gyro rates to average to zero
	gyro_correct_int[0] *= rollPitchBiasRate;
	gyro_correct_int[1] *= rollPitchBiasRate;

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
	float  invGRotMag = _fast_invsqrtf(gyros.getNorm());

	if (invGRotMag > 1e3f) {
		return;
	}
	const float invMag = (invAccelMag * invGRotMag);
	accelError *= invMag;

	// Accumulate integral of error.  Scale here so that units are (deg/s) but Ki has units of s
	gyro_correct_int[0] += accelError[0] * accelKi;
	gyro_correct_int[1] += accelError[1] * accelKi;


	// Correct rates based on error, integral component dealt with in updateSensors
	if (_dt > 0) {
		const float kpInvdT = accelKp / _dt;
		gyros += (accelError * kpInvdT);
	}

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
		_logger.error("inv_qmag > 1e3f");
	} else {
		_attitude *= inv_qmag;
	}

	float rpy[3];
	_attitude.toRollPitchYaw(rpy);

	_roll = rpy[1];
	_pitch = rpy[0];
}
