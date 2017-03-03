#include "uart.h"
#include "commandData.h"

#include <stdio.h>

void setup()
{
    uartInit();
    uartSendRaw("testing!", 9)
}

void loop()
{
    dumpCommandData(*commandData);
    recvBuffer = uartReadRaw(5);
    formatData(*commandData, recvBuffer);
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
