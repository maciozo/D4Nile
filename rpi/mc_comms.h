#ifndef _MC_COMMS
#define _MC_COMMS

#define SERIAL_DEVICE "/dev/serial0"
#define RECVBUFFER_SIZE 64
#define SENDBUFFER_SIZE 64

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

char recvBuffer[RECVBUFFER_SIZE];
char sendBuffer[SENDBUFFER_SIZE];
sensordata_t sensorData;


int uartInit(char* device);
int uartLoop(FILE *uartDevice);
char* uartReadRaw(void);
int uartSend(char* string);

#endif
