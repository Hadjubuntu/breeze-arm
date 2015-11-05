/*
 * RfControler.cpp
 *
 *  Created on: Sep 15, 2015
 *      Author: adrien
 */


#include <stdio.h>
#include "RfControler.h"

RfControler::RfControler() : Processing()
{
	_freqHz = 20;
}


void RfControler::process()
{
	if (RfSerial.available() > 0) {
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
}

void RfControler::send(RfPacket& packet)
{
	std::string header = packet.getHeader();
	std::string payload = packet.getPayload();

	int byteBuffer = header.length() + payload.length() + 1;

	if (byteBuffer <= RF_PACKET_MAX_LENGTH)
	{
		// Create byte to send
		char charArray[byteBuffer];

		// Concatenate data
		sprintf(charArray, "%s|%s\n", header.c_str(), payload.c_str());

		// Send data TODO
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
