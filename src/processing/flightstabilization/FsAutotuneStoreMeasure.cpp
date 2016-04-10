/*
 * FsAutotuneStoreMeasure.cpp
 *
 *  Created on: Apr 9, 2016
 *      Author: adrien
 */

#include "FsAutotuneStoreMeasure.h"

FsAutotuneStoreMeasure::FsAutotuneStoreMeasure(PID *pPid) : storeDate(Date::now())
{
	freqHz = 1;
	storedScore = 0.0;
	previousScore = 0.0;
	FsAutotuneCollect collectorObj(pPid);
	collector = &collectorObj;
}

void FsAutotuneStoreMeasure::process()
{
	Date cDate = Date::now();
	if (cDate.durationFrom(storeDate) >= 1.0)
	{
		// Store previous score
		previousScore = storedScore;

		// Update new score
		storedScore = collector->getScore();

		// Update store date
		storeDate = cDate;
	}
}

FsAutotuneStoreMeasure::~FsAutotuneStoreMeasure()
{

}

