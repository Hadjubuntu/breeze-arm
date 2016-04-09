/*
 * FsAutotuneStoreMeasure.h
 *
 *  Created on: Apr 9, 2016
 *      Author: adrien
 */

#ifndef PROCESSING_FLIGHTSTABILIZATION_FSAUTOTUNESTOREMEASURE_H_
#define PROCESSING_FLIGHTSTABILIZATION_FSAUTOTUNESTOREMEASURE_H_

#include "FsAutotuneCollect.h"

class FsAutotuneStoreMeasure : public Processing {
protected:
	Date storeDate;
	FsAutotuneCollect *collecter;
	float storedScore;
	float previousScore;
public:
	FsAutotuneStoreMeasure(FsAutotuneCollect *collecter);

	void init()
	{

	}
	/**
	 * Store data each x seconds
	 */
	void process();
	void callback();

	virtual ~FsAutotuneStoreMeasure();
};

#endif /* PROCESSING_FLIGHTSTABILIZATION_FSAUTOTUNESTOREMEASURE_H_ */
