#!/bin/bash

# Set up UART
stty -F /dev/serial0 115200
stty -F /dev/serial0 raw
bluetoothctl
/home/pi/D4Nile/rpi/test-ps4-uart



