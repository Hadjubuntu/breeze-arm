/*
 * ActuatorControl.cpp
 *
 *  Created on: Oct 23, 2015
 *      Author: adrien
 */

#include <wirish.h>
#include "ActuatorControl.h"

/**
 * Timer map
 * Timer 	Ch. 1 pin 	Ch. 2 pin 	Ch. 3 pin 	Ch. 4 pin
Timer1 	6 	7 	8 	–
Timer2 	2 	3 	1 	0
Timer3 	12 	11 	27 	28
Timer4 	5 	9 	14 	24
 */

#define PULSE_MIN_WIDTH 600

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

#define HZ_TO_US(f) (1000000.0f/f)

/**
 * Processing constructor
 */
ActuatorControl::ActuatorControl(FlightStabilization *pFlightStab) : Processing()
{
	_freqHz = 50;
	_flightStabilization = pFlightStab;
}

/**
 * Initialize timers
 */
void ActuatorControl::init()
{
	// Prepare all pin output
	// -----------------------
	// Timer 3 for motors at 480 Hz
//	pinMode(D12, PWM);
//	pinMode(D11, PWM);
//	pinMode(D27, PWM);
	pinMode(D28, PWM);

	// Timer 4 for servos at 50 Hz
	pinMode(D14, PWM);
//	pinMode(D24, PWM);
//	pinMode(D5, PWM);
//	pinMode(D9, PWM);


	// Set frequency for timers
	Timer3.setPeriod((uint32) HZ_TO_US(490)); // 490 Hz
	Timer4.setPeriod(20000); // 20000 microseconds = 50hz refresh
}


/**
 * Update command to actuators: servo-motors and motors
 */
void ActuatorControl::process()
{
	// Converts 0 to 1 signal to us signal 2000us wide
	unsigned short int val = (_flightStabilization->getThrottle() * 1850);

	// Write pulse
	pwmWrite(D28, US_TO_COMPARE(val + PULSE_MIN_WIDTH));
	pwmWrite(D14, US_TO_COMPARE(val + PULSE_MIN_WIDTH));
}
