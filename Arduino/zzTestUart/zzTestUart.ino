#include "uart.h"
#include "commandData.h"

#include <stdio.h>

void setup()
{
    char string[9]= "testing!";
  //  char[9] string = "testing!";
    uartInit();
    uartSendRaw(string, 9);
}

void loop()
{
    char recvBuffer[5];
    commanddata_t commandData = {0};
    while (1)
    {
        dumpCommandData(&commandData);
        uartReadRaw(recvBuffer, 5);
        formatData(&commandData, recvBuffer);
    }
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
    
    uartSendRaw(newLine, 2);
    
    return;
}
