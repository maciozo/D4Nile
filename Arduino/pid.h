#include <util/delay.h>

#ifndef PID_H_
#define PID_H_

extern double Kp_roll, 	Ki_roll, 	Kd_roll;
extern double Kp_pitch, Ki_pitch, 	Kd_pitch;
extern double Kp_yall, 	Ki_yall, 	Kd_yall;

int PID(double measured_angle, double desired_angle, double dt, double Kp, double Ki, double Kd); // PID control function

#endif
