/*
 * FastMath.h
 *
 *  Created on: Sep 18, 2015
 *      Author: adrien
 */

#ifndef MATH_COMMON_FASTMATH_H_
#define MATH_COMMON_FASTMATH_H_


class FastMath {
private:
	static constexpr float RAD2DEG = 57.29577951f;
public:
	FastMath();
	static float toDegrees(float angleRadians);
	static float toRadians(float angleDegrees);
};

#endif /* MATH_COMMON_FASTMATH_H_ */
