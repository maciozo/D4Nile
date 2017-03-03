#ifndef _SD_H
#define _SD_H

typedef struct
{
  int16_t acc_x;
  int16_t acc_y;
  int16_t acc_z;
  
  int16_t gyr_x;
  int16_t gyr_y;
  int16_t gyr_z;
  
  int16_t mag_x;
  int16_t mag_y;
  int16_t mag_z;
  
  int16_t ir;
  int16_t usonic;
  
  int16_t temp_bat;
  int16_t temp_m0;
  int16_t temp_m1;
  int16_t temp_m2;
  int16_t temp_m3;
  int16_t temp_air;
  
  int16_t gps_n;
  int16_t gps_e;
  int16_t gps_a;
} sensordata_t;

int formatData(sensordata_t* sensorData, char* rawdata);

#endif
