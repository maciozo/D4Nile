#RPi Notes

Please use Raspbian Jessie Lite. A newer one, since I'm not sure when they implemented `enable_uart`

##RPi3 Specific

- Since the standard PWM is taken by the Bluetooth hardware, we have to use mini PWM. Baud rate derived from core clock. Limitations described in http://raspberrypi.stackexchange.com/a/45571
- `setup.sh` carries out the tasks below

 - /boot/config.txt
 Add these lines to the file
 ```
 enable_uart=1 # Locks core frequency
 gpu_mem=16 # Not really needed, but why not?
 ```

 - /boot/cmdline.txt
Remove `console=serial0,115200`

 - Disable serial terminal service
 ```
 sudo systemctl stop serial-getty@ttyS0.service
 sudo systemctl disable serial-getty@ttyS0.service
 ```

- Serial I/O
 - To set baud rate
 `stty -F /dev/serial0 <rate>`
  - 115200 should be fine
  
 - Reading from terminal
 `cat < /dev/serial0`

 - Writing from terminal
 `echo "Hello, world!" > /dev/serial0`

- mini UART pinout available on https://pinout.xyz/pinout/uart
 - Using pin 6 as GND