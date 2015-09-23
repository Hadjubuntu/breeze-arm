/*
 * I2C.cpp
 *
 *  Created on: Sep 23, 2015
 *      Author: adrien
 */

#include "libmaple/i2c.h"
#include "I2C.h"


int I2C::writeTo(uint8 addr, uint8 val)
{
	// all i2c transactions send and receive arrays of i2c_msg objects
	i2c_msg msgs[1]; // we dont do any bursting here, so we only need one i2c_msg object
	uint8 msg_data[2] = {addr,val};

	msgs[0].addr = _deviceAddr;
	msgs[0].flags = 0; // write
	msgs[0].length = 2; //Write two data
	msgs[0].data = msg_data;
	int res = i2c_master_xfer(I2C1, msgs, 1,0);  //

	return res;
}

int I2C::readFrom(uint8 addr, uint8 nbBytes, uint8 *msgData)
{
	i2c_msg msgs[1];
	msgData[0] = addr;

	msgs[0].addr = _deviceAddr;
	msgs[0].flags = 0; //write flag is 0
	msgs[0].length = 1; // just one byte for the address to read, 0x00
	msgs[0].data = msgData;
	i2c_master_xfer(I2C1, msgs, 1,0);

	msgs[0].addr = _deviceAddr;
	msgs[0].flags = I2C_MSG_READ; //read
	msgs[0].length = nbBytes; // Read the number of bytes
	msgs[0].data = msgData;
	int res = i2c_master_xfer(I2C1, msgs, 1,0);

	return res;
}

