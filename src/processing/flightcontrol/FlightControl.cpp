/*
 * FlightControl.cpp
 *
 *  Created on: Oct 4, 2015
 *      Author: adrien
 */

#include "FlightControl.h"

FlightControl::FlightControl() : _targetAttitude(Quaternion::zero()), _currentAttitude(Quaternion::zero())
{
	// TODO Auto-generated constructor stub

}


void FlightControl::setInputs(Quaternion pTargetAttitude, Quaternion pCurrentAttitude)
{
	_targetAttitude = pTargetAttitude;
	_currentAttitude = pCurrentAttitude;
}


void FlightControl::process()
{

}
