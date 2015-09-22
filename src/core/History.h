/*
 * History.h
 *
 *  Created on: Sep 15, 2015
 *      Author: adrien
 */

#ifndef CORE_HISTORY_H_
#define CORE_HISTORY_H_

#include <vector>

/**
 * Class keeping history of a generic object T
 */
template <class T>
class History {
private:
	int _size;
	std::vector<T> _history;
public:
	/**
	 * Constructor
	 */
	History(int size) : _size(size) {
		_history.reserve(_size);
	}


	/**
	 * Add element to history
	 */
	void add(T e);

	/**
	 * Erase history
	 */
	void clear();

	static History<T> getDefault() {
		History<T> e(10);
		return e;
	}
};

#endif /* CORE_HISTORY_H_ */
