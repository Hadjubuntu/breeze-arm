/*
 * Baro.cpp
 *
 *  Created on: Oct 11, 2015
 *      Author: adrien
 */
#include "../../peripherals/HAL/HAL.h"
#include "Baro.h"


Baro::Baro() : _i2c(I2C::getInstance(BMP085_ADDRESS))
{
	_freqHz = 50;

	_dev_address = BMP085_ADDRESS;
	_pressure_samples = 1;
	_retry_time = 0;
	Temp = 0.0;
	RawTemp = 0;
	RawPress = 0;
	ac1 = 0; ac2= 0; ac3 = 0; ac4 = 0; ac5 = 0;
	ac6 = 0; b1 = 0; b2 = 0; mb = 0; _temp_sum = 0; Press = 0; mc = 0; _press_sum = 0; GroundPressure = 0; GroundTemp = 0;
	_last_update = 0; BMP085_State = 0; md = 0;
	_last_temp_read_command_time = 0;
	_last_press_read_command_time = 0;
}

void Baro::process()
{
//	accumulate();
//	read();
}

float Baro::getAltitudeMeters() {
	float alt = 0.0;
//	if (Press < GroundPressure) {
//		float scaleHeightEarth = 7000.0;
//		alt = - scaleHeightEarth * log(Press/GroundPressure);
//	}
	return alt;
}

bool Baro::BMP_DATA_READY() {
	long t_us = DateUtils::micros();
	int dtMin = 0;
	if (BMP085_State == 0) {
		dtMin = 5;
	}
	else {
		dtMin = 26;
	}
	if (t_us-(long)_last_temp_read_command_time > dtMin) {
		return true;
	}
	else {
		return false;
	}
}





// Public Methods //////////////////////////////////////////////////////////////
void Baro::init()
{
	uint8 buff[22];

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

	_last_press_read_command_time = 0;
	_last_temp_read_command_time = 0;

	//Send a command to read Temp
	Command_ReadTemp();

	BMP085_State = 0;

	// init raw temo
	RawTemp = 0;

	GroundPressure = 0;
	GroundTemp = 0;
//	Calibrate();
}

void Baro::Calibrate() {
	int step = 0;

	while (step < 100) {
		accumulate();
		read();

		HAL::delayMs(20);
		step ++;
	}

	step = 0;
	while (step < 200) {
		accumulate();
		read();
		if (GroundPressure == 0) {
			GroundPressure = getPressure();
		}
		else {
			GroundPressure = 0.8*GroundPressure + 0.2*getPressure();
		}

		if (GroundTemp == 0.0) {
			GroundTemp = getTemperature();
		}
		else {
			GroundTemp = 0.8*GroundTemp + 0.2*getTemperature();
		}

		HAL::delayMs(20);
		step ++;
	}
}

// Read the sensor. This is a state machine
// acumulate a new sensor reading
void Baro::accumulate(void)
{
	if (!BMP_DATA_READY()) {
		return;
	}


	if (BMP085_State == 0) {
		ReadTemp();
	} else {
		ReadPress();
		Calculate();
	}
	BMP085_State++;
	if (BMP085_State == 5) {
		BMP085_State = 0;
		Command_ReadTemp();
	} else {
		Command_ReadPress();
	}

}

// return altitude difference in meters between current pressure and a
// given base_pressure in Pascal
float Baro::get_altitude_difference(float base_pressure, float pressure) const
{
	float ret;

	// on faster CPUs use a more exact calculation
	float scaling = pressure / base_pressure;
	float temp    = GroundTemp + 273.15f; // TODO _ground_temperature

	// This is an exact calculation that is within +-2.5m of the standard atmosphere tables
	// in the troposphere (up to 11,000 m amsl).
	ret = 153.8462f * temp * (1.0f - expf(0.190259f * logf(scaling)));

	return ret;
}


// Read the sensor using accumulated data
uint8 Baro::read()
{
	if (_count == 0 && BMP_DATA_READY()) {
		accumulate();
	}
	if (_count == 0) {
		return 0;
	}
	_last_update = DateUtils::micros();

	Temp = 0.1f * _temp_sum / _count;
	Press = _press_sum / _count;

	_pressure_samples = _count;
	_count = 0;
	_temp_sum = 0;
	_press_sum = 0;

	return 1;
}

float Baro::getPressure() {
	return Press;
}

float Baro::get_ground_pressure() {
	return GroundPressure;
}

float Baro::getTemperature() const {
	return Temp;
}

// Private functions: /////////////////////////////////////////////////////////

// Send command to Read Pressure
void Baro::Command_ReadPress()
{
	//	 Mode 0x34+(OVERSAMPLING << 6) is osrs=3 when OVERSAMPLING=3 => 25.5ms conversion time
	_i2c.writeTo(0xF4, 0x34+(OVERSAMPLING << 6));
	_last_press_read_command_time = DateUtils::micros();
}

// Read Raw Pressure values
void Baro::ReadPress()
{
	uint8 buf[3];

	if (DateUtils::micros() < _retry_time) {
		return;
	}

	_i2c.readFrom(0xF6, 3, buf);
	_retry_time = DateUtils::micros() + 1000000;


	RawPress = (((uint32_t)buf[0] << 16)
			| ((uint32_t)buf[1] << 8)
			| ((uint32_t)buf[2])) >> (8 - OVERSAMPLING);
}

// Send Command to Read Temperature
void Baro::Command_ReadTemp()
{
	_i2c.writeTo(0xF4, 0x2E);
	_last_temp_read_command_time = DateUtils::micros();
}

// Read Raw Temperature values
void Baro::ReadTemp()
{
	uint8 buf[2];
	int32 _temp_sensor;

	if (DateUtils::micros() < _retry_time) {
		return;
	}

	_i2c.readFrom(0xF6, 2, buf);
	_retry_time = DateUtils::micros() + 1000;


	_temp_sensor = buf[0];
	_temp_sensor = (_temp_sensor << 8) | buf[1];

	RawTemp = 0.8*RawTemp + 0.2*_temp_sensor;
}


// Calculate Temperature and Pressure in real units.
void Baro::Calculate()
{
	int32 x1, x2, x3, b3, b5, b6, p;
	uint32 b4, b7;
	int32 tmp;

	// See Datasheet page 13 for this formulas
	// Based also on Jee Labs BMP085 example code. Thanks for share.
	// Temperature calculations
	x1 = ((int32_t)RawTemp - ac6) * ac5 >> 15;
	x2 = ((int32_t) mc << 11) / (x1 + md);
	b5 = x1 + x2;
	_temp_sum += (b5 + 8) >> 4;

	// Pressure calculations
	b6 = b5 - 4000;
	x1 = (b2 * (b6 * b6 >> 12)) >> 11;
	x2 = ac2 * b6 >> 11;
	x3 = x1 + x2;
	//b3 = (((int32_t) ac1 * 4 + x3)<<OVERSAMPLING + 2) >> 2; // BAD
	//b3 = ((int32_t) ac1 * 4 + x3 + 2) >> 2;  //OK for OVERSAMPLING=0
	tmp = ac1;
	tmp = (tmp*4 + x3)<<OVERSAMPLING;
	b3 = (tmp+2)/4;
	x1 = ac3 * b6 >> 13;
	x2 = (b1 * (b6 * b6 >> 12)) >> 16;
	x3 = ((x1 + x2) + 2) >> 2;
	b4 = (ac4 * (uint32_t) (x3 + 32768)) >> 15;
	b7 = ((uint32_t) RawPress - b3) * (50000 >> OVERSAMPLING);
	p = b7 < 0x80000000 ? (b7 * 2) / b4 : (b7 / b4) * 2;

	x1 = (p >> 8) * (p >> 8);
	x1 = (x1 * 3038) >> 16;
	x2 = (-7357 * p) >> 16;
	_press_sum += p + ((x1 + x2 + 3791) >> 4);

	_count++;
	if (_count == 254) {
		_temp_sum *= 0.5f;
		_press_sum *= 0.5f;
		_count /= 2;
	}
}
