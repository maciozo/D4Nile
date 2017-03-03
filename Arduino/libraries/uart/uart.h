#ifndef UART_H
#define UART_H

#define MC_RECVBUFFER_SIZE 5
#define MC_SENDBUFFER_SIZE 5

void uartInit(void);
void uartSendRaw(char* string);
void uartSendCommand(uint8_t command, int16_t data);
char* uartReadRaw(void);

#endif
