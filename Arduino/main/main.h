#ifndef MAIN_H
#define MAIN_H

#include "commandData.h"
#include "constants.h"

commanddata_t target_values;
commanddata_t sensor_data;
commanddata_t old_sensor_data;

char recvBuffer[MC_RECVBUFFER_SIZE];

void setup(void);
void loop(void);
void sendNewSensorData(commanddata_t* sensor_data, commanddata_t* old_sensor_data);

#else
    #define roll_setpoint WHY_ARE_YOU_INCLUDING_MAIN_H_MORE_THAN_ONCE
#endif
