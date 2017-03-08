#include "uart.h"
#include "commandData.h"
#include "constants.h"
#include "pid.h"
#include "sensors.h"
#include "main.h"

void setup(void)
{
    commanddata_t target_values = {0};
    commanddata_t sensor_data = {0};
    commanddata_t old_sensor_data = sensor_data;
    
    uartInit(void);
    uartSendCommand(INITIALISISNG, 1.0);
    
    sensorInit(void);
}

void loop(void)
{
    
}

/* Compares each current sensor value to the old one. Only sends ones that have changed */
void sendNewSensorData(commanddata_t* sensor_data, commanddata_t* old_sensor_data)
{
    if (sensor_data->yaw_ccw != old_sensor_data->yaw_ccw)
    {
        uartSendCommand(YAW_CCW, sensor_data->yaw_ccw);
        old_sensor_data->yaw_ccw = sensor_data->yaw_ccw;
    }
    
    if (sensor_data->throttle_up != old_sensor_data->throttle_up)
    {
        uartSendCommand(THROTTLE_UP, sensor_data->throttle_up);
        old_sensor_data->throttle_up = sensor_data->throttle_up;
    }
    
    if (sensor_data->roll_left != old_sensor_data->roll_left)
    {
        uartSendCommand(ROLL_LEFT, sensor_data->roll_left);
        old_sensor_data->roll_left = sensor_data->roll_left;
    }
    
    if (sensor_data->pitch_forward != old_sensor_data->pitch_forward)
    {
        uartSendCommand(PITCH_FORWARD, sensor_data->pitch_forward);
        old_sensor_data->pitch_forward = sensor_data->pitch_forward;
    }
    
    if (sensor_data->mode_button != old_sensor_data->mode_button)
    {
        uartSendCommand(MODE_BUTTON, sensor_data->mode_button);
        old_sensor_data->mode_button = sensor_data->mode_button;
    }
    
    if (sensor_data->servo_button != old_sensor_data->servo_button)
    {
        uartSendCommand(SERVO_BUTTON, sensor_data->servo_button);
        old_sensor_data->servo_button = sensor_data->servo_button;
    }
    
    if (sensor_data->autoland != old_sensor_data->autoland)
    {
        uartSendCommand(AUTOLAND, sensor_data->autoland);
        old_sensor_data->autoland = sensor_data->autoland;
    }
}