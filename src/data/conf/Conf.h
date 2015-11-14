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
	MULTICOPTER = 1
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
	// Flight control parameters
	const float maxAbsRollAngle = 0.34; // 0.34 for 20 degress, 0.5 for approx 30 degrees, go higher to 0.8 for 45 degrees
	const float maxAbsPitchAngle = 0.34;
	const float maxAbsCombinedAngle = 0.5;
	// Max command on the torque in Nm
	const float maxCommandNm = 4.0;
	// Use boost motor to compensate roll/pitch angle
	const bool useBoostMotors = true;



	// Firmware
	const int firmware = Firmware::MULTICOPTER;

	bool isFixedWing() {
		return (firmware == Firmware::FIXED_WING);
	}
	bool isMulticopter() {
		return (firmware == Firmware::MULTICOPTER);
	}


	/**
	 * Parse rf payload message and update configuration
	 */
	void parseRf(std::string payload);
};

#endif /* DATA_CONF_CONF_H_ */
