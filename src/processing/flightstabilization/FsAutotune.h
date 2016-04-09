/*
 * FsAutotune.h
 *
 *  Created on: Apr 9, 2016
 *      Author: adrien
 */

#ifndef PROCESSING_FLIGHTSTABILIZATION_FSAUTOTUNE_H_
#define PROCESSING_FLIGHTSTABILIZATION_FSAUTOTUNE_H_


#include "../../core/Processing.h"
#include "FsAutotuneStoreMeasure.h"
#include <list>

class FsAutotune : public Processing {
protected:
	std::list<FsAutotuneStoreMeasure> storeMeasureList;

public:
	FsAutotune();

	/**
	 * Process and update data
	 */
	void process();
	void callback() {};

	void updateMeasure(PID);
	void autotune(PID);

	void clear() {
		storeMeasureList.clear();
	}

	virtual ~FsAutotune();
};

#endif /* PROCESSING_FLIGHTSTABILIZATION_FSAUTOTUNE_H_ */
