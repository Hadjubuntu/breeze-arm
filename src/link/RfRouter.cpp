/*
 * RfRouter.cpp
 *
 *  Created on: Oct 16, 2015
 *      Author: adrien
 */

#include <cstring>
#include "../data/conf/Conf.h"
#include "RfRouter.h"

RfRouter::RfRouter(RfControler *pControler)
{
	// Rf router check for new message at 20Hz and route packet regarding their header
	_freqHz = 20;
	_controler = pControler;
}

void RfRouter::process()
{
	// If controler has new packet awaiting
	if (_controler->hasPacketPending())
	{
		// Remove first packet
		RfPacket packet = _controler->popFirstPacket();

		// Get payload
		std::string packetStr = packet.getPayload();

		// Route regarding its header data
//		switch (packet.getHeader())
//		{
//		case 'CONF':
//			Conf::getInstance().parseRf(packet.getPayload());
//			break;
//		default:
//			// Unknow message
//			break;
//		}
	}
}
