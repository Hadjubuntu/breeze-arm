/*
 * Telemetry.cpp
 *
 *  Created on: Nov 8, 2015
 *      Author: adrien-i3
 */

#include <cstring>
#include <string>
#include <stdio.h>
#include "../../math/common/FastMath.h"
#include "Telemetry.h"
#include "../../link/RfPacket.h"


Logger telemetryLogger;

Telemetry::Telemetry(AHRS *pAhrs) : Processing()
{
	// Set pointer to attitude and heading reference processing
	_ahrs = pAhrs;
	// 1 Hz updater
	_freqHz = 1;
}

void Telemetry::init()
{

}

void Telemetry::process()
{
	// Build packet
	// --------------
	float rpy[3];
	_ahrs->getAttitude().toRollPitchYaw(rpy);
	char payload[RF_PACKET_MAX_LENGTH - 5];
	sprintf(payload, "%d;%d;%d",
			FastMath::toCenti(rpy[0]),
			FastMath::toCenti(rpy[1]),
			FastMath::toCenti(rpy[2]));

	// At each loop, build a telemetry packet and send it via Logger
	RfPacket packet(Date::now(), "TM", payload);

	// Send packet
	// --------------
	// Converts string to char array
// TODO to be continued
	//	char charArray[RF_PACKET_MAX_LENGTH];
//	strncpy(charArray, packetStr.c_str(), RF_PACKET_MAX_LENGTH-1);
//
//	telemetryLogger.info(charArray);
}


