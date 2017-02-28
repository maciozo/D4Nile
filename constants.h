#ifndef _CONSTANTS_H
#define _CONSTANTS_H

/* ==============================MC -> Pi UART Protocol===================== */

    /* Accelerometer data */
    #define ACC_X 0x20
    #define ACC_Y 0x21
    #define ACC_Z 0x22

    /* Gyroscope data */
    #define GYR_X 0x23
    #define GYR_Y 0x24
    #define GYR_Z 0x25

    /* Magnetometer data */
    #define MAG_X 0x26
    #define MAG_Y 0x27
    #define MAG_Z 0x28

    /* Altitude data */
    #define IR 0x29
    #define USONIC 0x2A

    /* Temperature data */
    #define TEMP_BAT 0x2B
    #define TEMP_M0 0x2C
    #define TEMP_M1 0x2D
    #define TEMP_M2 0x2E
    #define TEMP_M3 0x2F
    #define TEMP_AIR 0x30

    /* GPS data */
    #define GPS_N 0x31
    #define GPS_E 0x32
    #define GPS_A 0x33
    
    #define RECV_MAX 0x33
    
/* ========================================================================== */

/* ==============================Pi -> MC UART Protocol===================== */

    #define YAW_LEFT 0x20
    #define THROTTLE_UP 0x21
    
    #define ROLL_LEFT 0x22
    #define PITCH_FORWARD 0x23

    #define X_BUTTON 0x24

/* ========================================================================== */

#endif
