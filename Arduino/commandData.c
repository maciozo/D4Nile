#include "sensorData.h"

int formatData(sensordata_t* sensorData, char* rawdata)
{
    tempData = (rawdata[1] << 8) | rawdata[2];
    switch (rawdata[0])
    {
        case YAW_CCW:
            sensorData->yaw_ccw = tempData;
            break;
        case THROTTLE_UP:
            sensorData->throttle_up = tempData;
            break;
        case ROLL_LEFT:
            sensorData->roll_left = tempData;
            break;
            
        case PITCH_FORWARD:
            sensorData->pitch_forward = tempData;
            break;
        case MODE_BUTTON:
            sensorData->mode_button = tempData;
            break;
        case SERVO_BUTTON:
            sensorData->servo_button = tempData;
            break;
        default:
            return 1;
    }
    return 0;
}
