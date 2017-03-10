#ifndef _CONSTANTS_H
#define _CONSTANTS_H

    /* 
        Enable output to seven segment array
        Disables PWM functions
    */
    #define DEBUG

    /* Include variables for tuning PID gain the the commandData struct */
    #define GAIN_TUNING

    /* Baud rate between RPi and Seeeduino */
    #define UART_BAUD 115200

/* ==============================Pi - MC UART Protocol===================== */

    /* Left stick */
    #define YAW_CCW 0x20
    #define THROTTLE_UP 0x21
    
    /* Right stick || Motion */
    #define ROLL_LEFT 0x22
    #define PITCH_FORWARD 0x23

    /* Buttons */
    #define MODE_BUTTON 0x24
    #define SERVO_BUTTON 0x25
    #define AUTOLAND 0x26
    
    /* Meta */
    #define STATUS 0x27
    #define READY 0.0
    #define INITIALISING 1.0
    
    /* Mad gains */
    #define ROLL_KP 0x28
    #define ROLL_KI 0x29
    #define ROLL_KD 0x2A
    #define PITCH_KP 0x2B
    #define PITCH_KI 0x2C
    #define PITCH_KD 0x2D
    #define YAW_KP 0x2E
    #define YAW_KI 0x2F
    #define YAW_KD 0x30

/* ========================================================================== */

#endif
