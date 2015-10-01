/*
 * Gyro.cpp
 *
 *  Created on: Sep 22, 2015
 *      Author: adrien
 */
#include <wirish/wirish.h>
#include <stdio.h>
#include "libmaple/i2c.h"
#include "Gyro.h"



void Gyro::init()
{
	// Initialize gyro
	///----------------------------
	// Power control
	_i2c.writeTo(GYRO_PWR_MNGT,0x00);
	delay(5);
	_i2c.writeTo(GYRO_PWR_MNGT,0x00);
	delay(5);
	// Sample at 100Hz
	_i2c.writeTo(GYRO_SMPLRT_DIV, 0x09);
	delay(5);
	// Gyro +/- 2000 deg/s, low-pass filter at 100Hz
	_i2c.writeTo(GYRO_DLPF_FS, 0x1e);
	delay(5);
	//
	_i2c.writeTo(GYRO_INT_CFG, 0x00);

	delay(500);

	int num_samples = 100;
	float accumulator[] = {0,0,0};

	for(int i = 0 ; i < num_samples ; i++)
	{
		update();

		accumulator[0] += _gyroRaw.getX();
		accumulator[1] += _gyroRaw.getY();
		accumulator[2] += _gyroRaw.getZ();

		delay(5);
	}

	for(int i = 0 ; i < 3 ; i++)
	{
		accumulator[i] /= num_samples;
	}

	_offset = accumulator;
}

void Gyro::update()
{
	// Retrieve raw data
	//---------------------
	uint8 buff[6];

	_i2c.readFrom(GYRO_REG_ADDR, 6, buff);

	int16 result[3];
	result[0] = ((((int16) buff[1]) << 8) | buff[0]) ;
	result[1] = ((((int16) buff[3]) << 8) | buff[2]) ;
	result[2] = ((((int16) buff[5]) << 8) | buff[4]) ;

	Vect3D cGyro = Vect3D::zero();
	cGyro.setX((result[0] - _offset.getX()) * GYRO_LSB_TO_DEGS);
	cGyro.setY((result[1] - _offset.getY()) * GYRO_LSB_TO_DEGS);
	cGyro.setZ((result[2] - _offset.getZ()) * GYRO_LSB_TO_DEGS);

	_gyroRaw = cGyro ;
	_gyroFiltered = cGyro ;
}
