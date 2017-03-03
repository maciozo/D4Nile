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

#include <stdint.h>

/* ========================================================================= */

/* Initialise the USART hardware */
void uartInit(void);

/* Send a string over UART.
    char* string: string to be sent
    unsigned int length: length of the string, including '\0'
*/
void uartSendRaw(char* string, unsigned int length);

/* Send telemetry data over UART.
    uint8_t command: What the data is describing. Refer to constants.h
    int16_t data: The payload to send
*/
void uartSendCommand(uint8_t command, int16_t data);

/* Read a string from UART
    char* string: Pointer to the received string to be placed in
    unsigned int length: Length of the string to read, including '\0'
*/
void uartReadRaw(char* string, unsigned int length);

#endif
