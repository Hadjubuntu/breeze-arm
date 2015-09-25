/*
 * Logger.h
 *
 *  Created on: Sep 24, 2015
 *      Author: adrien
 */

#ifndef CORE_LOGGER_H_
#define CORE_LOGGER_H_

#include <string>

enum LoggerMode
{
	DEBUG,
	INFO,
	OPERATIONNAL
};

class Logger {
private:
	static Logger _instance;
	LoggerMode _mode;
public:
	Logger() : _mode(DEBUG) {

	}

	void info(const char str[]) ;
	void debug(const char str[]);

};


#endif /* CORE_LOGGER_H_ */
