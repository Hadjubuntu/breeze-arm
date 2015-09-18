/*
 * Brain.h
 *------------------------------------------------------
 *  Created on: Sep 15, 2015
 *      Author: adrien
 */

#ifndef CORE_BRAIN_H_
#define CORE_BRAIN_H_


#include "History.h"

class Brain {
private:
	// History of link command
	History<int> _test;
public:
	/**
	 * Initialize brain
	 */
	Brain();

	/**
	 * Loop
	 */
	void loop();
};

#endif /* CORE_BRAIN_H_ */
