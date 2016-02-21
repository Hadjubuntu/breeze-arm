/*
 * Brain.cpp

 *
 *  Created on: Sep 15, 2015
 *      Author: adrien
 */
#include <stdio.h>
#include <wirish/wirish.h>
#include "../core/Brain.h"
#include "../peripherals/HAL/HAL.h"
#include "libmaple/i2c.h"


// SEE
//https://github.com/opendrone/flymaple-cpp/blob/master/libmaple/examples/test-timers.cpp


Brain::Brain() : _referenceDate(Date::zero()), _commands(History<int>::getDefault()) {
	_tickId = 0;
	_referenceDate.setTimElapsedSinceStartUs(micros());
}

void Brain::enableI2C()
{
	// Initialize I2C com
	i2c_master_enable(I2C1, I2C_FAST_MODE);
}

void Brain::initProcessings()
{
	for (Processing *proc : _processings)
	{
		proc->init();
	}
}

void Brain::loop()
{
	// For each processing
	for (Processing *proc : _processings)
	{
		// Normal processing mode - check processing freq last call
		// ----
		if (proc->isReady() && !proc->isTriggeringCallback())
		{
			// Update execution date
			proc->updateExecDate();

			// Then execute the processing
			proc->process();
		}

		// Callback triggering processing mode
		// ---
		if (proc->isTriggeringCallback() && proc->isCallbackReady())
		{
			proc->callback();
			proc->closeCallback();
		}
	}


	_tickId ++;
	HAL::delayUs(_delayTickUs);
}

Processing* Brain::getProcByName(std::string pName)
{
//	Processing *output;
//	std::vector<Processing>::iterator itrProc = _processings.begin();
//
//	while (itrProc != _processings.end() && output == NULL)
//	{
//		Processing current = *itrProc;
//		if (current.getName())
//		{
//
//		}
//		itrProc ++;
//	}
	return 0;
}

