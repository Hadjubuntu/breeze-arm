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

class Telemetry : public Processing {
private:
	AHRS *_ahrs;
public:
	Telemetry(AHRS*);

	void init();
	void process();
};

#endif /* PROCESSING_LINK_TELEMETRY_H_ */
