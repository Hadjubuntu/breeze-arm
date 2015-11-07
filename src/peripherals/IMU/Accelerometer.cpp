/*
 * Accelerometer.cpp
 *
 *  Created on: Sep 22, 2015
 *      Author: adrien
 */
#include <wirish/wirish.h>
#include <stdio.h>
#include "Accelerometer.h"

/**
 * Reference:
 * https://github.com/opendrone/flymaple/blob/testing/src/Accelerometer.cpp
 */
void Accelerometer::init()
{
	Serial3.println("Read from accelerometer");
	uint8 buff[1];
	_i2c.readFrom(0x00, 1, buff);

	// now we check msg_data for our 0xE5 magic number
	uint8 dev_id = buff[0];

	Serial3.println("Writing to accelerometer");

	if (dev_id != XL345_DEVID)
	{
		Serial3.println("Error, incorrect xl345 devid!");
		Serial3.println("Halting program, hit reset...");
		waitForButtonPress(0);
	}

	//invoke ADXL345
	_i2c.writeTo(ADXLREG_POWER_CTL,0x00);	delay(5);	//
	_i2c.writeTo(ADXLREG_POWER_CTL, 0xff);	delay(5);	//
	_i2c.writeTo(ADXLREG_POWER_CTL, 0x08); delay(5);	//
	_i2c.writeTo(ADXLREG_DATA_FORMAT, 0x08); delay(5);	//
	_i2c.writeTo(ADXLREG_BW_RATE, 0x09); delay(5);		//25Hz

	Serial3.println("Conf acc ok..");

	delay(100);

	//	Calculate offset
	float accumulator[] = {0.0, 0.0, 0.0};
	int num_samples = 30;

	for(int i = 0 ; i < num_samples ; i++)
	{
		// Update accelerometer
		//------
		update();

		// Accumulate data
		//-----
		accumulator[0] += _accRaw.getX();
		accumulator[1] += _accRaw.getY();
		accumulator[2] += _accRaw.getZ();

		Serial3.println("New sample of acc data received..");
		delay(100);
	}
	for(int i = 0 ; i < 3 ; i++)
	{
		accumulator[i] /= num_samples;
	}
	accumulator[2] = 0.0; // Force 0.0 offset on z axis

	// DEBUG 0 offset
//	_offset = accumulator;

	char str[60];
	sprintf(str, "Offset: {x:%f, y:%f, z:%f}", _offset.getX(), _offset.getY(), _offset.getZ());
	Serial3.println(str);
}

void Accelerometer::update()
{
	// Retrieve raw data from I2C
	uint8 buff[A_TO_READ];

	_i2c.readFrom(ADXLREG_DATAX0, A_TO_READ, buff);

	int16 result[3];
	result[1] = -((((int16) buff[1]) << 8) | buff[0]) ;
	result[0] = -((((int16) buff[3]) << 8) | buff[2]) ;
	result[2] = -((((int16) buff[5]) << 8) | buff[4]) ;

	// Create vector 3D from array of int16
	Vect3D cAcc = Vect3D::fromInt16Array(result);

	// Apply scale factor from LSB to g
	cAcc *= ACC_SENSITIVITY;

	// Retrieve offset
	cAcc -= _offset;

	_accRaw = cAcc;

//	_accFiltered = cAcc;
	_accFiltered = _accFiltered * (1.0 - _filterNewDataCoeff) + cAcc * _filterNewDataCoeff;

}
