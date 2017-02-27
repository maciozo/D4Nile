#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <time.h>

#include "mc_comms.h"
#include "../constants.h"

recvBuffer = "";
sendBuffer = "":
sensorData = {0};

int uartInit(const char* device)
{
    FILE *serial = fopen(device, "r+");
    
    /* Making fgets non-blocking 
    http://stackoverflow.com/a/6055774 */
    int fd = fileno(serial);
    flags = fcntl(fd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    fcntl(fd, F_SETFL, flags);
    
    uartLoop(serial);
    fclose(serial);
    
    return 0;
}

int uartLoop(FILE *uartDevice)
{
    int i;
    int mmapfd;
    long mmapPageSize = sysconf(_SC_PAGESIZE);
    char *mmapData;
    struct timespec tim, tim2;
    sensordata_t sensorData = {0};
    int16_t tempData = 0;
    
    /* timespec for 1ms */
    tim.tv_sec = 0;
    tim.tv_nsec = 1000000L;
    
    mmapfd = open("commsData", O_RDWR);
    pagesize = getpagesize();
    mmapData = mmap((caddr_t)0, pagesize, (PROT_READ | PROT_WRITE), MAP_SHARED, mmapfd, 0);
    
    if (mmapData == MAP_FAILED)
    {
        perror("mmap");
    }
    
    while (1)
    {
        if (sendBuffer[0] != '\0') /* Checking if there's anything to send */
        {
            fwrite(sendBuffer, sizeof(char), SENDBUFFER_SIZE, uartDevice);
        }
        
        if (fgets(recvBuffer, RECVBUFFER_SIZE, uartDevice) != NULL)
        {
            formatData(&sensorData, recvBuffer);
        }
        
        nanosleep(&tim, &tim2);
    }
    return 0;
}

int uartSendRaw(char* string)
{
    if (strlen(string) > SENDBUFFER_SIZE)
    {
        return 1;
    }
    sendBuffer = string;
    return 0;
}

int uartSendCommand(uint8_t command, int16_t data)
{
    char toSend[4];
    if ((command < 0x20) | (command > CTRL_MAX))
    {
        return 1;
    }
    
    toSend[0] = (uint8_t)command;
    toSend[1] = (int8_t)(data >> 8);
    toSend[2] = (int8_t)(data & 0x00FF);
    
    uartSendRaw((char*)toSend);
    
    return 0;
}

char* uartReadRaw(void)
{    
    return recvBuffer;
}

sensordata_t* getSensorData(void)
{
    return &sensorData
}

void formatData(sensordata_t* sensorData, char* rawdata)
{
    tempData = (rawdata[1] << 8) | rawdata[2];
    switch (rawdata[0])
    {
        case ACC_X:
            sensorData->acc_x = tempData;
            break;
        case ACC_Y:
            sensorData->acc_y = tempData;
            break;
        case ACC_Z:
            sensorData->acc_z = tempData;
            break;
            
        case GYR_X:
            sensorData->gyr_x = tempData;
            break;
        case GYR_Y:
            sensorData->gyr_y = tempData;
            break;
        case GYR_Z:
            sensorData->gyr_z = tempData;
            break;
            
        case MAG_X:
            sensorData->mag_x = tempData;
            break;
        case MAG_Y:
            sensorData->mag_y = tempData;
            break;
        case MAG_Z:
            sensorData->mag_z = tempData;
            break;
        
        case IR:
            sensorData->ir = tempData;
            break;
        case USONIC:
            sensorData->usonic = tempData;
        
        case TEMP_BAT:
            sensorData->temp_bat = tempData;
            break;
        case TEMP_M0:
            sensorData->temp_m0 = tempData;
            break;
        case TEMP_M1:
            sensorData->temp_m1 = tempData;
            break;
        case TEMP_M2:
            sensorData->temp_m2 = tempData;
            break;
        case TEMP_M3:
            sensorData->temp_m3 = tempData;
            break;
        case TEMP_AIR:
            sensorData->temp_air = tempData;
            break;
            
        case GPS_X:
            sensorData->gps_n = tempData;
            break;
        case GPS_Y:
            sensorData->gps_e = tempData;
            break;
        case GPS_Z:
            sensorData->gps_a = tempData;
            break;
            
        default:
            printf("Unknown control byte: 0x%x\n", rawdata[0]);
            printf("\tPayload: 0x%x 0x%x\n", rawdata[1], rawdata[2]);
    }
}
