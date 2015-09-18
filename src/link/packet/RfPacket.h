/*
 * RfPacket.h
 *
 *  Created on: Sep 15, 2015
 *      Author: adrien
 */

#ifndef LINK_RFPACKET_H_
#define LINK_RFPACKET_H_

#include <iostream>

using namespace std;

class RfPacket {
private:
	Date date;
	string payload;
public:
	RfPacket();
};

#endif /* LINK_RFPACKET_H_ */
