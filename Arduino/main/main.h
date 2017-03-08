#include "commandData.h"

commanddata_t target_values;
commanddata_t sensor_data;
commanddata_t old_sensor_data;

void setup(void);
void loop(void);
void sendNewSensorData(commanddata_t* sensor_data, commanddata_t* old_sensor_data);
