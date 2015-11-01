/*
 * ActuatorControl.h
 *
 *  Created on: Oct 23, 2015
 *      Author: adrien
 */

#ifndef PROCESSING_ACTUATOR_ACTUATORCONTROL_H_
#define PROCESSING_ACTUATOR_ACTUATORCONTROL_H_


#include "../flightstabilization/FlightStabilization.h"

class ActuatorControl : public Processing {
private:
	FlightStabilization *_flightStabilization;

public:
	ActuatorControl(FlightStabilization *);

	void init();

	void process();

	void processFixedWing(unsigned short int);

	void processMulticopter(unsigned short int);

	int getCommandNmToSignalUs(float, float);

	int motorMap[4];
};

#endif /* PROCESSING_ACTUATOR_ACTUATORCONTROL_H_ */
