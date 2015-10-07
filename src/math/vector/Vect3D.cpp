/*
 * Vect3.cpp
 *
 *  Created on: Sep 23, 2015
 *      Author: adrien
 */

#include "Vect3D.h"
#include "../common/FastMath.h"

Vect3D Vect3D::toRad()
{
	Vect3D e(
			FastMath::toRadians(_x),
			FastMath::toRadians(_y),
			FastMath::toRadians(_z));
	return e;
}
