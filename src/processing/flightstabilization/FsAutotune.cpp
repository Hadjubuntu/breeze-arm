/*
 * FsAutotune.cpp
 *
 *  Created on: Apr 9, 2016
 *      Author: adrien
 */

#include "FsAutotune.h"

FsAutotune::FsAutotune() :  Processing()
{
	_freqHz = 50;
}

void FsAutotune::process()
{
	for (PID pid : pidList)
	{
		autotune(pid);
	}
}

void FsAutotune::autotune(PID pid)
{
	// For a PID

	// score - prevScore (integral ?)
	// PID score = responsiveness ; error overhead ; mean error

	// If ds > 0 || probability to change
	// 		Accept parameter
	// Otherwise
	// 		Rollback

	// --------MEASURE1---------1s-----------MEASURE2---------2s--------....------>
}

FsAutotune::~FsAutotune()
{
	/** Default destructor */
}

