#include "uart.h"
#include "commanddata.h"

#include <stdio>

int main(void)
{
    commanddata_t commandData = {0};
    char recvBuffer[MC_RECVBUFFER_SIZE];
    uartInit();
    uartSendRaw("testing!", 9)
    while (1)
    {
        dumpCommandData(*commandData);
        recvBuffer = uartReadRaw(5);
    }
    return 0;
}

void dumpCommandData(commanddata_t* commandData)
{
    char toSend[32];
    
    sprintf(toSend, "YAWCCW: %d\n", commandData->yaw_ccw);
    uartSendRaw(toSend, 32);
    
    sprintf(toSend, "THROTTLEUP: %d\n", commandData->throttle_up);
    uartSendRaw(toSend, 32);
    
    sprintf(toSend, "ROLLLEFT: %d\n", commandData->roll_left);
    uartSendRaw(toSend, 32);
    
    sprintf(toSend, "PITCHFORWARD: %d\n", commandData->pitch_forward);
    uartSendRaw(toSend, 32);
    
    sprintf(toSend, "MODEBUTTON: %d\n", commandData->mode_button);
    uartSendRaw(toSend, 32);
    
    sprintf(toSend, "SERVOBUTTON: %d\n", commandData->throttle_up);
    uartSendRaw(toSend, 32);
    
    uartSendRaw("\n", 2)
    
    return;
}
