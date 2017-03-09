#include "uart.h"
#include "../commandData/commandData.h"

#include <stdio.h>

int main(void)
{
    char recvBuffer[5];
    commanddata_t commandData = {0};
    uartInit();
    while (1)
    {
        dumpCommandData(&commandData);
        uartReadRaw(recvBuffer, 5);
        formatData(&commandData, recvBuffer);
    }
    return 0;
}

void dumpCommandData(commanddata_t* commandData)
{
    char toSend[32];
    char newLine[2] = "\n";
    
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
    
    sprintf(toSend, "AUTOLAND: %d\n", commandData->autoland);
    uartSendRaw(toSend, 32);
    
    uartSendRaw(newLine, 2);
    
    return;
}
