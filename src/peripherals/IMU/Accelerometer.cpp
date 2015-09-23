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
	// Calculate offset
//	float accumulator[] = {0,0,0};
//	int num_samples = 30;
//	for(int i = 0 ; i < num_samples ; i++) {
//		short acc[3];
//		getAccelerometerData(acc);
//		accumulator[0] += acc[0];
//		accumulator[1] += acc[1];
//		accumulator[2] += acc[2];
//
//		Serial3.println("New sample of acc data received..");
//		delay(100);
//	}
//	for(int i = 0 ; i < 3 ; i++) accumulator[i] /= num_samples;
//	accumulator[2] -= 248; // 1g at 2mg/LSB more or less.
//	_offset = accumulator;
//	for(int i = 0 ; i < 3 ; i++) Serial3.println(accumulator[i]);
}
