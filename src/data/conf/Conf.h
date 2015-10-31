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

public:
	static Conf& getInstance();


	// Configuration parameters
	// ------------------------------------

	// Flight control parameters
	const float maxAbsRollAngle = 0.8;
	const float maxAbsPitchAngle = 0.8;
	const float maxAbsCombinedAngle = 0.7;
	// Max command on the torque in Nm
	const float maxCommandNm = 4.0;


	// Firmware
	const int firmware = Firmware::FIXED_WING;

	bool isFixedWing() {
		return (firmware == Firmware::FIXED_WING);
	}
	bool isMulticopter() {
		return (firmware == Firmware::MULTICOPTER);
	}
};

#endif /* DATA_CONF_CONF_H_ */
