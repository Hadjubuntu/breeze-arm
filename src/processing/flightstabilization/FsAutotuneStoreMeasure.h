/*
 * FsAutotuneStoreMeasure.h
 *
 *  Created on: Apr 9, 2016
 *      Author: adrien
 */

#ifndef PROCESSING_FLIGHTSTABILIZATION_FSAUTOTUNESTOREMEASURE_H_
#define PROCESSING_FLIGHTSTABILIZATION_FSAUTOTUNESTOREMEASURE_H_

#include "FsAutotuneCollect.h"
#include "../../core/Processing.h"
#include "../../math/pid/PID.h"


class FsAutotuneStoreMeasure : public Processing {
protected:
	Date storeDate;
	FsAutotuneCollect *collector;
	float storedScore;
	float previousScore;
public:
	FsAutotuneStoreMeasure(PID *pPid);

	void init()
	{

	}
	/**
	 * Store data each x seconds
	 */
	void process();
	void callback() {}

	void executeCollector()
	{
		collector->process();
	}



	virtual ~FsAutotuneStoreMeasure();
};

#endif /* PROCESSING_FLIGHTSTABILIZATION_FSAUTOTUNESTOREMEASURE_H_ */
