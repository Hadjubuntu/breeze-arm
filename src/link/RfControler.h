/*
 * RfControler.h
 *
 *  Created on: Sep 15, 2015
 *      Author: adrien
 */

#ifndef LINK_CONTROLER_RFCONTROLER_H_
#define LINK_CONTROLER_RFCONTROLER_H_


#include <list>
#include "../core/Processing.h"
#include "RfPacket.h"


#define RfSerial Serial3

class RfControler : public Processing  {
private:
	/** Packet end char */
	const char _endPacketChar = '\n';

	/** List of packets received */
	std::list<RfPacket> _receivedPackets;

	/** Incoming packet in construction until end character is received */
	std::string _incomingPacket;

	/** To send packets buffer */
	std::list<RfPacket> _toSendPackets;

	void receiveNewPackets();
	void sendPackets();

public:
	RfControler();

	void init() {
		// TODO
	}

	/**
	 * Listen to new packets which may have been received
	 */
	void process();

	void send(RfPacket &);

	RfPacket popFirstPacket();

	bool hasPacketPending();

	void addPacketToSend(RfPacket packet) {
		_toSendPackets.push_back(packet);
	}
};

#endif /* LINK_CONTROLER_RFCONTROLER_H_ */
