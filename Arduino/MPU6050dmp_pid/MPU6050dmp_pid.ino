#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include <HCMAX7219.h>
#include "SPI.h"
#include <PID_v1.h>

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

#define LOAD 10
// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68
// AD0 high = 0x69



//MPU6050 mpu(0x69); // <-- use for AD0 high

#define INTERRUPT_PIN 2
#define LED_PIN 13

#define LED_Y 4
#define LED_R 5
#define LED_G 6
#define LED_B 7 

//pid definitions
#define minPWM 2000
#define maxPWM 4000
#define thrust 3000

MPU6050 mpu;
HCMAX7219 HCMAX7219(LOAD);
int ledState = 0;
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
void dmpDataReady() {
    mpuInterrupt = true;
}

double roll_setpoint, pitch_setpoint, yall_setpoint, altitude_coeff;
double roll_angle, pitch_angle, yall_angle;
double err_roll, err_pitch, err_yall;
int left_front, right_front, left_back, right_back;

double roll_kp=70, roll_ki=10, roll_kd=15;
double pitch_kp=70, pitch_ki=10, pitch_kd=15;
double yall_kp=70, yall_ki=10, yall_kd=15;

PID roll_PID(&roll_angle, &err_roll, &roll_setpoint, roll_kp, roll_ki, roll_kd, DIRECT);
PID pitch_PID(&pitch_angle, &err_pitch, &pitch_setpoint, pitch_kp, pitch_ki, pitch_kd, DIRECT);
PID yall_PID(&yall_angle, &err_yall, &yall_setpoint, yall_kp, yall_ki, yall_kd, DIRECT);


void setup() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    Serial.begin(115200);
    while (!Serial)

    // initialize device
    Serial.println(F("Initializing I2C devices..."));
    mpu.initialize();
    pinMode(INTERRUPT_PIN, INPUT);
    pinMode(LED_R, OUTPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(LED_G, OUTPUT);
    pinMode(LED_Y, OUTPUT);


  //roll_angle = 30;      // dummy inputs, when we're integrating modules
  roll_setpoint = 0;    // these will be replaced by the outputs
  //pitch_angle = 30;     // from the Sensor and Pi_to_mc modules
  pitch_setpoint = 0;
  //yall_angle = 30;
  yall_setpoint = 0;
  
//  roll_PID.SetTunings(roll_kp, roll_ki, roll_kd);         This is for Ben's part for tunning
//  pitch_PID.SetTunings(pitch_kp, pitch_ki, pitch_kd);     the PID values via the web server
//  yall_PID.SetTunings(yall_kp, yall_ki, yall_kd);
  
  roll_PID.SetMode(AUTOMATIC);
  pitch_PID.SetMode(AUTOMATIC);
  yall_PID.SetMode(AUTOMATIC);


    
    // verify connection
    Serial.println(F("Testing device connections..."));
    Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));
    uint8_t test_whoami_reg = mpu.testConnection();
    Serial.println(test_whoami_reg);
    
    Serial.println(F("Initializing DMP..."));
    devStatus = mpu.dmpInitialize();
   // mpu.setDLPFMode(0);
    // supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788); // 1688 factory default for my test chip

    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
      
        Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
        Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
        attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        
        Serial.print(devStatus);
        Serial.println(F(")"));
    }

}

unsigned long previousMillis = 1; 

void loop() {
unsigned long time;

int yaw= (ypr[0] * 180/M_PI);

roll_setpoint = 0;
pitch_setpoint = 0;
yall_setpoint = 0;
altitude_coeff = 1;

  roll_angle = ypr[2] * 180/M_PI;    

  pitch_angle = ypr[1] * 180/M_PI;  

  yall_angle = ypr[0] * 180/M_PI;

roll_PID.Compute();
pitch_PID.Compute();
yall_PID.Compute();



left_front = thrust*altitude_coeff - err_pitch + err_roll - err_yall;
right_front = thrust*altitude_coeff - err_pitch - err_roll + err_yall;
left_back = thrust*altitude_coeff + err_pitch + err_roll + err_yall;
right_back = thrust*altitude_coeff + err_pitch - err_roll - err_yall;




       HCMAX7219.Clear();
       HCMAX7219.print7Seg(yaw,8);
       HCMAX7219.Refresh();
       digitalWrite(LED_B,HIGH);
       
// set motor limits
//      if (right_back > maxPWM) right_back = maxPWM;
//        else if (right_back < minPWM) right_back = minPWM;                  
//      if (right_front > maxPWM) right_front = maxPWM;
//        else if (right_front < minPWM) right_front = minPWM;      
//      if (left_back > maxPWM) left_back = maxPWM;
//        else if (left_back < minPWM) left_back = minPWM;            
//      if (left_front > maxPWM) left_front = maxPWM;
//      else if (left_front < minPWM) left_front = minPWM;

    
       unsigned long currentMillis = millis();
       const long interval = 1000; 

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED      

    previousMillis = currentMillis;
    ledState=!ledState;

  }
digitalWrite(LED_Y, ledState);
    

    
       
   //    Serial.print("program");
 //      Serial.print("\n");
    


      
    mpuInterrupt = false;  // reset interrupt flag and get INT_STATUS byte
    mpuIntStatus = mpu.getIntStatus();

    
    fifoCount = mpu.getFIFOCount();   // get current FIFO count

    
    if ((mpuIntStatus & 0x10) || fifoCount == 1024)  // check for overflow
    {  
        mpu.resetFIFO();   // reset so we can continue cleanly
        Serial.println(F("FIFO overflow!"));
        digitalWrite(LED_R, HIGH);
    }

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
    
   else if (mpuIntStatus & 0x02) 
   {
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount(); // wait for correct available data length, should be a VERY short wait
        mpu.getFIFOBytes(fifoBuffer, packetSize); // read a packet from FIFO
        fifoCount -= packetSize;  // track FIFO count here in case there is > 1 packet available

            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
      
            Serial.print("ypr\t");
            Serial.print(ypr[0] * 180/M_PI);
            Serial.print("\t");
            Serial.print(ypr[1] * 180/M_PI);
            Serial.print("\t");
            Serial.println(ypr[2] * 180/M_PI);

            mpuIntStatus = mpu.getIntStatus();
            //Serial.print(mpuIntStatus);
            Serial.print("\n");     

            digitalWrite(LED_G,HIGH);
            digitalWrite(LED_R,LOW);
            digitalWrite(LED_B,LOW);
            
    
            //time = millis();
           // Serial.println(time);

    }
  }
