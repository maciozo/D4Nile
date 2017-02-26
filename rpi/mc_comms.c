#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <fcntl.h>

#include "mc_comms.h"
#include "../constants.h"

int main(void)
{
    FILE *serial = fopen(SERIAL_DEVICE, "r+");
    
    /* Making fgets non-blocking 
    http://stackoverflow.com/a/6055774 */
    int fd = fileno(serial);
    flags = fcntl(fd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    fcntl(fd, F_SETFL, flags);
    
    uartLoop(serial);
    fclose(serial);
}

int uartLoop(FILE *uartDevice)
{
    char recvBuffer[RECVBUFFER_SIZE] = "";
    while (1)
    {
        if (fgets(recvBuffer, RECVBUFFER_SIZE, uartDevice))
        {
            /* TODO: Format data in to shared memory */
        }
        
        /* TODO:
            Check for data to send
            Send data
        */
    }
}
