/*
 * History.h
 *
 *  Created on: Sep 15, 2015
 *      Author: adrien
 */

#ifndef CORE_HISTORY_H_
#define CORE_HISTORY_H_

#include <vector>

using namespace std;

template <class T>
class History {
private:
	int _size;
	vector<T> _history;
public:
	/**
	 * Constructor
	 */
	History();

	/**
	 * Erase history
	 */
	void clear();
};

#endif /* CORE_HISTORY_H_ */
