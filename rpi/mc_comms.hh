#ifndef _MC_COMMS_H
#define _MC_COMMS_H

#include <stdint.h>

#define SERIAL_DEVICE "/dev/serial0"
#define RECVBUFFER_SIZE 5
#define SENDBUFFER_SIZE 5

FILE* uartInit(const char* device);
void uartClose(FILE* uartDevice);
char* uartReadRaw(FILE* uartDevice);
int uartSendRaw(char* string, FILE* uartDevice);
int uartSendCommand(uint8_t command, int16_t data, FILE* uartDevice);

#endif
