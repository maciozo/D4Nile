#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <time.h>
#include <string.h>

#include "mc_comms.hh"
#include "../constants.h"

FILE* uartInit(const char* device)
{
    static FILE *serial = fopen(device, "r+");
    
    /* Making fgets non-blocking 
    http://stackoverflow.com/a/6055774 */
    //int fd = fileno(serial);
    //int flags = fcntl(fd, F_GETFL, 0);
    //flags |= O_NONBLOCK;
    //fcntl(fd, F_SETFL, flags);
    
    /* fclose(serial); */
    
    return serial;
}

void uartClose(FILE* uartDevice)
{
    fclose(uartDevice);
}

int uartSendRaw(char* string, FILE* uartDevice)
{
    if (strlen(string) > SENDBUFFER_SIZE)
    {
        return 1;
    }
    fwrite(string, sizeof(char), SENDBUFFER_SIZE, uartDevice);
    return 0;
}

int uartSendCommand(uint8_t command, int16_t data, FILE* uartDevice)
{
    char toSend[5];
    if ((command < 0x20) | (command > RECV_MAX))
    {
        return 1;
    }
    
    /*
        Convert 8 bit command and 16 bit data in to a 24 bit string.
        8 bit command, 2x 8 bit data.
        CCCCCCCC|DDDDDDDD|DDDDDDDD
    */
    toSend[0] = (char)command;
    toSend[1] = (char)(data >> 8);
    toSend[2] = (char)(data & 0x00FF);
    toSend[3] = (char)'\n';
    toSend[4] = (char)'\0';
    
    uartSendRaw(toSend, uartDevice);
    
    return 0;
}

int uartReadRaw(FILE* uartDevice, char* recvBuffer)
{    
    if (fgets(recvBuffer, RECVBUFFER_SIZE, uartDevice) != NULL)
    {
        return 0;
    }
    return 1;
}