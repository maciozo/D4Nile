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

float batteryV;

void setup(void)
{
    // debug("SENSINIT", 8);
    init_sensor();
    init_sonar();
    delay(20000);
    
    initDebug();
    // debug("SETUP", 8);
    #ifndef GAIN_TUNING
        target_values = {0, 1.0, 0, 0, 0, 0, 0};
        sensor_data = {0, 0, 0, 0, 0, 0, 0};
        old_sensor_data = {0, 0, 0, 0, 0, 0, 0};
    #else
        target_values = {0, 1.0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        sensor_data = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        old_sensor_data = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    #endif
    
    // debug("UARTINIT", 8);
    uartInit();
    // uartSendCommand(STATUS, INITIALISING);
    
    #ifndef DEBUG
        init_pwm();
    #endif
    
    // debug("PID INIT", 8);
    init_pid();
    
    // uartSendCommand(STATUS, READY);
    // debug("READY", 8);
}

void loop(void)
{
    /* Get command from UART */
    // debug("READUART", 8);
    if (UCSR0A & (1 << RXC0))
    {
        uartReadRaw(recvBuffer, MC_RECVBUFFER_SIZE);
        /* Put the new command in to the target_values struct */
        // debug("FORMDATA", 8);
        formatData(&target_values, recvBuffer);
    }
    
    /* Do PID calculations to get the real sensor values towards the target values */
    // debug("DE  LOOP", 8);
    do_everything(&sensor_data, &target_values);
    
    /* Send any sensor values that have changed since they were last sent */
    // debug("SENDUART", 8);
    sendNewSensorData(&sensor_data, &old_sensor_data);
    // sendNewSensorData(&target_values, &old_sensor_data);
}

/* Compares each current sensor value to the old one. Only sends ones that have changed */
void sendNewSensorData(commanddata_t* sensor_data, commanddata_t* old_sensor_data)
{
    uartSendCommand(YAW_CCW, (int16_t)((sensor_data->yaw_ccw) * 32767 / 90));
    uartSendCommand(ROLL_LEFT, (int16_t)((sensor_data->roll_left) * 32767 / 10));
    uartSendCommand(PITCH_FORWARD, (int16_t)((sensor_data->pitch_forward) * 32767 / 10));
    
    batteryV = batteryVoltage();
    uartSendCommand(VOLTAGE, (int16_t)(batteryV * 100));
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
