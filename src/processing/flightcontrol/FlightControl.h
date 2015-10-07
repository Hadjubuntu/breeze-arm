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

class FlightControl : public Processing {
private:
	float _Pq;
	float _Pw;
	Quaternion _targetAttitude;
	Quaternion _currentAttitude;
	Vect3D _gyroRot;
	Vect3D _tau;
public:
	/**
	 * Constructor
	 */
	FlightControl();


	/** Set input parameters */
	void setInputs(Quaternion pTargetAttitude, Quaternion pCurrentAttitude, Vect3D pGyroRot);

	/**
	 * Process and update data
	 */
	void process();

	/* ****************************
	 * GETTERS
	 *************************** */
	Vect3D getTau() {
		return _tau;
	}


};

#endif /* PROCESSING_FLIGHTCONTROL_FLIGHTCONTROL_H_ */
