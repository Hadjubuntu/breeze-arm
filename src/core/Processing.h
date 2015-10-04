/*
 * Processing.h
 *
 *  Created on: Sep 21, 2015
 *      Author: adrien
 */

#ifndef CORE_PROCESSING_H_
#define CORE_PROCESSING_H_

#include "../core/Logger.h"
#include "../math/time/Date.h"

/**
 * Abstract class of a processing
 */
class Processing {
protected:
	/** Internal logger */
	Logger _logger;

	/** Frequency of process function calling (Hertz) */
	int _freqHz;

	/** Date last execution */
	Date _lastExecutionDate;

	/** Last dt (Seconds) */
	float _dt;
public:
	Processing();

	virtual void process() = 0;

	void updateExecDate()
	{
		// Get now date
		Date now = Date::now();

		// Store dt
		_dt = now.durationFrom(_lastExecutionDate);

		// Update last execution date
		_lastExecutionDate = now;
	}


	/**
	 * Tells whether the processing needs to be called again or not
	 */
	bool isReady();
};

#endif /* CORE_PROCESSING_H_ */
