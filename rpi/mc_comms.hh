#ifndef _MC_COMMS_H
#define _MC_COMMS_H

#include <stdint.h>

#define SERIAL_DEVICE "/dev/serial0"
#define RECVBUFFER_SIZE 5
#define SENDBUFFER_SIZE 5

FILE* uartInit(const char* device);
void uartClose(FILE* uartDevice);
int uartReadRaw(FILE* uartDevice, char* recvBuffer);
int uartSendRaw(FILE* uartDevice, char* string);
int uartSendCommand(FILE* uartDevice, uint8_t command, int16_t data);

#endif
