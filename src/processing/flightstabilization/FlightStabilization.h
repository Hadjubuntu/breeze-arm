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

class FlightStabilization : public Processing {
private:
	// Inputs
	// ------------------
	float _Pq;
	float _Pw;
	Quaternion _targetAttitude;
	Quaternion _currentAttitude;
	Vect3D _gyroRot;
	float _throttle;
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
	void setInputs(Quaternion pTargetAttitude, Quaternion pCurrentAttitude, Vect3D pGyroRot, float pThrottle);

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
