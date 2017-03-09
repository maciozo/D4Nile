CC=avr-gcc
CFLAGS=-I. -I../commandData/ -DF_CPU=16000000 -mmcu=atmega328p -Os

OC=avr-objcopy
OCFLAGS=-O ihex

PG=avrdude
PGFLAGS=-v -patmega328p -carduino -PCOM7 -b115200 -D

uart_test: uart_test.o uart.o ../commandData/commandData.o ../../../constants.h uart.h ../commandData/commandData.h
	$(CC) -o uart_test.elf uart_test.o uart.o ../commandData/commandData.o $(CFLAGS)
	$(OC) $(OCFLAGS) uart_test.elf uart_test.hex
	$(PG) $(PGFLAGS) -Uflash:w:uart_test.hex:i

.PHONY: clean

clean:
	rm -f uart_test.o
	rm -f uart.o
	rm -f commandData.o
	rm -f uart_test.elf 
	rm -f uart_test.hex
