#ifndef UART_H
#define UART_H

#define MC_RECVBUFFER_SIZE 5
#define MC_SENDBUFFER_SIZE 5

#define BAUDRATE 115200

/*
If we have issues with the baud rate try defining DOUBLE_UART_RATE
*/

#ifdef DOUBLE_UART_RATE
    #define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 8UL))) - 1)
#else
    #define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)
#endif

void uartInit(void);
void uartSendRaw(char* string);
void uartSendCommand(uint8_t command, int16_t data);
char* uartReadRaw(void);

#endif
