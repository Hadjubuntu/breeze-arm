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
	std::list<RfPacket> _packets;

	/** Incoming packet in construction until end character is received */
	std::string _incomingPacket;
public:
	RfControler();

	/**
	 * Listen to new packets which may have been received
	 */
	void process();

	void send(RfPacket &);

	RfPacket popFirstPacket();

	bool hasPacketPending();
};

#endif /* LINK_CONTROLER_RFCONTROLER_H_ */
