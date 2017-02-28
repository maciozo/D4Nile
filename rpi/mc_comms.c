#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <time.h>

#include "mc_comms.h"
#include "../constants.h"

FILE* uartInit(const char* device)
{
    FILE *serial = fopen(device, "r+");
    
    /* Making fgets non-blocking 
    http://stackoverflow.com/a/6055774 */
    int fd = fileno(serial);
    flags = fcntl(fd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    fcntl(fd, F_SETFL, flags);
    
    /* fclose(serial); */
    
    return serial;
}

int uartSendRaw(char* string, FILE* uartDevice)
{
    if (strlen(string) > SENDBUFFER_SIZE)
    {
        return 1;
    }
    fwrite(sendBuffer, sizeof(char), SENDBUFFER_SIZE, uartDevice);
    return 0;
}

int uartSendCommand(uint8_t command, int16_t data, FILE* uartDevice)
{
    char toSend[4];
    if ((command < 0x20) | (command > CTRL_MAX))
    {
        return 1;
    }
    
    toSend[0] = (uint8_t)command;
    toSend[1] = (int8_t)(data >> 8);
    toSend[2] = (int8_t)(data & 0x00FF);
    
    uartSendRaw(toSend, uartDevice);
    
    return 0;
}

char* uartReadRaw(FILE* uartDevice)
{    
    char recvBuffer[RECVBUFFER_SIZE];
    if (fgets(recvBuffer, RECVBUFFER_SIZE, uartDevice) != NULL)
    {
        return recvBuffer;
    }
    return NULL;
}
