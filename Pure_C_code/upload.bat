avr-gcc -mmcu=atmega328p -DF_CPU=16000000 -Wall -Os seeeduino-motor-test.c -o seeeduino-motor-test.elf

avr-objcopy -O ihex seeeduino-motor-test.elf seeeduino-motor-test.hex
pause
avrdude -v -patmega328p -carduino -PCOM4 -b115200 -D -Uflash:w:seeeduino-motor-test.hex:i 
pause