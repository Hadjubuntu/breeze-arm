/*
 * Uav.h
 *
 *  Created on: Sep 15, 2015
 *      Author: adrien
 */

#ifndef MODEL_UAV_UAV_H_
#define MODEL_UAV_UAV_H_

class Uav {
private:
	int _id;
	History<UavState> states;
public:
	Uav();
};

#endif /* MODEL_UAV_UAV_H_ */
