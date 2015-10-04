/*
 * FlightControl.h
 *
 *  Created on: Oct 4, 2015
 *      Author: adrien
 */

#ifndef PROCESSING_FLIGHTCONTROL_FLIGHTCONTROL_H_
#define PROCESSING_FLIGHTCONTROL_FLIGHTCONTROL_H_

class FlightControl {
private:

public:
	FlightControl();

	void init();

	void stabilize(Quaternion pTargetAttitude, Quaternion pCurrentAttitude);
};

#endif /* PROCESSING_FLIGHTCONTROL_FLIGHTCONTROL_H_ */
