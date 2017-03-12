#include "uart.h"
#include "commandData.h"
#include "constants.h"
#include "sensor.h"
#include "main.h"
#include <stdio.h>

// #define DEBUG
// #include "debug.h"

#ifdef DEBUG
    /* Include the HCMAX7219 and SPI library */
    #include <HCMAX7219.h>
    #include "SPI.h"

    /* Set the LOAD (CS) digital pin number*/
    #define LOAD 10
    
    /* Create an instance of the library */
    HCMAX7219 HCMAX7219(LOAD);
#endif

float data[] = {0.0, 0.0, 0.0};

void setup(void)
{
    initDebug();
    // debug("SETUP", 8);
    #ifndef GAIN_TUNING
        target_values = {0, 0, 0, 0, 0, 0, 0};
        sensor_data = {0, 0, 0, 0, 0, 0, 0};
        old_sensor_data = {0, 0, 0, 0, 0, 0, 0};
    #else
        target_values = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        sensor_data = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        old_sensor_data = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    #endif
    
    debug("UARTINIT", 8);
    uartInit();
    uartSendCommand(STATUS, INITIALISING);
    
    #ifndef DEBUG
        init_pwm();
    #endif
    debug("SENSINIT", 8);
    init_sensor();
    debug("PID INIT", 8);
    init_pid();
    
    uartSendCommand(STATUS, READY);
    debug("READY", 8);
}

void loop(void)
{
    /* Get command from UART */
    // debug("READUART", 8);
    uartReadRaw(recvBuffer, MC_RECVBUFFER_SIZE);
    
    /* Put the new command in to the target_values struct */
    // debug("FORMDATA", 8);
    formatData(&target_values, recvBuffer);
    
    /* Do PID calculations to get the real sensor values towards the target values */
    // debug("DE  LOOP", 8);
    do_everything(&sensor_data, &target_values, data);
    
    /* Send any sensor values that have changed since they were last sent */
    // debug("SENDUART", 8);
    // sendNewSensorData(&sensor_data, &old_sensor_data);
}

/* Compares each current sensor value to the old one. Only sends ones that have changed */
void sendNewSensorData(commanddata_t* sensor_data, commanddata_t* old_sensor_data)
{
    // if (sensor_data->yaw_ccw != old_sensor_data->yaw_ccw)
    // {
        uartSendCommand(YAW_CCW, sensor_data->yaw_ccw);
        old_sensor_data->yaw_ccw = sensor_data->yaw_ccw;
    // }
    
    // if (sensor_data->throttle_up != old_sensor_data->throttle_up)
    // {
        uartSendCommand(THROTTLE_UP, sensor_data->throttle_up);
        old_sensor_data->throttle_up = sensor_data->throttle_up;
    // }
    
    // if (sensor_data->roll_left != old_sensor_data->roll_left)
    // {
        uartSendCommand(ROLL_LEFT, sensor_data->roll_left);
        old_sensor_data->roll_left = sensor_data->roll_left;
    // }
    
    // if (sensor_data->pitch_forward != old_sensor_data->pitch_forward)
    // {
        uartSendCommand(PITCH_FORWARD, sensor_data->pitch_forward);
        old_sensor_data->pitch_forward = sensor_data->pitch_forward;
    // }
    
    // if (sensor_data->mode_button != old_sensor_data->mode_button)
    // {
        uartSendCommand(MODE_BUTTON, sensor_data->mode_button);
        old_sensor_data->mode_button = sensor_data->mode_button;
    // }
    
    // if (sensor_data->servo_button != old_sensor_data->servo_button)
    // {
        uartSendCommand(SERVO_BUTTON, sensor_data->servo_button);
        old_sensor_data->servo_button = sensor_data->servo_button;
    // }
    
    // if (sensor_data->autoland != old_sensor_data->autoland)
    // {
        uartSendCommand(AUTOLAND, sensor_data->autoland);
        old_sensor_data->autoland = sensor_data->autoland;
    // }
    

}

void initDebug()
{
    #ifdef DEBUG
    HCMAX7219.Clear();
    #endif
    return;
}

void debug(char TextString[], unsigned int Offset)
{
    #ifdef DEBUG
    HCMAX7219.Clear();
    HCMAX7219.print7Seg(TextString, Offset);
    HCMAX7219.Refresh();
    // delay(500);
    #endif
    return;
}

void debug(long number, unsigned int Offset)
{
    #ifdef DEBUG
    HCMAX7219.Clear();
    HCMAX7219.print7Seg(number, Offset);
    HCMAX7219.Refresh();
    // delay(500);
    #endif
    return;
}
