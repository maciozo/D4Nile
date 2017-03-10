#ifndef SENSOR_H_
#define SENSOR_H_

#define INTERRUPT_PIN 2
#define minPWM 2000
#define maxPWM 4000
#define thrust 3000

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

#include "commandData.h"

void init_sensor();
void init_pid();
void dmpDataReady();
void do_everything(commanddata_t* sensor_data, commanddata_t* target_values, float* data);
void init_pwm(void);
void change_pwm(int left_front, int left_back, int right_front, int right_back);

#endif /* SENSOR_H_ */
