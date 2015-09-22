/*
 * UavState.h
 *
 *  Created on: Sep 15, 2015
 *      Author: adrien
 */

#ifndef PROCESSING_UAV_UAVSTATE_H_
#define PROCESSING_UAV_UAVSTATE_H_

class UavState {
private:
	// Date related to the state
	Date _date;

	// GeoPosition
	GeoPosition _geoPosition;

	// Speed
	double _speedMs;

	// AHRS state
	//
public:
	UavState();
};

#endif /* PROCESSING_UAV_UAVSTATE_H_ */
