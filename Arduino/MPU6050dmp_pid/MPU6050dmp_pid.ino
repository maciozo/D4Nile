#include "sensor.h"

void setup() {
  init_pwm();
  init_sensor();
  init_pid();
}

void loop() {
  do_everything();
}


