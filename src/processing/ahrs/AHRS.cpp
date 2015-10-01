/*
 * AHRS.cpp
 *
 *  Created on: Sep 22, 2015
 *      Author: adrien
 */

#include "AHRS.h"

AHRS::AHRS() : _attitude(Quaternion::zero()),
_gyro(Gyro::create()),
_accelerometer(Accelerometer::create())

{

}

void AHRS::initSensors()
{
	_accelerometer.init();
	_gyro.init();
}


void AHRS::process()
{
	// Call parent function TODO create execute : update / set time ..
	Processing::process();

	_accelerometer.update();
	_gyro.update();
}
