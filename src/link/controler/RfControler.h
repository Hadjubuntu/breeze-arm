/*
 * RfControler.h
 *
 *  Created on: Sep 15, 2015
 *      Author: adrien
 */

#ifndef LINK_CONTROLER_RFCONTROLER_H_
#define LINK_CONTROLER_RFCONTROLER_H_

#include <iostream>

using namespace std;

class RfControler {
private:
	History<std::string> _packets;
public:
	RfControler();

	/**
	 * Listen to new packets which may have been received
	 */
	void listen();

	void send(RfPacket &);
};

#endif /* LINK_CONTROLER_RFCONTROLER_H_ */
