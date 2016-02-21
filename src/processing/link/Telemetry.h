/*
 * Telemetry.h
 *
 *  Created on: Nov 8, 2015
 *      Author: adrien-i3
 */

#ifndef PROCESSING_LINK_TELEMETRY_H_
#define PROCESSING_LINK_TELEMETRY_H_

#include "../../core/Processing.h"
#include "../ahrs/AHRS.h"
#include "../flightstabilization/FlightControl.h"

class Telemetry : public Processing {
private:
	AHRS *_ahrs;
	FlightControl *_flightControl;
public:
	Telemetry(AHRS*, FlightControl*);

	void init();
	void process();
	void callback() { };
};

#endif /* PROCESSING_LINK_TELEMETRY_H_ */
