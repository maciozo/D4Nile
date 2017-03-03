#include "uart.h"
#include "../constants.h"

void uartInit(void)
{
    Serial.begin(UART_BAUD);
    pinMode(D0, INPUT);
    pinMode(D1, OUTPUT);
    return;
}

void uartSendRaw(char* string, unsigned int length)
{
    Serial.write(string, length);
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
    
    uartSendRaw(toSend, MC_SENDBUFFER_SIZE);
    
    return;
}

char* uartReadRaw(int length)
{
    char *readBuffer;
    readBuffer = names = (char*)malloc(length * sizeof(char));
    Serial.readBytesUntil('\0', readBuffer, length);
    return readBuffer;
}
