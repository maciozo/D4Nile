

#define INTERRUPT_PIN 2
#define LED_PIN 13

//#define LED_Y 4
//#define LED_R 5
//#define LED_G 6
//#define LED_B 7 

//pid definitions
#define minPWM 2000
#define maxPWM 4000
#define thrust 3000

//#define LOAD 10

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif


// void print_to_screen(int value);
//void blink();

MPU6050 mpu;
//HCMAX7219 HCMAX7219(LOAD);
//int ledState = 0;
// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

Quaternion q;           // [w, x, y, z]         quaternion container
VectorFloat gravity;    // [x, y, z]            gravity vector
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high


double roll_setpoint, pitch_setpoint, yall_setpoint, altitude_coeff;
double roll_angle, pitch_angle, yall_angle;
double err_roll, err_pitch, err_yall;
int left_front, right_front, left_back, right_back;

double roll_kp=5, roll_ki=0, roll_kd=0;
double pitch_kp=5, pitch_ki=0, pitch_kd=0;
double yall_kp=2, yall_ki=0, yall_kd=0;

unsigned long previousMillis; 
unsigned long time;
unsigned long currentMillis = millis();
const long interval = 1000; 


