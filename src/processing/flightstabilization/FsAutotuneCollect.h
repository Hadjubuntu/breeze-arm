/*
 * FsAutotuneCollect.h
 *
 *  Created on: Apr 9, 2016
 *      Author: adrien
 */

#ifndef PROCESSING_FLIGHTSTABILIZATION_FSAUTOTUNECOLLECT_H_
#define PROCESSING_FLIGHTSTABILIZATION_FSAUTOTUNECOLLECT_H_

class FsAutotuneCollect : public Processing {
protected:
	PID *pid;
	float score;
public:
	FsAutotuneCollect(PID*);

	void init()
	{

	}
	/**
	 * Compute an average score on pid output
	 */
	void process();

	float getScore() {
		return score;
	}

	virtual ~FsAutotuneCollect();
};

#endif /* PROCESSING_FLIGHTSTABILIZATION_FSAUTOTUNECOLLECT_H_ */
