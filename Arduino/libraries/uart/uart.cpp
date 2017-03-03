#include "uart.h"
#include "../constants.h"

void uartInit(void)
{
    PRR &= ~PRUSART0; /* Disable USART power reduction */
    UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8);
    UBRR0L = (uint8_t)(BAUD_PRESCALLER);
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
    UCSR0C = (3<<UCSZ00);
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
