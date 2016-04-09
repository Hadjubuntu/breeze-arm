/*
 * FsAutotune.h
 *
 *  Created on: Apr 9, 2016
 *      Author: adrien
 */

#ifndef PROCESSING_FLIGHTSTABILIZATION_FSAUTOTUNE_H_
#define PROCESSING_FLIGHTSTABILIZATION_FSAUTOTUNE_H_


#include "../../core/Processing.h"
#include "../../math/pid/PID.h"
#include <list>

class FsAutotune : public Processing {
protected:
	std::list<PID> pidList;
public:
	FsAutotune();

	/**
	 * Process and update data
	 */
	void process();
	void callback() {};

	void autotune(PID);

	void clearPidList() {
		pidList.clear();
	}

	virtual ~FsAutotune();
};

#endif /* PROCESSING_FLIGHTSTABILIZATION_FSAUTOTUNE_H_ */
