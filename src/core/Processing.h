/*
 * Processing.h
 *
 *  Created on: Sep 21, 2015
 *      Author: adrien
 */

#ifndef CORE_PROCESSING_H_
#define CORE_PROCESSING_H_

#include "../math/time/Date.h"

/**
 * Abstract class of a processing
 */
class Processing {
private:
	/** Frequency of process function calling (Hertz) */
	int _freqHz;
	/** Date last execution */
	Date _lastExecutionDate;
public:
	Processing();

	/**
	 * Execute the processsing
	 */
	void process();

	/**
	 * Tells whether the processing needs to be called again or not
	 */
	bool isReady();
};

#endif /* CORE_PROCESSING_H_ */
