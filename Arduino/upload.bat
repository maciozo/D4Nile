avr-gcc -mmcu=atmega328p -DF_CPU=16000000 -Wall -Os main.c sonar.c sonar.h uart.c uart.h ir.c ir.h i2c.c i2c.h MPU9250_reg.h -lm -o main.elf

avr-objcopy -O ihex main.elf main.hex
pause
avrdude -v -patmega328p -carduino -PCOM3 -b115200 -D -Uflash:w:main.hex:i 
pause