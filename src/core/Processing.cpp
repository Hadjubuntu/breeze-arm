/*
 * Processing.cpp
 *
 *  Created on: Sep 21, 2015
 *      Author: adrien
 */

#include "Processing.h"
#include "../math/time/Date.h"

Processing::Processing() : _freqHz(50), _lastExecutionDate(Date::zero()) {
}


void Processing::process()
{

	// Update last execution date
	_lastExecutionDate = Date::now();
}


bool Processing::isReady() {
	// Retrieve now date
	Date now = Date::now();

	// Compute duration from last execution
	float durationLastExecutionSeconds = now.durationFrom(_lastExecutionDate);

	// Compute delta time between two execution of the processing
	float dtExecExpected = 1.0/_freqHz;

	// Returns yes if processing needs to be executed
	return durationLastExecutionSeconds >= dtExecExpected;
}
