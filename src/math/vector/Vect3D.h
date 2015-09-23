/*
 * Vect3.h
 *
 *  Created on: Sep 23, 2015
 *      Author: adrien
 */

#ifndef MATH_VECTOR_VECT3D_H_
#define MATH_VECTOR_VECT3D_H_

class Vect3D {
private:
	float _x;
	float _y;
	float _z;
public:
	/** Default constructor */
	Vect3D(float x, float y, float z) : _x(x), _y(y), _z(z) {

	}

	/** *************************************************
	 * Basic functions on vectors
	 ************************************************** */
	float dotProduct(Vect3D& vect) {
		return _x * vect.getX() + _y * vect.getY() + _z * vect.getZ();
	}

	Vect3D crossProduct(Vect3D& vect) {
		Vect3D e(_y * vect._z - _z * vect._y,
			  	 _z * vect._x - _x * vect._z,
				 _x * vect._y - _y * vect._x);
		return e;
	}

	/** *************************************************
	 * Operators
	 ************************************************** */
	Vect3D operator*(float scalar) const {
		return Vect3D(_x * scalar, _y * scalar, _z * scalar);
	}
	Vect3D operator+(const Vect3D &vect) const {
		return Vect3D(_x + vect._x, _y + vect._y, _z + vect._z);
	}
	Vect3D operator-(const Vect3D &vect) const {
		return Vect3D(_x - vect._x, _y - vect._y, _z - vect._z);
	}
	Vect3D operator*(const Vect3D &vect) const {
		return Vect3D(_x * vect._x, _y * vect._y, _z * vect._z);
	}
	Vect3D operator/(const Vect3D &vect) const {
		return Vect3D(_x / vect._x, _y / vect._y, _z / vect._z);
	}
	float operator[](int i) const {
		if (i == 0) {
			return _x;
		} else if (i == 1) {
			return _y;
		}
		else {
			return _z;
		}
	}
	void operator=(Vect3D &vect) {
		_x = vect._x;
		_y = vect._y;
		_z = vect._z;
	}
	void operator=(float array[]) {
		_x = array[0];
		_y = array[1];
		_z = array[2];
	}

	/** *************************************************
	 * Getters
	 ************************************************** */
	float getX() {
		return _x;
	}

	float getY() {
		return _y;
	}

	float getZ() {
		return _z;
	}

	static Vect3D zero() {
		Vect3D zero(0.0, 0.0, 0.0);
		return zero;
	}
};

#endif /* MATH_VECTOR_VECT3D_H_ */