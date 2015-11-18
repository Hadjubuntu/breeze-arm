/*
 * ActuatorControl.cpp
 *
 *  Created on: Oct 23, 2015
 *      Author: adrien
 */

#include <wirish.h>
#include "../../math/common/FastMath.h"
#include "../../data/conf/Conf.h"
#include "../../link/RadioSbus.h"
#include "ActuatorControl.h"
#include <array>

/**
 * Timer map
 * Timer 	Ch. 1 pin 	Ch. 2 pin 	Ch. 3 pin 	Ch. 4 pin
Timer1 	6 	7 	8 	–
Timer2 	2 	3 	1 	0
Timer3 	12 	11 	27 	28
Timer4 	5 	9 	14 	24
 */


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

unsigned short levelToCtrl(unsigned short level)
{
	unsigned short ctrl = (level < 0)?0:((level >= 1000)?50000:((1000 + level) * 24));
	return ctrl;
}



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
	pinMode(D28, PWM);
	pinMode(D27, PWM);
	pinMode(D11, PWM);
	pinMode(D12, PWM);

	// Timer 4 for servos at 50 Hz
	pinMode(D14, PWM);
	//	pinMode(D24, PWM);
	//	pinMode(D5, PWM);
	//	pinMode(D9, PWM);


	// Set frequency for timers
	Timer3.setPeriod((uint32) HZ_TO_US(490)); // 490 Hz
	Timer4.setPeriod(20000); // 20000 microseconds = 50hz refresh

	// For multicopter only
	pwmWrite(D28, levelToCtrl(0));
	pwmWrite(D27, levelToCtrl(0));
	pwmWrite(D11, levelToCtrl(0));
	pwmWrite(D12, levelToCtrl(0));
}


/**
 * Update command to actuators: servo-motors and motors
 */
void ActuatorControl::process()
{
	// Converts 0 to 1 signal to us signal 1000ms wide
	unsigned short int throttle = (_flightStabilization->getThrottle() * 1000);


	switch (Conf::getInstance().firmware)
	{
	case FIXED_WING:
		processFixedWing(throttle);
		break;
	case HCOPTER:
		processMulticopter(throttle, 4);
		break;
	case YCOPTER:
		processMulticopter(throttle, 3);
		break;
	default:
		// Throw error unknow firmware
		break;
	}
}

int ActuatorControl::getCommandNmToSignalUs(float commandNm, float nmToDeltaSignalUs)
{
	BoundAbs(commandNm, Conf::getInstance().maxCommandNm);
	int deltaSignal = (int) (-1) * commandNm * nmToDeltaSignalUs;

	BoundAbs(deltaSignal, RADIO_VAR);
	return deltaSignal;
}

void ActuatorControl::processFixedWing(unsigned short int  throttle)
{
	// Motors - write pulse
	pwmWrite(D28, US_TO_COMPARE(throttle));

	// Servos - write pulse
	// -----------------------
	// Wings / Roll
	Vect3D torqueCmd = _flightStabilization->getTau();
	int rollDeltaSignal = getCommandNmToSignalUs(torqueCmd.getX(), 150.0f);

	//	pwmWrite(D14, US_TO_COMPARE(throttle + PULSE_MIN_WIDTH));
	//	pwmWrite(D24, US_TO_COMPARE(throttle + PULSE_MIN_WIDTH));
	//
	//	// Pitch
	//	pwmWrite(D5, US_TO_COMPARE(throttle + PULSE_MIN_WIDTH));
	//
	//	// Rubber
	//	pwmWrite(D9, US_TO_COMPARE(throttle + PULSE_MIN_WIDTH));

	// Optionnal flaps
}


/**
 * H-copter:
 *
 * X1 ------ X2
 *      |
 *      |
 *      |
 * X3 ------ X4
 */
void ActuatorControl::processMulticopter(unsigned short int throttle, int nbMotors)
{
	// Retrieve torque command
	Vect3D torqueCmd = _flightStabilization->getTau();

	// Compute delta signal from torque command
	int rollDeltaSignal = getCommandNmToSignalUs(torqueCmd.getX(), 20.0f);
	int pitchDeltaSignal = getCommandNmToSignalUs(torqueCmd.getY(), 20.0f);
	int yawDeltaSignal = getCommandNmToSignalUs(torqueCmd.getZ(), 70.0f);

	int motorX[nbMotors];

	for (int j = 0; j < nbMotors;  j ++) {
		motorX[j] = 0;
	}

	int _motorActivation[4][3] = {
			{1, 1, -1},
			{-1, 1, 1},
			{1, -1, 1},
			{-1, -1, -1}
	};

	if (nbMotors == 3)
	{
		for (int j = 0; j < 4; j ++) {
			_motorActivation[j][2] = 0;
		}
	}

	// Set throttle repartition only throttle superior to a minimum threshold
	// Otherwise cut-off motors
	if (throttle > 15)
	{
		for (int i = 0; i < nbMotors; i ++)
		{
			motorX[i] = throttle
					+ _motorActivation[i][0] * rollDeltaSignal
					+ _motorActivation[i][1] * pitchDeltaSignal
					+ _motorActivation[i][2] * yawDeltaSignal;

			Bound(motorX[i], 0, 900);

			// Debug
			motors[i] = motorX[i];
		}
	}

	// Write pulse for motors
	pwmWrite(D28, levelToCtrl(motorX[0]));
	pwmWrite(D27, levelToCtrl(motorX[1]));
	pwmWrite(D11, levelToCtrl(motorX[2]));

	if (nbMotors == 4)
	{
		pwmWrite(D12, levelToCtrl(motorX[3]));
	}
	else {
		// Signal goes from 650 to 2250 ms
		pwmWrite(D14, US_TO_COMPARE(1450 + yawDeltaSignal));
	}
}
