/*
 * Accelerometer.cpp
 *
 *  Created on: Sep 22, 2015
 *      Author: adrien
 */
#include <wirish/wirish.h>
#include "Accelerometer.h"
#include "libmaple/i2c.h"


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
	_i2c.writeTo(ADXLREG_POWER_CTL,0x00);
	_i2c.writeTo(ADXLREG_POWER_CTL,0x08); //Set accelerometer to measure mode

	Serial3.println("Conf acc ok..");

	delay(100);

	//	Calculate offset
	float accumulator[] = {0,0,0};
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
	accumulator[2] -= 248; // 1g at 2mg/LSB more or less.

	// TODO FIXME
//	_offset.set(accumulator[0], accumulator[1], accumulator[2]);
}

void Accelerometer::update()
{
	int16 regAddress = ADXLREG_DATAX0;    //start reading byte
	uint8 buff[A_TO_READ];

	_i2c.readFrom(regAddress, A_TO_READ, buff);

	//Readings for each axis with 10-bit resolution, ie 2 bytes.
	//We want to convert two bytes into an int variable
	_accRaw.setX(((((int16) buff[1]) << 8) | buff[0]) - _offset[0]);
	_accRaw.setY(((((int16) buff[3]) << 8) | buff[2]) - _offset[1]);
	_accRaw.setZ(((((int16) buff[5]) << 8) | buff[4]) - _offset[2]);
}
