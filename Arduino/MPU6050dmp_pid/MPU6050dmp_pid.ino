#include "sensor.h"

void setup() {
  init_pwm();
  init_sensor();
  init_pid();
}

void loop() {
  
  float ypr[3]={0};
  //int16_t yaw_val;
 do_everything(ypr);
/*
  Serial.println(ypr[0]);
  Serial.print("\t");
  Serial.print(ypr[1]);
  Serial.print("\t");
  Serial.println(ypr[2]);
*/

}


