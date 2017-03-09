#include "../../constants.h"
#include "../mc_comms.hh"

#include <stdio.h>
#include <pthread.h>

struct arg_struct
{
    FILE* serialDevice;
    int stop;
};

void* readLoop(void* arguments)
{
    printf("readLoop started\n");
    struct arg_struct *args = arguments;
    char recvBuffer[64] = "";
    while (!(args->stop))
    {
        uartReadRaw(args->serialDevice, recvBuffer, 64);
        printf("RAW --> %s\n", recvBuffer);
    }
    return NULL;
}

int main()
{
    char file[] = "/dev/serial0";
    pthread_t readThread;
    struct arg_struct args;
    
    FILE* serialDevice = uartInit(file);
    
    args.serialDevice = serialDevice;
    args.stop = 0;
    
    if (pthread_create(&readThread, NULL, &readLoop, (void *)&args) != 0) 
    {
        printf("Fuck!\n");
        return -1;
    }
    
    uartSendCommand(serialDevice, YAW_CCW, (uint8_t)10);
    uartSendCommand(serialDevice, ROLL_LEFT, (uint8_t)5);
    uartSendCommand(serialDevice, THROTTLE_UP, (uint8_t)99);
    uartSendCommand(serialDevice, PITCH_FORWARD, (uint8_t)24);
    uartSendCommand(serialDevice, MODE_BUTTON, (uint8_t)1);
    uartSendCommand(serialDevice, SERVO_BUTTON, (uint8_t)1);
    uartSendCommand(serialDevice, AUTOLAND, (uint8_t)1);
    
    uartSendCommand(serialDevice, YAW_CCW, (uint8_t)54);
    uartSendCommand(serialDevice, ROLL_LEFT, (uint8_t)22);
    uartSendCommand(serialDevice, THROTTLE_UP, (uint8_t)95);
    uartSendCommand(serialDevice, PITCH_FORWARD, (uint8_t)51);
    uartSendCommand(serialDevice, MODE_BUTTON, (uint8_t)0);
    uartSendCommand(serialDevice, SERVO_BUTTON, (uint8_t)0);
    uartSendCommand(serialDevice, AUTOLAND, (uint8_t)0);
    
    args.stop = 1;
    return pthread_join(readThread, NULL);
}
