/**
 * breeze-arm
 * VectMath.cpp
 * ----------------------------
 *
 * ----------------------------
 *  Created on: Jan 24, 2016
 *      Author: Adrien HADJ-SALAH
 */

#include "VectMath.h"
#include "../filter/LeastSquareFilter.h"

VectMath::VectMath() {
	// TODO Auto-generated constructor stub

}

float VectMath::derivate(std::vector<float> X)
{
	LeastSquareFilter filter;
	int n = X.size();

	float sumDeltaEstimation = 0.0;

	for (int i = 0; i < n; i ++)
	{
		float estimation = filter.apply(X, i);
		float delta = X.at(i) - estimation;
		sumDeltaEstimation += delta;
	}

	return sumDeltaEstimation / n;
}

