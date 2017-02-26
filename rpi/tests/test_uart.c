#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

/* http://stackoverflow.com/questions/10362111/linux-programming-write-to-device-file */
int main(void)
{
    FILE *serialDev;
    int i;
    char sendBuffer[80] = "";
    char recvBuffer[80] = "";
    
    printf("Text to send: ");
    scanf("%s", sendBuffer);
    
    serialDev = fopen("/dev/serial0", "r+");
    
    for (i = 0; i < 10; i++)
    {
        fprintf(serialDev, "%s%d\r\n", sendBuffer, i);
        /* perror("perror output:"); */
    }
    
    fprintf(serialDev, "Please type something ^_^ (type stop to close)\n");
    
    while (1)
    {
        fgets(recvBuffer, 80, serialDev);
        /* perror("perror output:"); */
        if (!strcmp(recvBuffer, "stop\n"))
        {
            printf("stopping\n");
            break;
        }
        printf("Received: %s", recvBuffer);
    }

    fclose(serialDev);
    
    return 0;
}