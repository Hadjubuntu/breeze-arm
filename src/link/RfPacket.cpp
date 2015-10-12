/*
 * RfPacket.cpp
 *
 *  Created on: Sep 15, 2015
 *      Author: adrien
 */

#include "RfPacket.h"

RfPacket::RfPacket(Date pDate, std::string pHeader, std::string pPayload) : _date(pDate) {
	_header = pHeader;
	_payload = pPayload;
}

