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

Brain uavBrain;
AHRS ahrs;

void setup() {
	/* Set up the LED to blink  */
	pinMode(BOARD_LED_PIN, OUTPUT);

	/* Send a message out USART2  */
	Serial3.begin(57600);
	Serial3.println("Hello world!");

	i2c_master_enable(I2C1, I2C_FAST_MODE);

	ahrs.initSensors();
}

void loop()
{
	toggleLED();


	ahrs.process();
	uavBrain.loop();

	if (uavBrain.getTickId() % 200 == 0)
	{
		char str[90];
		sprintf(str, "Gyro [%.2f, %.2f, %.2f]",
				ahrs.getGyro().getGyroFiltered().getX(),
				ahrs.getGyro().getGyroFiltered().getY(),
				ahrs.getGyro().getGyroFiltered().getZ()
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
