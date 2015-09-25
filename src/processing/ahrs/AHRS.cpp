/*
 * AHRS.cpp
 *
 *  Created on: Sep 22, 2015
 *      Author: adrien
 */

#include "AHRS.h"

AHRS::AHRS() : _attitude(Quaternion::zero()), _accelerometer(Accelerometer::create())
{

}

void AHRS::initSensors()
{
	_accelerometer.init();
}
