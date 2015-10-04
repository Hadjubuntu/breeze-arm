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
	Quaternion(float w, float x, float y, float z) : _w(w), _x(x), _y(y), _z(z) {
		// Default constructor
	}

	void toRollPitchYaw(float *rpy);


	float getNorm() { return _w *_w + _x*_x +_y*_y + _z*_z; }

	float operator[](int x) {
		if (x == 0) {
			return _w;
		} else if (x == 1) {
			return _x;
		} else if (x == 2) {
			return _y;
		} else if (x == 3) {
			return _z;
		} else {
			// TODO throw error
			return 0.0;
		}
	}

	Quaternion operator+=(float array[]) {
		_w += array[0];
		_x += array[1];
		_y += array[2];
		_z += array[3];
		return (*this);
	}
	Quaternion operator-=(float array[]) {
		_w -= array[0];
		_x -= array[1];
		_y -= array[2];
		_z -= array[3];
		return (*this);
	}
	Quaternion operator*=(float v) {
		_w *= v;
		_x *= v;
		_y *= v;
		_z *= v;
		return (*this);
	}

	Quaternion operator=(Quaternion q) {
		_w = q[0];
		_x = q[1];
		_y = q[2];
		_z = q[3];
		return (*this);
	}
	Quaternion operator-() {
		_w = -_w;
		_x = -_x;
		_y = -_y;
		_z = -_z;
		return (*this);
	}
	// TODO
	static Quaternion fromEuler(float rollRad, float pitchRad, float yawRad) {
		return Quaternion(0, 0, 0, 0);
	}

	// TODO toEuler > vect3D

	static Quaternion zero() {
		Quaternion e(1.0, 0.0, 0.0, 0.0);
		return e;
	}
};

#endif /* MATH_VECTOR_QUATERNION_H_ */
