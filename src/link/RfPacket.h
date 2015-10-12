/*
 * RfPacket.h
 *
 *  Created on: Sep 15, 2015
 *      Author: adrien
 */

#ifndef LINK_RFPACKET_H_
#define LINK_RFPACKET_H_

#include <string>
#include "../math/time/Date.h"

using namespace std;

class RfPacket {
private:
	Date _date;
	std::string _header;
	std::string _payload;
public:
	RfPacket(Date pDate, std::string pHeader, std::string pPayload);
};

#endif /* LINK_RFPACKET_H_ */
