#include "commandData.h"
#include "constants.h"
#include <avr/io.h>

#include "uart.h"

int formatData(commanddata_t* commandData, char* rawdata)
{
    int16_t tempData = (rawdata[1] << 8) | (rawdata[2] & 0xFF);
    switch (rawdata[0])
    {
        case STAHP:
            if (tempData)
            {
                while(1)
                {
                    OCR0A = 62;
                    OCR0B = 62;
                    OCR2A = 62;
                    OCR2B = 62;
                }
            }
            break;
        case YAW_CCW:
            commandData->yaw_ccw = (tempData / 32767.0) * 90;
            break;
        case THROTTLE_UP:
            commandData->throttle_up = (tempData/327670.0) + 1;
            break;
        case ROLL_LEFT:
            commandData->roll_left = (tempData / 32767.0) * 10;
            break;
            
        case PITCH_FORWARD:
            commandData->pitch_forward = (tempData / 32767.0) * 10;
            break;
        case MODE_BUTTON:
            commandData->mode_button = rawdata[2];
            break;
        case SERVO_BUTTON:
            commandData->servo_button = rawdata[2] & 0xFF;
            break;
        case AUTOLAND:
            commandData->autoland = rawdata[2];
            break;
            
        case ROLL_KP:
            commandData->roll_kp = tempData;
            break;
            
        case ROLL_KI:
            commandData->roll_ki = tempData;
            break;
            
        case ROLL_KD:
            commandData->roll_kd = tempData;
            break;
            
        case PITCH_KP:
            commandData->pitch_kp = tempData;
            break;
            
        case PITCH_KI:
            commandData->pitch_ki = tempData;
            break;
            
        case PITCH_KD:
            commandData->pitch_kd = tempData;
            break;
            
        case YAW_KP:
            commandData->yaw_kp = tempData;
            break;
            
        case YAW_KI:
            commandData->yaw_ki = tempData;
            break;
            
        case YAW_KD:
            commandData->yaw_kd = tempData;
            break;
            
        default:
            return 1;
    }
    return 0;
}
