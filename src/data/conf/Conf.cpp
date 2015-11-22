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

/**
 * const float maxAbsRollAngle = 0.34; // 0.34 for 20 degress, 0.5 for approx 30 degrees, go higher to 0.8 for 45 degrees
	const float maxAbsPitchAngle = 0.34;
	const float maxAbsCombinedAngle = 0.5;
	// Max command on the torque in Nm
	const float maxCommandNm = 6.0;
	// Use boost motor to compensate roll/pitch angle
	const bool useBoostMotors = true;
 */
Conf::Conf() {
	_parameters.push_back(Param<float>("UNKNOW", 0.0f));
	// 0.34 for 20 degress, 0.5 for approx 30 degrees, go higher to 0.8 for 45 degrees
	_parameters.push_back(Param<float>("maxAbsRollAngle", 0.34f));
	_parameters.push_back(Param<float>("maxAbsPitchAngle", 0.34f));
	_parameters.push_back(Param<float>("maxAbsCombinedAngle", 0.5f));
	// Max command on the torque in Nm
	_parameters.push_back(Param<float>("maxCommandNm", 6.0f));
}



Param<float>* Conf::get(std::string pName)
{
	Param<float>* e;
	int idx = find(pName);

	if (idx >= 0)
	{
		e = &_parameters[idx];
	}
	else {
		// Unknow param
		e = &_parameters[0];
	}

	return e;
}

int Conf::set(std::string pName, float pValue)
{
	int idx = find(pName);
	if (idx >= 0)
	{
		_parameters[idx].setValue(pValue);
	}
	else
	{
		_parameters.push_back(Param<float>(pName, pValue));
	}

	return idx;
}

int Conf::find(std::string pName)
{
	int findIndex = -1;
	int index = 0;

	while (index < _parameters.size() && findIndex == -1)
	{
		if (_parameters[index].getName().compare(pName) == 0)
		{
			findIndex = index;
		}
		index ++;
	}

	return findIndex;
}

Conf& Conf::getInstance()
{
	return INSTANCE;
}

/**
 * Retrieve parameter name and value from payload packet message
 */
void Conf::parseRf(std::string payload)
{
	// TODO split using separator
	std::string paramName = "";
	float paramValue = 0.0;

	// If parameter exists, then set value
	if (find(paramName) > 0)
	{
		set(paramName, paramValue);
	}
	// Otherwise send to GCS the exception
	else {

	}
}

void Conf::sendConfToGcs()
{
	for (Param<float> param : _parameters)
	{
		RfPacket packet(Date::now(), "CONF", param.toString());
		_rfControler->addPacketToSend(packet);
	}
}
