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

	/** Callback is called */
	bool _callbackTrigger;
	Date _callbackStartDate;
	long _callbackDtUs;

public:
	Processing();

	/** Initialize processing */
	virtual void init() = 0;

	/** Execute processing */
	virtual void process() = 0;

	virtual void callback() = 0;

	/**
	 * Update execution date
	 * This function is called by UAV brain
	 */
	void updateExecDate()
	{
		// Get now date
		Date now = Date::now();

		// Store dt
		_dt = now.durationFrom(_lastExecutionDate);

		// Update last execution date
		_lastExecutionDate = now;
	}

	/** Wait for dt seconds */
	void wait(float dtWait);

	/**
	 * Tells whether the processing needs to be called again or not
	 */
	bool isReady();
	bool isCallbackReady();

	bool isTriggeringCallback() {
		return _callbackTrigger;
	}
	void closeCallback() {
		_callbackTrigger = false;
		_callbackDtUs = 0;
	}
	void planCallback(long pDtUs) {
		_callbackDtUs = pDtUs;
		_callbackTrigger = true;
		_callbackStartDate = Date::now();
	}
};

#endif /* CORE_PROCESSING_H_ */
