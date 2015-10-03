/*
 * Brain.cpp

 *
 *  Created on: Sep 15, 2015
 *      Author: adrien
 */
#include <stdio.h>
#include <wirish/wirish.h>
#include "../core/Brain.h"


// SEE
//https://github.com/opendrone/flymaple-cpp/blob/master/libmaple/examples/test-timers.cpp


Brain::Brain() : _referenceDate(Date::zero()), _commands(History<int>::getDefault()) {
	_tickId = 0;
	_referenceDate.setTimElapsedSinceStartUs(micros());
}



void Brain::loop()
{
	// For each processing
	for (Processing *proc : _processings)
	{
		// Check if the processing needs to be executed
		if (proc->isReady())
		{
			// Update execution date
			proc->updateExecDate();

			// Then execute the processing
			proc->process();
		}
	}


	_tickId ++;
	delayMicroseconds(_delayTickUs);
}

