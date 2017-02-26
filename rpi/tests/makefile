CC=gcc
CFLAGS=-I.
ODIR=../bin/tests

test_uart: test_uart.o
	$(CC) -o $(ODIR)/test_uart test_uart.o $(CFLAGS)
	
.PHONY: clean

clean:
	rm -f test_uart.o
	rm -f $(ODIR)/test_uart