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
	// Sample at 500Hz (1kHz / (div + 1))
	_i2c.writeTo(GYRO_SMPLRT_DIV, 0x01);
	delay(5);
	// Gyro +/- 2000 deg/s, low-pass filter at 10Hz
	_i2c.writeTo(GYRO_DLPF_FS, 0x1d);
	delay(5);
	//
	_i2c.writeTo(GYRO_INT_CFG, 0x00);

	delay(500);

	int num_samples = 200;
	float accumulator[] = {0.0, 0.0, 0.0};

	for(int i = 0 ; i < num_samples ; i++)
	{
		update();

		accumulator[0] += _gyroRaw.getX();
		accumulator[1] += _gyroRaw.getY();
		accumulator[2] += _gyroRaw.getZ();

		delay(10);
	}

	for(int i = 0 ; i < 3 ; i++)
	{
		accumulator[i] /= num_samples;
	}

	_offset = accumulator;
}

void Gyro::update()
{
	// Read raw data
	uint8 buff[6];

	_i2c.readFrom(GYRO_REG_ADDR, 6, buff);

	int16 result[3];
	result[0] = ((((int16) buff[0]) << 8) | buff[1]) ;
	result[1] = ((((int16) buff[2]) << 8) | buff[3]) ;
	result[2] = ((((int16) buff[4]) << 8) | buff[5]) ;

	// Create gyro vector from raw data
	Vect3D cGyro = Vect3D::fromInt16Array(result);

	// Scale gyro from LSB to deg/s
	cGyro *= GYRO_LSB_TO_DEGS;

	// Retrieve offset
	cGyro -= _offset;

	// Store data and filter gyro output
	_gyroRaw = cGyro ;

	// Filter gyro data
	_gyroFiltered = cGyro;
//	_gyroFiltered = (_gyroFiltered * (1.0 - 0.5) + cGyro * 0.5) ;
}
