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

#define RF_PACKET_MAX_LENGTH 80

class RfPacket {
private:
	Date _date;
	std::string _header;
	std::string _payload;
public:
	/**
	 * Default constructor
	 * @param pDate date
	 * @param pHeader header
	 * @param pPayload Payload
	 */
	RfPacket(Date pDate, std::string pHeader, std::string pPayload);


	/** ***********************************************
	 *  GETTERS
	 ************************************************ */
	std::string getHeader() {
		return _header;
	}
	std::string getPayload() {
		return _payload;
	}
};

#endif /* LINK_RFPACKET_H_ */
