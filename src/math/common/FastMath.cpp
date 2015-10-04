/*
 * FastMath.cpp
 *
 *  Created on: Sep 18, 2015
 *      Author: adrien
 */

#include "FastMath.h"

FastMath::FastMath() {

}
float FastMath::toRadians(float angleDegrees)
{
	return angleDegrees / RAD2DEG;
}

float FastMath::toDegrees(float angleRadians)
{
	return angleRadians * RAD2DEG;
}

