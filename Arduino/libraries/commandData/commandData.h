#ifndef _CD_H
#define _CD_H

#include <stdint.h>

typedef struct
{
  int16_t yaw_ccw;
  int16_t throttle_up;
  int16_t roll_left;
  int16_t pitch_forward;
  int16_t mode_button;
  int16_t servo_button;
} commanddata_t;

int formatData(commanddata_t* commandData, char* rawdata);

#endif
