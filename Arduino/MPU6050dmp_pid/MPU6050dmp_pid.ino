#include "sensor.h"

void setup() {
  init_pwm();
  init_sensor();
  init_pid();
}

void loop() {
  
  float ypr[3]={0};
  do_everything(ypr);

  Serial.print("ypr\t");
  Serial.print(ypr[0]);
  Serial.print("\t");
  Serial.print(ypr[1]);
  Serial.print("\t");
  Serial.println(ypr[2]);


}


