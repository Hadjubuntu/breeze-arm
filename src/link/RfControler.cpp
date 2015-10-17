/*
 * RfControler.cpp
 *
 *  Created on: Sep 15, 2015
 *      Author: adrien
 */

#include "RfControler.h"

RfControler::RfControler() : Processing()
{
	_freqHz = 20;
}


void RfControler::process()
{
	char inChar = (char) RfSerial.read();

	// If incoming character is a new line, then the packet is stored
	//----------------------------------------------------------
	if (inChar == _endPacketChar)
	{
		// Add packet to history
		RfPacket e(Date::now(), "no_header", _incomingPacket);
		_packets.push_back(e);

		// Reset incoming packet
		_incomingPacket.clear();

	}
	// Otherwise append new char to current incoming packet
	else
	{
		// Push back character to string
		_incomingPacket += inChar;
	}
}

RfPacket RfControler::popFirstPacket()
{
	RfPacket packet = _packets.front();
	_packets.pop_front();
	return packet;
}

bool RfControler::hasPacketPending()
{
	return !_packets.empty();
}
