/*
 * Baro.h
 *
 *  Created on: Oct 11, 2015
 *      Author: adrien
 */

#ifndef PERIPHERALS_IMU_BARO_H_
#define PERIPHERALS_IMU_BARO_H_

#include "../../peripherals/I2C/I2C.h"
#include "../../core/Processing.h"

#define BMP085_ADDRESS 0x77  //(0xEE >> 1)
#define BMP085_EOC 30        // End of conversion pin PC7 on APM1


// No EOC connection from Baro
// Use times instead.
// Temp conversion time is 4.5ms
// Pressure conversion time is 25.5ms (for OVERSAMPLING=3)


// oversampling 3 gives 26ms conversion time. We then average
#define OVERSAMPLING 3

class Baro : public Processing {
private:
	I2C _i2c;
	uint8 _dev_address;

	int32        RawPress;
	int32        RawTemp;
	float		    _temp_sum;
	float			_press_sum;
	uint8_t			_count;
	float           Temp;
	float           Press;
	// Flymaple has no EOC pin, so use times instead
	uint32       _last_press_read_command_time;
	uint32       _last_temp_read_command_time;


	uint32                           _last_update; // in us
	uint8                            _pressure_samples;
	// State machine
	uint8                        BMP085_State;
	// Internal calibration registers
	int16                        ac1, ac2, ac3, b1, b2, mb, mc, md;
	uint16                       ac4, ac5, ac6;


	uint32                       _retry_time;

	void                            Command_ReadPress();
	void                            Command_ReadTemp();
	void                            ReadPress();
	void                            ReadTemp();
	void                            Calculate();
	void							Calibrate();
	bool BMP_DATA_READY();

public:

	float 			GroundPressure;
	float 			GroundTemp;

	Baro();

	void init();
	void process();

	uint8        read();
	void 			accumulate(void);
	float           getPressure();
	float			get_ground_pressure();
	long 			get_alt_cm();
	float           getTemperature() const;

	float get_altitude_difference(float, float) const;

	static Baro create() {
		Baro e;
		return e;
	}

};




#endif /* PERIPHERALS_IMU_BARO_H_ */
