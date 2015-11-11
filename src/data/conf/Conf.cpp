/**
 * breeze-arm
 * Configuration.cpp
 * ----------------------------
 *
 * ----------------------------
 *  Created on: Oct 24, 2015
 *      Author: Adrien HADJ-SALAH
 */

#include <string>
#include "Conf.h"

Conf Conf::INSTANCE = Conf();

Conf::Conf() {

}


Conf& Conf::getInstance()
{
	return INSTANCE;
}

void Conf::parseRf(std::string payload)
{

}
