#ifndef _MC_COMMS_H
#define _MC_COMMS_H

#define SERIAL_DEVICE "/dev/serial0"
#define RECVBUFFER_SIZE 64
#define SENDBUFFER_SIZE 64

int uartInit(char* device);
void uartClose(FILE* uartDevice);
char* uartReadRaw(FILE* uartDevice);
int uartSendRaw(char* string, FILE* uartDevice);
int uartSendCommand(uint8_t command, int16_t data, FILE* uartDevice);

#endif
