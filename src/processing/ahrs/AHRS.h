/*
 * AHRS.h
 *
 * Attitude and heading reference system
 *
 *  Created on: Sep 22, 2015
 *      Author: adrien
 */

#ifndef PROCESSING_AHRS_AHRS_H_
#define PROCESSING_AHRS_AHRS_H_


#include "../../core/Processing.h"
#include "../../math/vector/Quaternion.h"
#include "../../peripherals/IMU/Accelerometer.h"
#include "../../peripherals/IMU/Gyro.h"



class AHRS : public Processing {
private:

	/** Rotation gravity body frame */
	Vect3D _grot;

	/** Quaternion of attitude */
	Quaternion _attitude;

	/** Gyroscope */
	Gyro _gyro;

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

	float* getGyroCorr();

	/******************************************************
	 * GETTERS
	 *****************************************************/
	Accelerometer getAcc() { return _accelerometer; }
	Gyro getGyro() { return _gyro; }

	Quaternion getAttitude() { return _attitude; }
};

#endif /* PROCESSING_AHRS_AHRS_H_ */
