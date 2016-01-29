/**
 * breeze-arm
 * Sonar.cpp
 * ----------------------------
 *
 * ----------------------------
 *  Created on: Jan 22, 2016
 *      Author: Adrien HADJ-SALAH
 */

#include "Sonar.h"
#include "../../../math/common/VectMath.h"
#include "../../../math/common/FastMath.h"

int nSonar = 10;
History<float> sonarValues(nSonar);

Sonar::Sonar() : Processing()
{
	_freqHz = 10;
	_sonarPin = 13;
	_filteredSonarValueCm = 0.0;
}

void Sonar::init()
{
	_sonarPin = 13;
	pinMode(_sonarPin, INPUT);

	// Create zero vector at initialization
	for (int i = 0; i < nSonar; i ++)
	{
		sonarValues.add(0.0);
	}
}

void Sonar::process()
{
	// Read new value
	float currentSonarVal = (float) analogRead(_sonarPin) * 0.3175; //

	// Filter measured value by using mean delta
	float deltaMean = VectMath::derivate(sonarValues.toVector());
	float deltaMeasured = currentSonarVal - sonarValues.getLast();

	float filteredValue = currentSonarVal;

	// Do not consider spike value due to analog wrong reads
	if (FastMath::fabs(deltaMeasured) >= 150) {
		filteredValue = sonarValues.getLast();
	}
	else if (FastMath::fabs(deltaMeasured) > FastMath::fabs(deltaMean)) {
		filteredValue = sonarValues.getLast() + deltaMeasured / 10.0;
	}

	// Store it to history
	sonarValues.add(filteredValue);
	//
	//	// Apply least-square filter
	_filteredSonarValueCm = _filter.apply(sonarValues.toVector(), nSonar + 1);
}

