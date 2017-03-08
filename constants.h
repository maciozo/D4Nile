#ifndef _CONSTANTS_H
#define _CONSTANTS_H

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

/* ========================================================================== */

#endif
