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

int main(int argc, char** argv)
{
  const char uartDevice[] = "/dev/serial0";
  FILE* serialDevice = uartInit(uartDevice);
  char toSend[SENDBUFFER_SIZE] = "";

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
            uartSendCommand(MODE_BUTTON, modeButton, serialDevice);

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
            uartSendCommand(SERVO_BUTTON, servoButton, serialDevice);
          }
      }
      else if (event.isAxis())
      {
	if (modeButton && (event.number > 5 || event.number == 1))
	{
	  switch(event.number)
	  {
	    case 1 : throttleUp = event.value;
                     uartSendCommand(THROTTLE_UP, throttleUp, serialDevice);
		     break;
	    case 11: rollLeft = -event.value;
                     uartSendCommand(ROLL_LEFT, rollLeft, serialDevice);
		     break;
	    case 13: pitchForward = -event.value;
                     uartSendCommand(PITCH_FORWARD, pitchForward, serialDevice);
		     break;
	  }
	printf("MOTION Throttle: %6d, Roll: %6d, Pitch: %6d\n", throttleUp, rollLeft, pitchForward);
	}
	else if (modeButton == 0 && event.number <= 5)
	{
	  switch(event.number)
	  {
	    case 0 : yawCCW = event.value;
                     uartSendCommand(YAW_CCW, yawCCW, serialDevice);
		     break;
	    case 1 : throttleUp = event.value;
                     uartSendCommand(THROTTLE_UP, throttleUp, serialDevice);
		     break;
	    case 2 : rollLeft = event.value;
                     uartSendCommand(ROLL_LEFT, rollLeft, serialDevice);
		     break;
	    case 5 : pitchForward = event.value;
                     uartSendCommand(PITCH_FORWARD, pitchForward, serialDevice);
		     break;
	  }
	printf("JOYSTICK Throttle: %6d, Yaw: %6d, Roll: %6d, Pitch: %6d\n", throttleUp, yawCCW, rollLeft, pitchForward);
	}	
      }
    }
  }
}
