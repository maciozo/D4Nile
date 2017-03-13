#ifndef SENSOR_H_
#define SENSOR_H_

#define INTERRUPT_PIN 2
#define minPWM 2000
#define maxPWM 4000
#define thrust 2300

#define echoPin 3 // Echo Pin
#define trigPin 2 // Trigger Pin

#define batteryRead A0

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

#include "commandData.h"

void init_sensor();
void init_pid();
void dmpDataReady();
void do_everything(commanddata_t* sensor_data, commanddata_t* target_values);
void init_pwm(void);
void change_pwm(double left_front, double left_back, double right_front, double right_back);
void init_sonar();
int read_ultrasonic();
float batteryVoltage();

#endif /* SENSOR_H_ */
