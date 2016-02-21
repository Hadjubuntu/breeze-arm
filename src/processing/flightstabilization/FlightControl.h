/*
 * FlightControl.h
 *
 *  Created on: Oct 21, 2015
 *      Author: adrien
 */

#ifndef PROCESSING_FLIGHTSTABILIZATION_FLIGHTCONTROL_H_
#define PROCESSING_FLIGHTSTABILIZATION_FLIGHTCONTROL_H_

#include "../../core/Processing.h"
#include "FlightStabilization.h"
#include "../link/RadioControler.h"
#include "../ahrs/AHRS.h"
#include "../../data/conf/Param.h"

class FlightControl : public Processing {
private:
	RadioControler *_radioController;
	FlightStabilization *_flightStabilization;
	AHRS *_ahrs;
	float _throttleInitUs;
	float _yawInt;

	// Configuration parameters
	Param<float> *_maxAbsRollAngle;
	Param<float> *_maxAbsPitchAngle;
	Param<float> *_maxAbsCombinedAngle;

	// Angle desired (rad)
	float _rollDesired;
	float _pitchDesired;
public:
	FlightControl(RadioControler*, FlightStabilization*, AHRS*);

	void init();
	void process();
	void callback() { };

	float radioToRad(float, float);

	float rpy[2];

	float getYawInt() {
		return _yawInt;
	}

	float getRollDesired() {
		return _rollDesired;
	}
	float getPitchDesired() {
		return _pitchDesired;
	}
};

#endif /* PROCESSING_FLIGHTSTABILIZATION_FLIGHTCONTROL_H_ */
