/*
 * Date.h
 *
 *  Created on: Sep 15, 2015
 *      Author: adrien
 */

#ifndef MATH_TIME_DATE_H_
#define MATH_TIME_DATE_H_

class Date {
private:
	long _timeElapsedSinceStartNanoSeconds;
public:
	/**
	 * Constructor
	 */
	Date(long);

	/**
	 * Compare two dates
	 */
	int compareTo(Date &);

	/**
	 * Returns duration from another date in seconds
	 */
	double durationFrom(Date &);
};

#endif /* MATH_TIME_DATE_H_ */
