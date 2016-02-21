/*
 * Baro.cpp

 *
 *  Created on: Oct 11, 2015
 *      Author: adrien
 */
#include <stdio.h>
#include "../../peripherals/HAL/HAL.h"
#include "Baro.h"

Baro::Baro() : Processing(), _i2c(I2C::getInstance(BMP085_ADDRESS))
{
	_freqHz = 10;

	_dev_address = BMP085_ADDRESS;
	_pressure_samples = 1;
	_retry_time = 0;
	_trueTemperature = 0.0;
	_uncompensatedTemperature = 0;
	_uncompensatedPressure = 0;
	ac1 = 0; ac2= 0; ac3 = 0; ac4 = 0; ac5 = 0;
	ac6 = 0; b1 = 0; b2 = 0; mb = 0;
	_truePressure = 0;
	mc = 0;
	GroundPressure = 23843;
	GroundTemp = 0;
	_last_update = 0; md = 0;
	_count = 0;
	_state = 0;
	b5 = 0;
	_iter = 0;
	_altitudeMeters = 0.0;
}

void Baro::init()
{
	uint8_t buff[22];


	// We read the calibration data registers
	_i2c.readFrom(0xAA, 22, buff) ;

	ac1 = ((int16_t)buff[0] << 8) | buff[1];
	ac2 = ((int16_t)buff[2] << 8) | buff[3];
	ac3 = ((int16_t)buff[4] << 8) | buff[5];
	ac4 = ((int16_t)buff[6] << 8) | buff[7];
	ac5 = ((int16_t)buff[8] << 8) | buff[9];
	ac6 = ((int16_t)buff[10] << 8) | buff[11];
	b1 = ((int16_t)buff[12] << 8) | buff[13];
	b2 = ((int16_t)buff[14] << 8) | buff[15];
	mb = ((int16_t)buff[16] << 8) | buff[17];
	mc = ((int16_t)buff[18] << 8) | buff[19];
	md = ((int16_t)buff[20] << 8) | buff[21];

	char str[80];
	sprintf(str, "LOG|%d|%d|%d|%d|%d|%d|%d|%d", ac1, ac2, ac3, ac4, ac5, ac6, b1, b2);
	Serial3.println(str);
}

void Baro::process()
{
	if (_state == 0)
	{
		_state = 1;
		readUncompensatedTempValue();
	}

	// Force to check state again since it has changed above
	if (_state == 3)
	{
		_state = 5;
		readUncompensatedPressureValue();
	}

	// Again
	if (_state == 6)
	{
		calculateTrueTemperature();

		calculateTruePressure();

		calculateAltitude();

		_state = 0;

		if (_iter < 100) {
			_iter ++;
		}
	}
}

void Baro::callback()
{
	if (_state == 1)
	{
		uint8 datas[2];
		_i2c.readFrom(0xF6, 2, datas);
		_uncompensatedTemperature = (datas[0] << 8) + datas[1];
		_state = 3;

	}
	else if (_state == 5)
	{
		uint8 datas[3];
		_i2c.readFrom(0xF6, 3, datas);
		_uncompensatedPressure = (datas[0] << 16) + (datas[1] << 8) + (datas[0] > (8 - OVERSAMPLING));
		_state = 6;
	}
}

void Baro::readUncompensatedTempValue()
{
	_i2c.writeTo(0xF4, 0x2E);

	// Wait for 4.5ms
	planCallback(DateUtils::secondsToMicros(4.5/1000.0));
}


void Baro::readUncompensatedPressureValue()
{
	_i2c.writeTo(0xF4, 0x34 + (OVERSAMPLING << 6));
	// Wait for 4.5ms
	planCallback(DateUtils::secondsToMicros(26.0/1000.0));
}

void Baro::calculateTrueTemperature()
{
	long x1 = (_uncompensatedTemperature - ac6) * ac5 >> 15;
	long x2 = ((long) mc << 11) / (x1 + md);
	b5 = x1 + x2;
	_trueTemperature = (b5 + 8) >> 4;
}

void Baro::calculateTruePressure()
{
	long b6, b3,  x1, x2, x3;
	long tmpPressure;
	unsigned long b4, b7;

	b6 = b5 - 4000;
	x1 = (b2 * (b6 * b6 >> 12)) >> 11;
	x2 = ac2 * b6 >> 11;
	x3 = x1 + x2;

	b3 = (((ac1*4 + x3)<<OVERSAMPLING) + 2) / 4;
	x1 = ac3 * b6 >> 13;
	x2 = (b1 * (b6 * b6 >> 12)) >> 16;
	x3 = ((x1 + x2) + 2) >> 2;

	b4 = (ac4 * (unsigned long) (x3 + 32768)) >> 15;
	b7 = ((unsigned long) _uncompensatedPressure - b3) * (50000 >> OVERSAMPLING);
	tmpPressure = b7 < 0x80000000 ? (b7 * 2) / b4 : (b7 / b4) * 2;

	x1 = (tmpPressure >> 8) * (tmpPressure >> 8);
	x1 = (x1 * 3038) >> 16;
	x2 = (-7357 * tmpPressure) >> 16;
	_truePressure = tmpPressure + ((x1 + x2 + 3791) >> 4);
}

void Baro::calculateAltitude()
{
	// Calibration value
	if (_iter >= 10  && _iter <= 30)
	{
		GroundPressure = GroundPressure * 0.2 + 0.8 * _truePressure;
		GroundTemp = GroundTemp * 0.2 + 0.8 * _trueTemperature;
	}

	// Calculate
	float diffPressure = _truePressure / GroundPressure;
	_altitudeMeters = 44330 * (1 - pow(diffPressure, 1/5.255));
}
