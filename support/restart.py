#!/usr/bin/env python

import serial
import os
import platform
import sys
import time
from struct import pack

print 'start script'

try:
    ser = serial.Serial('/dev/ttyUSB0', baudrate=115200, xonxoff=1)

    try:
        # try to toggle DTR/RTS (old scheme)
        ser.setRTS(0)
        time.sleep(0.01)
        ser.setDTR(0)
        time.sleep(0.01)
        ser.setDTR(1)
        time.sleep(0.01)
        ser.setDTR(0)

        # try magic number
        ser.setRTS(1)
        time.sleep(0.01)
        ser.setDTR(1)
        time.sleep(0.01)
        ser.setDTR(0)
        time.sleep(0.01)
        ser.write("1EAF")

    finally:
        # ok we're done here
        ser.close()

except Exception as e:
    print 'Failed to open serial port ttyUSB* for reset'
    sys.exit()
