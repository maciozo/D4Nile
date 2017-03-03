#include "uart.h"
#include "../constants.h"

void uartInit(void)
{
    Serial.begin(UART_BAUD);
    pinMode(D0, INPUT);
    pinMode(D1, OUTPUT);
    return;
}

void uartSendRaw(char* string)
{
    Serial.write(string, MC_SENDBUFFER_SIZE);
    return;
}

void uartSendCommand(uint8_t command, int16_t data)
{
    char toSend[MC_SENDBUFFER_SIZE];
    
    toSend[0] = (char)command;
    toSend[1] = (char)(data >> 8);
    toSend[2] = (char)(data);
    toSend[3] = '\n';
    toSend[4] = '\0';
    
    uartSendRaw(toSend);
    
    return;
}

char* uartReadRaw(void)
{
    char readBuffer[MC_RECVBUFFER_SIZE];
    Serial.readBytesUntil('\0', readBuffer, MC_READBUFFER_SIZE);
    return readBuffer;
}
