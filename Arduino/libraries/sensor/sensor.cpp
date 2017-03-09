#include "sensor.h"
#include "PID_v1.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "MPU6050.h"
#include <avr/io.h>
#include <util/delay.h>

MPU6050 mpu;

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
int16_t gx, gy, gz;
volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high


double roll_setpoint, pitch_setpoint, yall_setpoint, altitude_coeff;
double roll_angle, pitch_angle, yall_angular_vel;
double err_roll, err_pitch, err_yall;
int left_front, right_front, left_back, right_back;

double roll_kp=5, roll_ki=0, roll_kd=0;
double pitch_kp=5, pitch_ki=0, pitch_kd=0;
double yall_kp=2, yall_ki=0, yall_kd=0;

PID roll_PID(&roll_angle, &err_roll, &roll_setpoint, roll_kp, roll_ki, roll_kd, DIRECT);
PID pitch_PID(&pitch_angle, &err_pitch, &pitch_setpoint, pitch_kp, pitch_ki, pitch_kd, DIRECT);
PID yall_PID(&yall_angular_vel, &err_yall, &yall_setpoint, yall_kp, yall_ki, yall_kd, DIRECT);


void init_sensor()
{
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



void init_pid()
{
  roll_setpoint = 0;    // these will be replaced by the outputs
  pitch_setpoint = 0;
  yall_setpoint = 0;
  
//  roll_PID.SetTunings(roll_kp, roll_ki, roll_kd);         This is for Ben's part for tunning
//  pitch_PID.SetTunings(pitch_kp, pitch_ki, pitch_kd);     the PID values via the web server
//  yall_PID.SetTunings(yall_kp, yall_ki, yall_kd);
  Serial.println("pid1");
  roll_PID.SetMode(AUTOMATIC);
  Serial.println("pid2");
  pitch_PID.SetMode(AUTOMATIC);
  Serial.println("pid3");
  yall_PID.SetMode(AUTOMATIC);

}

void dmpDataReady() 
{
    mpuInterrupt = true;
}


void do_everything(float* data)
{

//Serial.println("do evetything loop");
roll_setpoint = 0;
pitch_setpoint = 0;
yall_setpoint = 0;
altitude_coeff = 1;

roll_angle = ypr[2] * 180/M_PI;    
pitch_angle = ypr[1] * 180/M_PI;  
yall_angular_vel = gz/728;


roll_PID.Compute();
pitch_PID.Compute();
yall_PID.Compute();

left_front = thrust*altitude_coeff - err_pitch + err_roll - err_yall;
right_front = thrust*altitude_coeff - err_pitch - err_roll + err_yall;
left_back = thrust*altitude_coeff + err_pitch + err_roll + err_yall;
right_back = thrust*altitude_coeff + err_pitch - err_roll - err_yall;

//Serial.println(left_front);

//int yaw= (ypr[0] * 180/M_PI);

       
// set motor limits
      if (right_back > maxPWM) right_back = maxPWM;
        else if (right_back < minPWM) right_back = minPWM;                  
      if (right_front > maxPWM) right_front = maxPWM;
        else if (right_front < minPWM) right_front = minPWM;      
      if (left_back > maxPWM) left_back = maxPWM;
        else if (left_back < minPWM) left_back = minPWM;            
      if (left_front > maxPWM) left_front = maxPWM;
      else if (left_front < minPWM) left_front = minPWM;

change_pwm(left_front, left_back, right_front, right_back);

    



    mpuInterrupt = false;  // reset interrupt flag and get INT_STATUS byte
    mpuIntStatus = mpu.getIntStatus();
    
    fifoCount = mpu.getFIFOCount();   // get current FIFO count

    if ((mpuIntStatus & 0x10) || fifoCount == 1024)  // check for overflow
    {  
        mpu.resetFIFO();   // reset so we can continue cleanly
        Serial.println(F("FIFO overflow!"));

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
            mpu.getRotation(&gx, &gy, &gz);
/*
            Serial.println("ypr\t");
            Serial.print(ypr[0] * 180/M_PI);
            Serial.print("\t");
            Serial.print(ypr[1] * 180/M_PI);
            Serial.print("\t");
            Serial.print(ypr[2] * 180/M_PI);
            Serial.print("\t");   */
            Serial.println(gz/728);


            mpuIntStatus = mpu.getIntStatus();
            //Serial.print(mpuIntStatus);
           // Serial.print("\n");     


           
            //unsigned long time = millis();
            //Serial.println(time);

    }


    data[0]=gz/728;
    data[1]=ypr[1]* 180/M_PI;
    data[2]=ypr[2]* 180/M_PI;
    
}



void init_pwm(void)
{
	   Serial.print("init_pwm");
		//set output pins for OCRs
        DDRB |= _BV(PB3);
        DDRD |= _BV(PD5);
        DDRD |= _BV(PD6);
        DDRD |= _BV(PD3);

        //set non-inverting mode fast PWM with top value 0xFF (not OCR)
        TCCR0A = _BV(COM0A1) | _BV(COM0B1) | _BV(WGM00) | _BV(WGM01);
        TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM20) | _BV(WGM21);

        //select clock to give prescaler 256, giving a frequency of --Hz
        TCCR0B = _BV(CS02);
        TCCR2B = _BV(CS22) | _BV(CS21);

        //set initial value for OCRs
        OCR0A = 62;
        OCR0B = 62;
        OCR2A = 62;
        OCR2B = 62;
}

void change_pwm(int left_front, int left_back, int right_front, int right_back)
{
	   OCR0A = left_front/33;		//divide input by 33 to put it in duty cycle range
	   OCR0B = left_back/33;
	   OCR2A = right_front/33;
	   OCR2B = right_back/33;
}

