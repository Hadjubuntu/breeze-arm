/*
 * Quaternion.h
 *
 *  Created on: Sep 22, 2015
 *      Author: adrien
 */

#ifndef MATH_VECTOR_QUATERNION_H_
#define MATH_VECTOR_QUATERNION_H_

/**
 * Quaternion
 */
class Quaternion {
private:
	float _w;
	float _x;
	float _y;
	float _z;
public:
	/**
	 * Constructor of a quaternion
	 */
	Quaternion(float w, float x, float y, float z) : _w(w), _x(x), _y(y), _z(z)
	{

	}

	// TODO
	static Quaternion fromEuler(float rollRad, float pitchRad, float yawRad) {
		return Quaternion(0, 0, 0, 0);
	}

	// TODO toEuler > vect3D

	static Quaternion zero() {
		Quaternion e(0.0, 0.0, 0.0, 0.0);
		return e;
	}
};

#endif /* MATH_VECTOR_QUATERNION_H_ */
