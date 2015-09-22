/*
 * GeoPosition.h
 *
 *  Created on: Sep 18, 2015
 *      Author: adrien
 */

#ifndef PROCESSING_NAV_GEOPOSITION_H_
#define PROCESSING_NAV_GEOPOSITION_H_

class GeoPosition {
private:
	/** Latitude in degrees */
	double _lat;
	/** Longitude in radians */
	double _lon;
	/** Altitude in meters */
	double _alt;
public:
	/**
	 * Default constructor
	 */
	GeoPosition(double pLat, double pLon, double pLat);

	/**
	 * Distance to another geo position in meters
	 */
	double distance(const GeoPosition&);

	/**
	 * Heading needed to reach another point (radians)
	 */
	double headingTo(const GeoPosition &);
};

#endif /* PROCESSING_NAV_GEOPOSITION_H_ */
