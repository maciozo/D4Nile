#include "uart.h"
#include "../constants.h"

#include <stdint.h>
#include <stdlib.h>

void uartInit(void)
{
    PRR &= ~PRUSART0; /* Disable USART power reduction */
    
    #ifdef DOUBLE_UART_RATE
        UCSR0A |= U2X0
    #endif
    
    UBRR0H = (uint8_t)(BAUD_PRESCALLER >> 8);
    UBRR0L = (uint8_t)(BAUD_PRESCALLER);
    
    UCSR0B = RXEN0 | TXEN0; /* Enable USART RX/TX */
    
    /* Set to asynchronous */
    UCSR0C &= ~UMSEL01;
    UCSR0C &= ~UMSEL00;
    
    /* Disabling parity, since the RPi3 doesn't support it */
    USCR0C &= ~UPM01;
    USCR0C &= ~UPM00;
    
    /* 1 stop bit. */
    USCR0C &= ~USBS0;
    
    /* 8 bit character size */
    USCR0C &= ~UCSZ02;
    USCR0C |= UCSZ01;
    USCR0C |= UCSZ00;
    
    return;
}

void uartSendRaw(char* string, unsigned int length)
{
    unsigned int i;
    
    for (i = 0; i < length; i++)
    {
        /* Wait until the transmit buffer is populated. UDREn goes high when data register is empty. */
        while (!(UCSR0A & UDRE0));
        
        /* Put character in to transmission buffer */
        UDR0 = string[i];
    }
    
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

void uartReadRaw(unsigned int length, char* str)
{
    unsigned int i;
    
    for (i = 0; i < length; i++)
    {
        /* Wait until USART receive complete. RXCn goes high. */
        while (!(UCSR0A & RXC0));
        
        /* Store character from transmission buffer */
        str[i] = UDR0;
    }
}
