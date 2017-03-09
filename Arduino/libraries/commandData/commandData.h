#ifndef _CD_H
#define _CD_H

#include "constants.h"
#include <stdint.h>

typedef struct
{
    double yaw_ccw;
    double throttle_up;
    double roll_left;
    double pitch_forward;
    double mode_button;
    double servo_button;
    double autoland;
    #ifdef GAIN_TUNING
        double roll_kp;
        double roll_ki;
        double roll_kd;
        double pitch_kp;
        double pitch_ki;
        double pitch_kd;
        double yaw_kp;
        double yaw_ki;
        double yaw_kd;
    #endif
} commanddata_t;

int formatData(commanddata_t* commandData, char* rawdata);

#endif
