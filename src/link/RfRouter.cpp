/*
 * RfRouter.cpp
 *
 *  Created on: Oct 16, 2015
 *      Author: adrien
 */

#include <cstring>
#include "RfRouter.h"

RfRouter::RfRouter(RfControler *pControler) {
	_freqHz = 20;
	_controler = pControler;
}

void RfRouter::process()
{
	// If controler has new packet awaiting
	if (_controler->hasPacketPending())
	{
		// Remove first packet
		RfPacket packet = _controler->popFirstPacket();

		// Get payload
		std::string packetStr = packet.getPayload();

		// Converts string to char array
		char charArray[RF_PACKET_MAX_LENGTH];
		strncpy(charArray, packetStr.c_str(), RF_PACKET_MAX_LENGTH-1);

		// Print data
		Serial3.println(charArray);

		// Route regarding its header data
	}
}
