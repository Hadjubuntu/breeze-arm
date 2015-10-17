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
	if (_controler->hasPacketPending())
	{
		RfPacket packet = _controler->popFirstPacket();
		std::string packetStr = packet.getPayload();

		char charArray[packetStr.length()];
		strncpy(charArray, packetStr.c_str(), sizeof(packetStr.length()));

		Serial3.println(charArray);
	}
}
