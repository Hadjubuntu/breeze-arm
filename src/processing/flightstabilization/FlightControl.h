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
public:
	FlightControl(RadioControler*, FlightStabilization*, AHRS*);

	void init();
	void process();

	float radioToRad(float, float);

	float rpy[2];

	float getYawInt() {
		return _yawInt;
	}
};

#endif /* PROCESSING_FLIGHTSTABILIZATION_FLIGHTCONTROL_H_ */
