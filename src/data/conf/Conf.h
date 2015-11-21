/**
 * breeze-arm
 * Configuration.h
 * ----------------------------
 *
 * ----------------------------
 *  Created on: Oct 24, 2015
 *      Author: Adrien HADJ-SALAH
 */

#ifndef DATA_CONF_CONF_H_
#define DATA_CONF_CONF_H_

#include <vector>
#include "Param.h"

enum Firmware: int {
	FIXED_WING = 0,
	HCOPTER = 1,
	YCOPTER = 2,
};

class Conf {
private:
	// Singleton variables and functions
	// ------------------------------------
	static Conf INSTANCE;

	Conf& operator= (const Conf&){}
	Conf (const Conf &){}

	Conf() ;

	std::vector<Param<float>> _parameters;

public:
	static Conf& getInstance();

	Param<float>* get(std::string pName);
	void set(std::string pName, float pValue);
	int find(std::string pName);


	// Default configuration parameters
	// ------------------------------------
	// Use boost motor to compensate roll/pitch angle
	const bool useBoostMotors = true;


	// Firmware
	const int firmware = Firmware::YCOPTER;


	/**
	 * Parse rf payload message and update configuration
	 */
	void parseRf(std::string payload);
};

#endif /* DATA_CONF_CONF_H_ */
