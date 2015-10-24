/*
 * ActuatorControl.cpp
 *
 *  Created on: Oct 23, 2015
 *      Author: adrien
 */

#include <wirish.h>
#include "ActuatorControl.h"

#define PULSE_MIN_WIDTH 700

// This picks the smallest prescaler that allows an overflow < 2^16.
#define MAX_OVERFLOW    ((1 << 16) - 1)
#define CYC_MSEC        (1000 * CYCLES_PER_MICROSECOND)
#define TAU_MSEC        20
#define TAU_USEC        (TAU_MSEC * 1000)
#define TAU_CYC         (TAU_MSEC * CYC_MSEC)
#define SERVO_PRESCALER (TAU_CYC / MAX_OVERFLOW + 1)
#define SERVO_OVERFLOW  ((uint16)round((double)TAU_CYC / SERVO_PRESCALER))

// Unit conversions
#define US_TO_COMPARE(us) ((uint16)map((us), 0, TAU_USEC, 0, SERVO_OVERFLOW))
#define COMPARE_TO_US(c)  ((uint32)map((c), 0, SERVO_OVERFLOW, 0, TAU_USEC))

ActuatorControl::ActuatorControl(FlightStabilization *pFlightStab)
{
	_freqHz = 50;
	_flightStabilization = pFlightStab;
}

void ActuatorControl::init()
{
	pinMode(D12,PWM);
	Timer3.setPeriod(20000); // 20000 microseconds = 50hz refresh
}


/**
 * Update command to actuators: servo-motors and motors
 */
void ActuatorControl::process()
{
	// Converts 0 to 1 signal to us signal
	unsigned short int val = (_flightStabilization->getThrottle() * 1000);
	// Write pulse
	pwmWrite(D12, US_TO_COMPARE(val + PULSE_MIN_WIDTH));
}
