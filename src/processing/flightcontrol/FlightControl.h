/*
 * FlightControl.h
 *
 *  Created on: Oct 4, 2015
 *      Author: adrien
 */

#ifndef PROCESSING_FLIGHTCONTROL_FLIGHTCONTROL_H_
#define PROCESSING_FLIGHTCONTROL_FLIGHTCONTROL_H_

#include "../../math/vector/Quaternion.h"
#include "../../core/Processing.h"

class FlightControl : Processing {
private:
	Quaternion _targetAttitude;
	Quaternion _currentAttitude;
public:
	/**
	 * Constructor
	 */
	FlightControl();


	/** Set input parameters */
	void setInputs(Quaternion pTargetAttitude, Quaternion pCurrentAttitude);

	/**
	 * Process and update data
	 */
	void process();


};

#endif /* PROCESSING_FLIGHTCONTROL_FLIGHTCONTROL_H_ */
