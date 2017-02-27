#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>

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
    int i;
    int mmapfd;
    long mmapPageSize = sysconf(_SC_PAGESIZE);
    char *mmapData;
    
    mmapfd = open("commsData", O_RDWR);
    pagesize = getpagesize();
    mmapData = mmap((caddr_t)0, pagesize, (PROT_READ | PROT_WRITE), MAP_SHARED, mmapfd, 0);
    
    if (mmapData == MAP_FAILED)
    {
        perror("mmap");
    }
    
    while (1)
    {
        /* TODO:
            Check for data to send
            Send data
        */
        
        if (fgets(recvBuffer, RECVBUFFER_SIZE, uartDevice))
        {
            /* TODO: Format data in to shared memory */
            
        }
        
        
    }
}
