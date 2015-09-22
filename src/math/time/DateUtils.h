/*
 * DateUtils.h
 *
 *  Created on: Sep 21, 2015
 *      Author: adrien
 */

#ifndef MATH_TIME_DATEUTILS_H_
#define MATH_TIME_DATEUTILS_H_

#include "Date.h"

// Date constants
//------------------------------------------
#define S_TO_NS 1000000l


/**
 * Useful function to manipulate dates
 */
class DateUtils {
public:
	/**
	 * Transform delta time in microseconds into delta time in seconds
	 */
	static float microToSeconds(long dt) {
		return (float) (dt / S_TO_NS);
	}


};



#endif /* MATH_TIME_DATEUTILS_H_ */
