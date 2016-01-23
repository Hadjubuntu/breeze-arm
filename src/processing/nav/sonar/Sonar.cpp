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


History<float> sonarValues(10);

Sonar::Sonar() : Processing()
{
	_freqHz = 10;
	_sonarPin = 13;
	_filteredSonarValueCm = 0.0;
}

void Sonar::init()
{
	_sonarPin = 13;

	// Create zero vector at initialization
	for (int i = 0; i < 10; i ++)
	{
		sonarValues.add(0.0);
	}
}

void Sonar::process()
{
	// Read new value
	float currentSonarVal = (float) analogRead(_sonarPin) * 0.3175;

	// Store it to history
	sonarValues.add(currentSonarVal);
//
//	// Apply least-square filter
	_filteredSonarValueCm = _filter.apply(sonarValues.toVector());
}

