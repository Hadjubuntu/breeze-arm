/*
 * RadioControler.cpp
 *
 *  Created on: Oct 18, 2015
 *      Author: adrien
 */

#include "RadioControler.h"

/**
 * Constructor
 */
RadioControler::RadioControler()
{
	// 100 Hz updater
	_freqHz = 100;
}

/**
 * Initialize radio controller
 */
void RadioControler::init()
{
	// Start serial for sbus radio
	_handler.begin();

	// Wait for two seconds
	wait(2.0);

	// Set calibration values
	for (int i = 0; i < 7; i ++) {
		_handler.channelsCalib[i] = _handler.channels[i];
	}
}

void RadioControler::process()
{
	_handler.FeedLine();

	if (_handler.toChannels == 1)
	{
		_handler.UpdateChannels();
		_handler.lastUpdate = Date::now();
		_handler.toChannels = 0;
	}
}
