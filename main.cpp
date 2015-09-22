/**
 * Main source code of Breeze ARM project
 */
#include <stdio.h>
#include <wirish/wirish.h>
#include <vector>
#include "libraries/Wire/Wire.h"
#include "libmaple/i2c.h"
#include "src/core/Brain.h"


////////Acceleration sensor ADXL345 function/////////////////////////////
#define ACC (0x53)    //Defined ADXL345 address, ALT ADDRESS pin is grounded
#define A_TO_READ (6) //the number of bytes to read(each axis accounted for two-byte)
#define XL345_DEVID   0xE5 //ADXL345 ID register，需要注意芯片有一个地址选择线将AD0连接到GND口
// ADXL345 Control register
#define ADXLREG_TAP_AXES     0x2A
#define ADXLREG_BW_RATE      0x2C
#define ADXLREG_POWER_CTL    0x2D
#define ADXLREG_INT_ENABLE   0x2E
#define ADXLREG_DATA_FORMAT  0x31
#define ADXLREG_FIFO_CTL     0x38
#define ADXLREG_DUR          0x21

//ADXL345 Data register
#define ADXLREG_DEVID        0x00
#define ADXLREG_DATAX0       0x32
#define ADXLREG_DATAX1       0x33
#define ADXLREG_DATAY0       0x34
#define ADXLREG_DATAY1       0x35
#define ADXLREG_DATAZ0       0x36
#define ADXLREG_DATAZ1       0x37

void getAccelerometerData(int16 *);
void writeVar(void *, uint8);

////////////////////////////////////////////////////////////////////////////////////
// Function prototype: void writeTo (uint8 DEVICE uint8 address, uint8 val)
// Parameter Description: DEVICE: I2C device address
// Address: Operation register address
// Val: write register values
// Return Value: None
// Description: val is written to the corresponding address register through the I2C bus
///////////////////////////////////////////////////////////////////////////////////
void writeTo(uint8 DEVICE, uint8 address, uint8 val)
{
  // all i2c transactions send and receive arrays of i2c_msg objects
  i2c_msg msgs[1]; // we dont do any bursting here, so we only need one i2c_msg object
 uint8 msg_data[2] = {address,val};

  msgs[0].addr = DEVICE;
  msgs[0].flags = 0; // write
  msgs[0].length = 2; //Write two data
  msgs[0].data = msg_data;
  i2c_master_xfer(I2C1, msgs, 1,0);  //
}

////////////////////////////////////////////////////////////////////////////////////
// Function prototype: void readFrom (the uint8 DEVICE uint8 address, uint8 num, uint8 * msg_data)
// Parameter Description: DEVICE: I2C device address
// Address: Operation register address
// Num: the number of reads
// * Msg_data: read data stored pointer
// Return Value: None
// Description: I2C bus to read data
///////////////////////////////////////////////////////////////////////////////////
void readFrom(uint8 DEVICE, uint8 address, uint8 num, uint8 *msg_data)
{
  i2c_msg msgs[1];
  msg_data[0] = address;

  msgs[0].addr = DEVICE;
  msgs[0].flags = 0; //write flag is 0
  msgs[0].length = 1; // just one byte for the address to read, 0x00
  msgs[0].data = msg_data;
  i2c_master_xfer(I2C1, msgs, 1,0);

  msgs[0].addr = DEVICE;
  msgs[0].flags = I2C_MSG_READ; //read
  msgs[0].length = num; // Read the number of bytes
  msgs[0].data = msg_data;
  i2c_master_xfer(I2C1, msgs, 1,0);
}


void serialFloatPrint(float f)
{
  uint8 *b = (uint8 *)&f;
  for(uint8 i=0; i<4; i++)
  {

    uint8 b1 = (b[i] >> 4) & 0x0f;
    uint8 b2 = (b[i] & 0x0f);

    char c1 = (b1 < 10) ? ('0' + b1) : 'A' + b1 - 10;
    char c2 = (b2 < 10) ? ('0' + b2) : 'A' + b2 - 10;

    SerialUSB.print(c1);
    SerialUSB.print(c2);
  }
}


void writeArr(void * varr, uint8 arr_length, uint8 type_bytes)
{
  uint8 *arr = (uint8 *) varr;
  for(uint8 i=0; i<arr_length; i++)
  {
    writeVar(&arr[i * type_bytes], type_bytes);
  }
}


// thanks to Francesco Ferrara and the Simplo project for the following code!
void writeVar(void *val, uint8 type_bytes)
{
  uint8 * addr=(uint8 *)(val);
  for(uint8 i=0; i<type_bytes; i++)
  {
    SerialUSB.write(addr[i]);
  }
}

// Accelerometer correction offset
int16 a_offset[3];
////////////////////////////////////////////////////////////////////////////////////
// Function prototype: void initAcc (void)
// Parameter Description: None
// Return Value: None
// Description: Initialize ADXL345 accelerometer
///////////////////////////////////////////////////////////////////////////////////
void initAcc(void)
{
	//all i2c transactions send and receive arrays of i2c_msg objects
	i2c_msg msgs[1]; // we dont do any bursting here, so we only need one i2c_msg object
	uint8 msg_data[2]= {
			0x00,0x00  };
	msgs[0].addr = ACC;
	msgs[0].flags = 0;
	msgs[0].length = 1; // just one byte for the address to read, 0x00
	msgs[0].data = msg_data;
	i2c_master_xfer(I2C1, msgs, 1,0);

	msgs[0].addr = ACC;
	msgs[0].flags = I2C_MSG_READ;
	msgs[0].length = 1; // just one byte for the address to read, 0x00
	msgs[0].data = msg_data;
	i2c_master_xfer(I2C1, msgs, 1,0);
	// now we check msg_data for our 0xE5 magic number
	uint8 dev_id = msg_data[0];
	//SerialUSB.print("Read device ID from xl345: ");
	//SerialUSB.println(dev_id,HEX);

	Serial3.println("Writing to accelerometer");

	if (dev_id != XL345_DEVID)
	{
		Serial3.println("Error, incorrect xl345 devid!");
		Serial3.println("Halting program, hit reset...");
		waitForButtonPress(0);
	}
	//invoke ADXL345
	writeTo(ACC,ADXLREG_POWER_CTL,0x00);
	writeTo(ACC,ADXLREG_POWER_CTL,0x08); //Set accelerometer to measure mode

	Serial3.println("Conf acc ok..");

	delay(100);
	// Calculate offset
	float accumulator[] = {0,0,0};
	int num_samples = 30;
	for(int i = 0 ; i < num_samples ; i++) {
		short acc[3];
		getAccelerometerData(acc);
		accumulator[0] += acc[0];
		accumulator[1] += acc[1];
		accumulator[2] += acc[2];

		Serial3.println("New sample of acc data received..");
		delay(100);
	}
	for(int i = 0 ; i < 3 ; i++) accumulator[i] /= num_samples;
	accumulator[2] -= 248; // 1g at 2mg/LSB more or less.
	for(int i = 0 ; i < 3 ; i++) a_offset[i] = accumulator[i];
	for(int i = 0 ; i < 3 ; i++) Serial3.println(accumulator[i]);
}

////////////////////////////////////////////////////////////////////////////////////
// Function prototype: void getAccelerometerData (int16 * result)
// Parameter Description: * result: read acceleration values ​​pointer
// Return Value: None
// Description: Read the the ADXL345 accelerometer original data
///////////////////////////////////////////////////////////////////////////////////
void getAccelerometerData(int16 * result)
{
	int16 regAddress = ADXLREG_DATAX0;    //start reading byte
	uint8 buff[A_TO_READ];

	readFrom(ACC, regAddress, A_TO_READ, buff); //read ADXL345 data and store it in buffer

	//Readings for each axis with 10-bit resolution, ie 2 bytes.
	//We want to convert two bytes into an int variable
	result[0] = ((((int16)buff[1]) << 8) | buff[0]) - a_offset[0];
	result[1] = ((((int16)buff[3]) << 8) | buff[2]) - a_offset[1];
	result[2] = ((((int16)buff[5]) << 8) | buff[4]) - a_offset[2];
}

Brain uavBrain;


void setup() {
	/* Set up the LED to blink  */
	pinMode(BOARD_LED_PIN, OUTPUT);

	/* Send a message out USART2  */
	Serial3.begin(57600);
	Serial3.println("Hello world!");

	 i2c_master_enable(I2C1, I2C_FAST_MODE);

	initAcc();

}

void loop() {


	toggleLED();

	int16 accData[3];
	getAccelerometerData(accData);

	char str[60];
	sprintf(str, "accX: %d | accY: %d | tick: %lu", accData[0], accData[1], uavBrain.getTickId());
	Serial3.println(str);

	uavBrain.loop();

	delay(500);
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
