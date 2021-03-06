#include "uart.h"
#include "constants.h"

#include <stdint.h>
#include <stdlib.h>
#include <avr/io.h>

void uartInit(void)
{
    PRR &= ~(1 << PRUSART0); /* Disable USART power reduction */
    
    #ifdef DOUBLE_UART_RATE
        UCSR0A |= (1 << U2X0);
    #endif
    
    UBRR0H = (uint8_t)(BAUD_PRESCALLER >> 8);
    UBRR0L = (uint8_t)(BAUD_PRESCALLER);
    
    UCSR0B = (1 << RXEN0) | (1 << TXEN0); /* Enable USART RX/TX */
    
    /* Set to asynchronous */
    UCSR0C &= ~(1 << UMSEL01);
    UCSR0C &= ~(1 << UMSEL00);
    
    /* Disabling parity, since the RPi3 doesn't support it */
    UCSR0C &= ~(1 << UPM01);
    UCSR0C &= ~(1 << UPM00);
    
    /* 1 stop bit. */
    UCSR0C &= ~(1 << USBS0);
    
    /* 8 bit character size */
    UCSR0C &= ~(1 << UCSZ02);
    UCSR0C |= (1 << UCSZ01);
    UCSR0C |= (1 << UCSZ00);
    
    /* LED for debugging */
    DDRD |= (1 << DDD7); /* Set D7 to write */
    PORTD &= ~(1 << PORTD7); /* Turn LED off */
    
    return;
}

void uartSendRaw(unsigned char* string, unsigned int length)
{
    unsigned int i;
    
    for (i = 0; i < length; i++)
    {
        /* Wait until the transmit buffer is populated. UDREn goes high when data register is empty. */
        while (!(UCSR0A & (1 << UDRE0)));
        
        /* Put character in to transmission buffer */
        UDR0 = string[i];
    }
    
    return;
}

void uartSendRaw(char* string, unsigned int length)
{
    unsigned int i;
    
    for (i = 0; i < length; i++)
    {
        /* Wait until the transmit buffer is populated. UDREn goes high when data register is empty. */
        while (!(UCSR0A & (1 << UDRE0)));
        
        /* Put character in to transmission buffer */
        UDR0 = string[i];
    }
    
    return;
}

void uartSendCommand(char command, int16_t data)
{
    // unsigned char toSend[MC_SENDBUFFER_SIZE + 2];
    unsigned char toSend[MC_SENDBUFFER_SIZE];
    
    // toSend[0] = (char) UART_START;
    // toSend[1] = (char) command;
    // toSend[2] = (char) (data >> 8);
    // toSend[3] = (char) (data & 0xFF);
    // toSend[4] = (char) UART_STOP;
    
    toSend[0] = (char) command;
    toSend[1] = (char) (data >> 8);
    toSend[2] = (char) (data & 0xFF);
    
    // uartSendRaw(toSend, MC_SENDBUFFER_SIZE + 2);
    uartSendRaw(toSend, MC_SENDBUFFER_SIZE);
    
    return;
}

void uartReadRaw(char* string, unsigned int length)
{
    unsigned int i;
    // while (UDR0 != UART_START);
    for (i = 0; i < length; i++)
    {
        /* Wait until USART receive complete. RXCn goes high. */
        // while (!(UCSR0A & (1 << RXC0)));
        
        /* Store character from transmission buffer */
        string[i] = UDR0;
    }
    // if (UDR0 != UART_STOP)
    // {
        // string[0] = 0x00;
        // string[1] = 0x00;
        // string[2] = 0x00;
    // }
    #ifdef ECHO
    uartSendRaw(string, MC_RECVBUFFER_SIZE);
    #endif
}
