/*
 * Quaternion.cpp
 *
 *  Created on: Sep 22, 2015
 *      Author: adrien
 */

#include <math.h>
#include "../common/FastMath.h"
#include "Quaternion.h"


/**
 * Constructor from roll pitch yaw
 * */
Quaternion::Quaternion(float roll, float pitch, float yaw)
{
	/** TODO */
	_w = 0.0;
	_x = 0.0;
	_y = 0.0;
	_z = 0.0;
}

void Quaternion::toRollPitchYaw(float rpy[3])
{
	float R13, R11, R12, R23, R33;
	float q0s = _w * _w;
	float q1s = _x * _x;
	float q2s = _y * _y;
	float q3s = _z * _z;

	R13    = 2 * (_x * _z - _w * _y);
	R11    = q0s + q1s - q2s - q3s;
	R12    = 2 * (_x * _y + _w * _z);
	R23    = 2 * (_y * _z + _w * _x);
	R33    = q0s - q1s - q2s + q3s;

	rpy[1] = FastMath::toDegrees(asinf(-R13)); // pitch always between -pi/2 to pi/2
	rpy[2] = FastMath::toDegrees(atan2f(R12, R11));
	rpy[0] = FastMath::toDegrees(atan2f(R23, R33));
}


float Quaternion::getNorm2() {
	return _w *_w + _x*_x +_y*_y + _z*_z;
}

float Quaternion::getNorm() {
	return FastMath::sqrt(getNorm2());
}
