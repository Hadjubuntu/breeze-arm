/*
 * FastMath.cpp
 *
 *  Created on: Sep 18, 2015
 *      Author: adrien
 */

#include <stdint.h>
#include <cmath>
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

inline float FastMath::fastpow2(float p)
{
	float offset = (p < 0) ? 1.0f : 0.0f;
	float clipp = (p < -126) ? -126.0f : p;
	int w = clipp;
	float z = clipp - w + offset;
	union { uint32_t i; float f; } v = { uint32_t ( (1 << 23) * (clipp + 121.2740575f + 27.7280233f / (4.84252568f - z) - 1.49012907f * z) ) };

	return v.f;
}

inline float FastMath::exp(float x)
{
	return FastMath::fastpow2(1.442695040f * x);
}

inline float FastMath::sqrt(float x) {
	return std::sqrt(x);
}
