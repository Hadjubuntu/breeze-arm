/*
 * Brain.h
 *------------------------------------------------------
 *  Created on: Sep 15, 2015
 *      Author: adrien
 */

#ifndef CORE_BRAIN_H_
#define CORE_BRAIN_H_

#include "History.h"
#include "../math/time/Date.h"
#include "Processing.h"


/**
 * The brain manage time by calling processing needed to be executed on the schedule
 */
class Brain {
private:
	// delay between tick in microseconds
	const static int _delayTickUs = 100;

	// Tick id counter
	long _tickId;

	// Reference date
	Date _referenceDate;

	// Processing needed to be executed
	std::vector<Processing> _processings;

	// History of link command
	History<int> _commands;
public:
	/**
	 * Initialize brain
	 */
	Brain();

	/** Add a new processing on the scheduler */
	void addProcessing(Processing& e) { _processings.push_back(e); }


	/** Loop */
	void loop();


	// Getters
	//--------------------
	long getTickId() const { return _tickId; }
};

#endif /* CORE_BRAIN_H_ */
