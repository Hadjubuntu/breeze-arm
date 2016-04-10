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
#include "FsAutotuneStoreMeasure.h"
#include <vector>

class FsAutotune : public Processing {
protected:
	std::vector<FsAutotuneStoreMeasure*> storeMeasureList;

public:
	FsAutotune();

	void init()
	{

	}

	/**
	 * Process and update data
	 */
	void process();
	void callback() {};

	// Add a new PID to autotune
	void addAutotune(PID *pPid);

	void updateMeasure(PID);
	void autotune(PID);

	void clear() {
		storeMeasureList.clear();
	}

	virtual ~FsAutotune();
};

#endif /* PROCESSING_FLIGHTSTABILIZATION_FSAUTOTUNE_H_ */
