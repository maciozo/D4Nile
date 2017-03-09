#ifndef _CD_H
#define _CD_H


#include "constants.h"
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
    #ifdef GAIN_TUNING
        float roll_kp;
        float roll_ki;
        float roll_kd;
        float pitch_kp;
        float pitch_ki;
        float pitch_kd;
        float yaw_kp;
        float yaw_ki;
        float yaw_kd;
    #endif
} commanddata_t;

int formatData(commanddata_t* commandData, char* rawdata);

#endif
