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
	int _motorActivation[4][3] = {
			{1, 1, -1},
			{-1, 1, 1},
			{1, -1, 1},
			{-1, -1, -1}
	};

	void initMotorRepartition();

public:
	// debug
	int motors[4];
	ActuatorControl(FlightStabilization *);

	void init();

	void process();

	void processFixedWing(unsigned short int);

	void processMulticopter(unsigned short int, int nbMotors);

	int getCommandNmToSignalUs(float, float);
};

#endif /* PROCESSING_ACTUATOR_ACTUATORCONTROL_H_ */
