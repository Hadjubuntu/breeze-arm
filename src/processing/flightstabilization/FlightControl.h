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
public:
	FlightControl(RadioControler*, FlightStabilization*, AHRS*);

	void init();
	void process();

	float radioToRad(float, float);
};

#endif /* PROCESSING_FLIGHTSTABILIZATION_FLIGHTCONTROL_H_ */
