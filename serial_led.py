#!/usr/bin/env python

# Serial LED "client" for Python
# Send commands to the Teensyduino "server" via USB serial.
# Usage: ./serial_led.py [signal_name] [light_num]
#    ex: ./serial_led.py ok 0

from sys import argv, exit
import serial
from serial.tools import list_ports

# If this doesn't work, try `lsusb | grep -i teensy` to find the right ID
device_name = 'USB VID:PID=16c0:0483 SNR=12345'

for port in list(list_ports.comports()):
    if port[2] == device_name:
        ser = serial.Serial(port[0], 38400, timeout=1)
        break

signals = {
    "ok": 0,
    "warn": 1,
    "crit": 2,
    "off": 3,
    "unknown": 4
}

try:
    status = signals[argv[1]]
except:
    status = signals["unknown"]

# If you change light_offset here, change it in the Arduino code too
light_offset = 10
try:
    light = light_offset * int(argv[2])
except:
    light = 0

if ser is not None:
    ser.write([light + status])
    ser.close()
    exit(0)
else:
    exit(1)
