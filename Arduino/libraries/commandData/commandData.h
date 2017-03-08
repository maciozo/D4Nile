#ifndef _CD_H
#define _CD_H

#include <stdint.h>

typedef struct
{
  float yaw_ccw;
  float throttle_up;
  float roll_left;
  float pitch_forward;
  float mode_button;
  float servo_button;
  float autoland;
} commanddata_t;

int formatData(commanddata_t* commandData, char* rawdata);

#endif
