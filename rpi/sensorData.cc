#include "sensorData.h"

int formatData(sensordata_t* sensorData, char* rawdata)
{
    tempData = (rawdata[1] << 8) | rawdata[2];
    switch (rawdata[0])
    {
        case ACC_X:
            sensorData->acc_x = tempData;
            break;
        case ACC_Y:
            sensorData->acc_y = tempData;
            break;
        case ACC_Z:
            sensorData->acc_z = tempData;
            break;
            
        case GYR_X:
            sensorData->gyr_x = tempData;
            break;
        case GYR_Y:
            sensorData->gyr_y = tempData;
            break;
        case GYR_Z:
            sensorData->gyr_z = tempData;
            break;
            
        case MAG_X:
            sensorData->mag_x = tempData;
            break;
        case MAG_Y:
            sensorData->mag_y = tempData;
            break;
        case MAG_Z:
            sensorData->mag_z = tempData;
            break;
        
        case IR:
            sensorData->ir = tempData;
            break;
        case USONIC:
            sensorData->usonic = tempData;
        
        case TEMP_BAT:
            sensorData->temp_bat = tempData;
            break;
        case TEMP_M0:
            sensorData->temp_m0 = tempData;
            break;
        case TEMP_M1:
            sensorData->temp_m1 = tempData;
            break;
        case TEMP_M2:
            sensorData->temp_m2 = tempData;
            break;
        case TEMP_M3:
            sensorData->temp_m3 = tempData;
            break;
        case TEMP_AIR:
            sensorData->temp_air = tempData;
            break;
            
        case GPS_X:
            sensorData->gps_n = tempData;
            break;
        case GPS_Y:
            sensorData->gps_e = tempData;
            break;
        case GPS_Z:
            sensorData->gps_a = tempData;
            break;
            
        default:
            printf("Unknown control byte: 0x%x\n", rawdata[0]);
            printf("\tPayload: 0x%x 0x%x\n", rawdata[1], rawdata[2]);
            return 1;
    }
    return 0;
}
