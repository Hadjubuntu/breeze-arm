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
#include "../../math/pid/PID.h"
#include "../ahrs/AHRS.h"
#include "../flightstabilization/FlightControl.h"

class FlightStabilization : public Processing {
private:
	// Dependencies
	// ---
	AHRS *_ahrs;
	FlightControl *_flightControl;

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

	// PID
	PID _pidRoll;
	PID _pidPitch;
	PID _pidAltitude;
public:
	/**
	 * Constructor
	 */
	FlightStabilization(AHRS*, FlightControl*);

	/**
	 * TODO
	 */
	void init() {

	}

	void updateInputParameters();

	/**
	 * Process and update data
	 */
	void process();
	void callback() {};

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
