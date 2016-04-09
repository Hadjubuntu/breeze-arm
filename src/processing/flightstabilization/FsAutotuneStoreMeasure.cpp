/*
 * FsAutotuneStoreMeasure.cpp
 *
 *  Created on: Apr 9, 2016
 *      Author: adrien
 */

#include "FsAutotuneStoreMeasure.h"

FsAutotuneStoreMeasure::FsAutotuneStoreMeasure(FsAutotuneCollect *pCollecter)
{
	freqHz = 1;
	storeDate = Date::now();
	collecter = pCollecter;
	storedScore = 0.0;
	previousScore = 0.0;
}

void FsAutotuneStoreMeasure::process()
{
	Date cDate = Date::now();
	if (cDate.durationFrom(storeDate) >= 1.0)
	{
		// Store previous score
		previousScore = storedScore;

		// Update new score
		storedScore = collecter->getScore();

		// Update store date
		storeDate = cDate;
	}
}

FsAutotuneStoreMeasure::~FsAutotuneStoreMeasure()
{

}

