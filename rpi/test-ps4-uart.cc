// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Copyright Drew Noakes 2013-2016
//
// File first modified by Charlie Mason 25/02/2017

#include <unistd.h>

#include "joystick.hh"
#include "mc_comms.hh"
#include "../constants.h"

// Negative values correspond to the direction in variable names (positive for opposite direction)
int16_t servoButton = 0; //Turn on/off electromagnet
int16_t modeButton = 0; //Acts as mode switch between joystick and motion control methods
int16_t throttleUp = 0; // Axis 1
int16_t yawCCW = 0;       // Axis 0
int16_t pitchForward = 0;  // Axis 5 (Joystick) Axis 13 (Motion)
int16_t rollLeft = 0;      // Axis 2 (Joystick) Axis 11 (Motion)
int16_t pidDec = 0;        // L1
int16_t pidInc = 0;        // R1
int16_t STAAAHP = 0;       // Touchpad button

int main(int argc, char** argv)
{
    const char uartDevice[] = "/dev/serial0";
    FILE* serialDevice = uartInit(uartDevice);
    
    sleep(10);

    // Create an instance of Joystick
    Joystick joystick("/dev/input/js0");

    // Ensure that it was found and that we can use it
    if (joystick.isFound())
    {
        printf("Joystick Connected.\n");
    }
    else
    {
        printf("Joystick not detected, exiting.\n");
        exit(1);
    }

    while (true)
    {
        // Restrict rate
        usleep(1000);
        //uartSendCommand(serialDevice, MODE_BUTTON, modeButton);
        //uartSendCommand(serialDevice, SERVO_BUTTON, servoButton);
        //uartSendCommand(serialDevice, THROTTLE_UP, throttleUp);
        //uartSendCommand(serialDevice, PITCH_FORWARD, rollLeft);
        //uartSendCommand(serialDevice, ROLL_LEFT, pitchForward);
        //uartSendCommand(serialDevice, YAW_CCW, yawCCW);

        // Attempt to sample an event from the joystick
        JoystickEvent event;
        if (joystick.sample(&event))
        {
            if (event.isButton())
            {
                if (event.number == 1)
                {
                    modeButton = event.value;
                    printf("Button %u is %s\n", event.number, modeButton == 0 ? "up" : "down");
                    // uartSendCommand(serialDevice, MODE_BUTTON, modeButton);

                    if (event.value == 0)
                    {
                        pitchForward = 0;
                        rollLeft = 0;
                        printf("Pitch and Roll reset, now in JOYSTICK mode\n");
                    }
                }
                else if (event.number == 0)
                {
                    servoButton = event.value;
                    printf("Button %u is %s\n", event.number, servoButton == 0 ? "up" : "down");
                    uartSendCommand(serialDevice, SERVO_BUTTON, servoButton);
                }
                else if (event.number == 4)
                {
                    pidDec = event.value;
                    printf("pidDec is %s\n", event.value == 0 ? "up" : "down");
                    uartSendCommand(serialDevice, PID_DEC, pidDec);
                }
                else if (event.number == 5)
                {
                    pidInc = event.value;
                    printf("pidInc is %s\n", event.value == 0 ? "up" : "down");
                    uartSendCommand(serialDevice, PID_INC, pidInc);
                }
                else if (event.number == 13)
                {
                    STAAAHP = event.value;
                    printf("STAAAHP is %s\n", event.value == 0 ? "up" : "down");
                    uartSendCommand(serialDevice, STAHP, STAAAHP);
                }
            }

            else if (event.isAxis())
            {
                if (modeButton && (event.number > 5 || event.number == 1))
                {
                    switch(event.number)
                    {
                    case 1 :
                        throttleUp = -event.value;
                        uartSendCommand(serialDevice, THROTTLE_UP, throttleUp);
                        break;
                    case 11:
                        rollLeft = -event.value;
                        uartSendCommand(serialDevice, PITCH_FORWARD, rollLeft); //Roll & Pitch axis swapped
                        break;
                    case 13:
                        pitchForward = event.value;
                        uartSendCommand(serialDevice, ROLL_LEFT, pitchForward);
                        break;
                    }
                    printf("MOTION Throttle: %6d, Roll: %6d, Pitch: %6d\n", throttleUp, rollLeft, pitchForward);
                }
                else if (modeButton == 0 && event.number <= 5)
                {
                    switch(event.number)
                    {
                    case 0 :
                        yawCCW = -event.value;
                        uartSendCommand(serialDevice, YAW_CCW, yawCCW);
                        break;
                    case 1 :
                        throttleUp = -event.value;
                        uartSendCommand(serialDevice, THROTTLE_UP, throttleUp);
                        break;
                    case 2 :
                        rollLeft = event.value;
                        uartSendCommand(serialDevice, PITCH_FORWARD, rollLeft);
                        break;
                    case 5 :
                        pitchForward = -event.value;
                        uartSendCommand(serialDevice, ROLL_LEFT, pitchForward);
                        break;
                    }
                printf("JOYSTICK Throttle: %6d, Yaw: %6d, Roll: %6d, Pitch: %6d\n", throttleUp, yawCCW, rollLeft, pitchForward);
                }
            }
        }
    }
}
