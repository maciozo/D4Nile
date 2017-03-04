#include "commandData.h"
#include "constants.h"

int formatData(commanddata_t* commandData, char* rawdata)
{
    int16_t tempData = (rawdata[1] << 8) | rawdata[2];
    switch (rawdata[0])
    {
        case YAW_CCW:
            commandData->yaw_ccw = tempData;
            break;
        case THROTTLE_UP:
            commandData->throttle_up = tempData;
            break;
        case ROLL_LEFT:
            commandData->roll_left = tempData;
            break;
            
        case PITCH_FORWARD:
            commandData->pitch_forward = tempData;
            break;
        case MODE_BUTTON:
            commandData->mode_button = tempData;
            break;
        case SERVO_BUTTON:
            commandData->servo_button = tempData;
            break;
        default:
            return 1;
    }
    return 0;
}
