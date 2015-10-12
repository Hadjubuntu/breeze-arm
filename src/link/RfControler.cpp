/*
 * RfControler.cpp
 *
 *  Created on: Sep 15, 2015
 *      Author: adrien
 */

#include "RfControler.h"

RfControler::RfControler() : Processing(), _packets(History<std::string>(30))
{
	_freqHz = 2;
}


void RfControler::process()
{
	Serial3.println("hello controller rf");
}
