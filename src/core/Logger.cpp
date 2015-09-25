/*
 * Logger.cpp
 *
 *  Created on: Sep 24, 2015
 *      Author: adrien
 */

#include <string>
#include <wirish/wirish.h>
#include "Logger.h"

	void Logger::info(const char str[])
	{
		if (_mode == INFO || _mode == DEBUG)
		{
			Serial3.println(str);
		}
	}

	void Logger::debug(const char str[])
	{
		if (_mode == DEBUG) {
			Serial3.println(str);
		}
	}

