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

void RfControler::receiveNewPackets() {
	if (RfSerial.available() > 0) {
		char inChar = (char) (RfSerial.read());
		// If incoming character is a new line, then the packet is stored
		//----------------------------------------------------------
		if (inChar == _endPacketChar) {
			// Add packet to history
			RfPacket e(Date::now(), "no_header", _incomingPacket);
			_receivedPackets.push_back(e);
			// Reset incoming packet
			_incomingPacket.clear();
		} else        // Otherwise append new char to current incoming packet
		{
			// Push back character to string
			_incomingPacket += inChar;
		}
	}
}

void RfControler::sendPackets() {
	if (_toSendPackets.size() > 0)
	{
		RfPacket packet = _toSendPackets.front();
		_toSendPackets.pop_front();
		send(packet);
	}
}

void RfControler::process()
{
	receiveNewPackets();
	sendPackets();
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
	RfPacket packet = _receivedPackets.front();
	_receivedPackets.pop_front();
	return packet;
}

bool RfControler::hasPacketPending()
{
	return !_receivedPackets.empty();
}
