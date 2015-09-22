/*
 * History.cpp
 *
 *  Created on: Sep 15, 2015
 *      Author: adrien
 */

#include "History.h"



template<class T>
void History<T>::add(T element)
{
	_history.push_back(element);
}


template<class T>
void History<T>::clear()
{
	_history.erase();
}
