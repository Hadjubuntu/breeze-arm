/*
 * FlightControl.h
 *
 *  Created on: Oct 4, 2015
 *      Author: adrien
 */

#ifndef PROCESSING_FLIGHTSTABILIZATION_FLIGHTSTABILIZATION_H_
#define PROCESSING_FLIGHTSTABILIZATION_FLIGHTSTABILIZATION_H_

#include "../../math/vector/Quaternion.h"
#include "../../core/Processing.h"
#include "../../data/conf/Param.h"

class FlightStabilization : public Processing {
private:
	// Inputs
	// ------------------
	Param<float> *_Pq;
	Param<float> *_Pw;
	Param<float> *_Kangle;
	Param<float> *_Krate;
	Quaternion _targetAttitude;
	Quaternion _currentAttitude;
	Vect3D _gyroRot;
	float _throttle;
	float _yawFromGyro;
	// Outputs
	// ------------------
	Vect3D _tau;
	float _throttleOut;
public:
	/**
	 * Constructor
	 */
	FlightStabilization();

	/**
	 * TODO
	 */
	void init() {

	}


	/** Set input parameters */
	void setInputs(Quaternion pTargetAttitude, Quaternion pCurrentAttitude, float yawCmd, Vect3D pGyroRot, float pThrottle);

	/**
	 * Process and update data
	 */
	void process();

	float boostThrottleCompensateTiltAngle(float throttle);
	bool isSafeToUseBoost(float throttle, float combinedTilt);

	/* ****************************
	 * GETTERS
	 *************************** */
	Vect3D getTau() {
		return _tau;
	}

	float getThrottle() {
		return _throttleOut;
	}

	Quaternion getTargetAttitude() {
		return _targetAttitude;
	}
};

#endif /* PROCESSING_FLIGHTSTABILIZATION_FLIGHTSTABILIZATION_H_ */
