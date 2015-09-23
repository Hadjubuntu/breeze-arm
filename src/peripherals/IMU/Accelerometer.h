/*
 * Accelerometer.h
 *
 *  Created on: Sep 22, 2015
 *      Author: adrien
 */

#ifndef PERIPHERALS_IMU_ACCELEROMETER_H_
#define PERIPHERALS_IMU_ACCELEROMETER_H_

#include "../../math/vector/Vect3D.h"
#include "../../peripherals/I2C/I2C.h"

////////Acceleration sensor ADXL345 function/////////////////////////////
#define ACC (0x53)    //Defined ADXL345 address, ALT ADDRESS pin is grounded
#define A_TO_READ (6) //the number of bytes to read(each axis accounted for two-byte)
#define XL345_DEVID   0xE5 //ADXL345 ID register，需要注意芯片有一个地址选择线将AD0连接到GND口
// ADXL345 Control register
#define ADXLREG_TAP_AXES     0x2A
#define ADXLREG_BW_RATE      0x2C
#define ADXLREG_POWER_CTL    0x2D
#define ADXLREG_INT_ENABLE   0x2E
#define ADXLREG_DATA_FORMAT  0x31
#define ADXLREG_FIFO_CTL     0x38
#define ADXLREG_DUR          0x21

//ADXL345 Data register
#define ADXLREG_DEVID        0x00
#define ADXLREG_DATAX0       0x32
#define ADXLREG_DATAX1       0x33
#define ADXLREG_DATAY0       0x34
#define ADXLREG_DATAY1       0x35
#define ADXLREG_DATAZ0       0x36
#define ADXLREG_DATAZ1       0x37


class Accelerometer
{
	/** I2C communication */
	I2C _i2c;
	/** Filtered acceleration */
	Vect3D _accFiltered;

	/** Raw data acceleration */
	Vect3D _accRaw;

	/** Initial offset acceleration */
	Vect3D _offset;
public:
	Accelerometer() :
		_i2c(I2C::getInstance(ACC)),
		_accFiltered(Vect3D::zero()),
		_accRaw(Vect3D::zero()),
		_offset(Vect3D::zero()) {

	}

	void init();

	void read();
};

#endif /* PERIPHERALS_IMU_ACCELEROMETER_H_ */
