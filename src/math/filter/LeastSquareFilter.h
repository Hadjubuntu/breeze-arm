/**
 * breeze-arm
 * LeastSquare.h
 * ----------------------------
 *
 * ----------------------------
 *  Created on: Jan 22, 2016
 *      Author: Adrien HADJ-SALAH
 */

#ifndef MATH_FILTER_LEASTSQUAREFILTER_H_
#define MATH_FILTER_LEASTSQUAREFILTER_H_

#include <vector>

class LeastSquareFilter {
public:
	LeastSquareFilter();

	float apply(std::vector<float> pValues);

	std::vector<float> createSimpleVector(int n);

	// Vector functions
	// ----
	static float mean(std::vector<float> pX);
	static std::vector<float> addScalar(std::vector<float> pVect, float pScalar);
	static std::vector<float>  product(std::vector<float> A, std::vector<float> B);
	static float sum(std::vector<float> A);

};

#endif /* MATH_FILTER_LEASTSQUAREFILTER_H_ */
