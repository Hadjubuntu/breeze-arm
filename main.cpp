/**
 * Main source code of Breeze ARM project
 */
#include <stdio.h>
#include <wirish/wirish.h>
#include <vector>
#include "libraries/Wire/Wire.h"
#include "libmaple/i2c.h"
#include "src/core/Brain.h"
#include "src/processing/ahrs/AHRS.h"

AHRS ahrs;
Brain uavBrain;

void setup() {
	/* Set up the LED to blink  */
	pinMode(BOARD_LED_PIN, OUTPUT);

	/* Send a message out USART2  */
	Serial3.begin(57600);
	Serial3.println("Hello world!");

	i2c_master_enable(I2C1, I2C_FAST_MODE);

	uavBrain.addProcessing(&ahrs);
	ahrs.initSensors();
}

void loop()
{
	toggleLED();

	uavBrain.loop();

	if (uavBrain.getTickId() % 100 == 0)
	{
//		char str[90];
//		sprintf(str, "Gyro [%.2f, %.2f, %.2f]",
//				ahrs.getGyro().getGyroFiltered().getX(),
//				ahrs.getGyro().getGyroFiltered().getY(),
//				ahrs.getGyro().getGyroFiltered().getZ()
//		);

		char str[90];
		sprintf(str, "Acc [%.2f, %.2f, %.2f]",
				ahrs.getAcc().getAccRaw().getX(),
				ahrs.getAcc().getAccRaw().getY(),
				ahrs.getAcc().getAccRaw().getZ()
		);

		Serial3.println(str);
	}
}

// Force init to be called *first*, i.e. before static object allocation.
// Otherwise, statically allocated objects that need libmaple may fail.
__attribute__((constructor)) void premain() {
	init();
}

int main(void) {
	setup();

	while (true) {
		loop();
	}

	return 0;
}
