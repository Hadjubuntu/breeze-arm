/*
 * AHRS.h
 *
 *  Created on: Sep 22, 2015
 *      Author: adrien
 */

#ifndef PROCESSING_AHRS_AHRS_H_
#define PROCESSING_AHRS_AHRS_H_

#include "../../core/Processing.h"
#include "../../math/vector/Quaternion.h"
#include "../../peripherals/IMU/Accelerometer.h"

class AHRS : public Processing {
private:
	/** Quaternion of attitude */
	Quaternion _attitude;

	/** Gyroscope */

	/** Accelerometer */
	Accelerometer _accelerometer;

public:
	AHRS();

	/**
	 * Initialize sensors accelerometer and gyro
	 */
	void initSensors();

	/**
	 * Process and update data
	 */
	void process();
};

#endif /* PROCESSING_AHRS_AHRS_H_ */
