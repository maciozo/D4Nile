#include "pid.h"

double Kp_roll = 70;
double Ki_roll = 10;
double Kd_roll = 15;

double Kp_pitch = 70;
double Ki_pitch = 10;
double Kd_pitch = 15;

double Kp_yall = 70;
double Ki_yall = 10;
double Kd_yall = 15;

double integral = 0;
double previous_error = 0;

int PID(double measured_angle, double desired_angle, double dt, double Kp, double Ki, double Kd)
{

double output;
double error = desired_angle - measured_angle;
integral += error*dt;
double derivative = (error-previous_error)/dt;

	if ((integral < -0.01)||(integral > 0.01))
		integral = 0;								// prevent integral wind-up

output = Kp*error + Ki*integral + Kd*derivative;	// 	calculate new value
previous_error = error;
_delay_ms(dt);

return(output);

}