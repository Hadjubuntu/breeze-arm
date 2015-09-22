/*
 * GeoPosition.cpp
 *
 *  Created on: Sep 18, 2015
 *      Author: adrien
 */

#include "../../data/nav/GeoPosition.h"

/**
 * Default constructor
 */
GeoPosition::GeoPosition(double pLat, double pLon, double pAlt) {
	_lat = pLat;
	_lon = pLon;
	_alt = pAlt;
}

