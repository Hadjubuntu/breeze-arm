/*
 * AHRS.cpp
 *
 *  Created on: Sep 22, 2015
 *      Author: adrien
 */

#include <stdio.h>
#include "AHRS.h"


float gyro_correct_int[3];

inline float _fast_invsqrtf(float number)   // rel. err. < 0.07%
{                               // Jan Kaldec, http://rrrola.wz.cz/inv_sqrt.html
	float x2, y;
	const float threehalfs = 1.5F;

	union {
		float    f;
		uint32_t u;
	} i;

	x2  = number * 0.5F;
	y   = number;

	i.f = y; // evil floating point bit level hacking
	i.u = 0x5f3759df - (i.u >> 1); // what the fxck?
	y   = i.f;
	y   = y * (threehalfs - (x2 * y * y));   // 1st iteration

	return y;
}



AHRS::AHRS() : Processing(), _grot(Vect3D::zero()),
		_attitude(Quaternion::zero()),
		_gyro(Gyro::create()),
		_accelerometer(Accelerometer::create())
{
	// 400 Hz update
	_freqHz = 400;

	gyro_correct_int[0] = 0.0;
	gyro_correct_int[1] = 0.0;
}


void AHRS::initSensors()
{
	_accelerometer.init();
	_gyro.init();
}

float* AHRS::getGyroCorr() {
	return gyro_correct_int;
}



void AHRS::process()
{
	const float accelKi = 0.01;
	const float accelKp = 0.1;
	const float rollPitchBiasRate = 0.999;

	_accelerometer.update();
	_gyro.update();

	// Retrieve Gyro
	Vect3D tmpGyros = _gyro.getGyroFiltered();
	// Correct value
	Vect3D gyros = Vect3D(tmpGyros.getX() + gyro_correct_int[0], tmpGyros.getY() + gyro_correct_int[1], tmpGyros.getZ());

	// Zeroing error
	// Force the roll & pitch gyro rates to average to zero
	gyro_correct_int[0] *= rollPitchBiasRate;
	gyro_correct_int[1] *= rollPitchBiasRate;

	// TODO as https://github.com/openpilot/OpenPilot/blob/master/flight/modules/Attitude/attitude.c
	//---------------------

	// Compute rotation
	float arrayGrot[3];
	arrayGrot[0] = -(2.0 * (_attitude[1] * _attitude[3] - _attitude[0] * _attitude[2]));
	arrayGrot[1] = -(2.0 * (_attitude[2] * _attitude[3] + _attitude[0] * _attitude[1]));
	arrayGrot[2] = -(_attitude[0] * _attitude[0] - _attitude[1] * _attitude[1] - _attitude[2] * _attitude[2] + _attitude[3] * _attitude[3]);
	_grot = Vect3D(arrayGrot[0], arrayGrot[1], arrayGrot[2]);


	// filter g rot
	Vect3D accelError = _accelerometer.getAccFiltered().crossProduct(_grot);
	float invAccelMag = _fast_invsqrtf(_accelerometer.getAccFiltered().getNorm2());

	if (invAccelMag > 1e3f) {
		return;
	}
	float  invGRotMag = _fast_invsqrtf(gyros.getNorm2());

	if (invGRotMag > 1e3f) {
		return;
	}
	const float invMag = (invAccelMag * invGRotMag);
	accelError *= invMag;

	// Accumulate integral of error.  Scale here so that units are (deg/s) but Ki has units of s
	gyro_correct_int[0] += accelError[0] * accelKi;
	gyro_correct_int[1] += accelError[1] * accelKi;


	// Correct rates based on error, integral component dealt with in updateSensors
	if (_dt > 0.0) {
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

	float inv_qmag = _fast_invsqrtf(_attitude.getNorm2());

	// If quaternion has become inappropriately short or is nan reinit.
	// THIS SHOULD NEVER ACTUALLY HAPPEN
	if ((fabsf(inv_qmag) > 1e3f) || isnan(inv_qmag)) {
		_attitude = Quaternion::zero();
		_logger.error("inv_qmag > 1e3f");
	} else {
		_attitude *= inv_qmag;
	}
}
