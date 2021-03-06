#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

#include "definitions.h"
#include <PID_v1.h>

#define JOYSTICK_1_X A0
#define JOYSTICK_1_Y A1
#define JOYSTICK_2_X A2
#define JOYSTICK_2_Y A3

//#define PWM
#define DISPLAY

#ifdef DISPLAY
  #include <HCMAX7219.h>
  #include "SPI.h"
  #define LOAD 10
  HCMAX7219 HCMAX7219(LOAD);
#endif

#ifdef PWM
  #include "PWM_functions.h"
#endif

int VAL_1_X, VAL_1_Y, VAL_2_X, VAL_2_Y;

PID roll_PID(&roll_angle, &err_roll, &roll_setpoint, roll_kp, roll_ki, roll_kd, REVERSE);
PID pitch_PID(&pitch_angle, &err_pitch, &pitch_setpoint, pitch_kp, pitch_ki, pitch_kd, REVERSE);
PID yall_PID(&yall_angle, &err_yall, &yall_setpoint, yall_kp, yall_ki, yall_kd, DIRECT);

void dmpDataReady() {
    mpuInterrupt = true;
}


void setup() {
#ifdef PWM
  init_pwm();
#endif


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
  //  pinMode(LED_R, OUTPUT);
  //  pinMode(LED_B, OUTPUT);
  //  pinMode(LED_G, OUTPUT);
  //  pinMode(LED_Y, OUTPUT);

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



void loop() {
if (Serial.available() > 0) {
    int data = Serial.read()-48;
   // Serial.readBytes(buffer, length)
    Serial.print(data);
    #ifdef DISPLAY
           HCMAX7219.Clear();
           HCMAX7219.print7Seg(data,8);
           HCMAX7219.Refresh(); 
    #endif
    }
 

  VAL_1_X = analogRead(JOYSTICK_1_X);
  VAL_1_Y = analogRead(JOYSTICK_1_Y);
  VAL_2_X = analogRead(JOYSTICK_2_X);
  VAL_2_Y = analogRead(JOYSTICK_2_Y);


       
/*
  Serial.println(VAL_1_X); Serial.print("\t");
  Serial.print(VAL_1_Y); Serial.print("\t");
  Serial.print(VAL_2_X); Serial.print("\t");
  Serial.print(VAL_2_Y); Serial.print("\t");
*/  
  
roll_setpoint = VAL_1_X-500;
pitch_setpoint = VAL_1_Y-500;
yall_setpoint = VAL_2_X-500;
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

//Serial.println(left_front);

int yaw= (ypr[0] * 180/M_PI);
//print_to_screen(yaw);
//digitalWrite(LED_B,HIGH);
       
// set motor limits
      if (right_back > maxPWM) right_back = maxPWM;
        else if (right_back < minPWM) right_back = minPWM;                  
      if (right_front > maxPWM) right_front = maxPWM;
        else if (right_front < minPWM) right_front = minPWM;      
      if (left_back > maxPWM) left_back = maxPWM;
        else if (left_back < minPWM) left_back = minPWM;            
      if (left_front > maxPWM) left_front = maxPWM;
      else if (left_front < minPWM) left_front = minPWM;

#ifdef PWM
  change_pwm(left_front, left_back, right_front, right_back);
#endif

/*
Serial.println(OCR0A); Serial.print("\t");
Serial.print(OCR0B); Serial.print("\t");
Serial.print(OCR2A); Serial.print("\t");
Serial.print(OCR2B); Serial.print("\t");
*/

//    blink();

    mpuInterrupt = false;  // reset interrupt flag and get INT_STATUS byte
    mpuIntStatus = mpu.getIntStatus();
    
    fifoCount = mpu.getFIFOCount();   // get current FIFO count

    if ((mpuIntStatus & 0x10) || fifoCount == 1024)  // check for overflow
    {  
        mpu.resetFIFO();   // reset so we can continue cleanly
        Serial.println(F("FIFO overflow!"));
  //      digitalWrite(LED_R, HIGH);
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
      
/*            Serial.print("ypr\t");
            Serial.print(ypr[0] * 180/M_PI);
            Serial.print("\t");
            Serial.print(ypr[1] * 180/M_PI);
            Serial.print("\t");
            Serial.println(ypr[2] * 180/M_PI);
*/
            mpuIntStatus = mpu.getIntStatus();
            //Serial.print(mpuIntStatus);
            //Serial.print("\n");     

            //digitalWrite(LED_G,HIGH);
         //   digitalWrite(LED_R,LOW);
           // digitalWrite(LED_B,LOW);
           
         //   unsigned long time = millis();
          //  Serial.println(time);

    }
  }


  /*

  void print_to_screen(int value)
  {
       HCMAX7219.Clear();
       HCMAX7219.print7Seg(value,8);
       HCMAX7219.Refresh();  
  }

  void blink()
  {
       unsigned long currentMillis = millis();
       const long interval = 1000; 
       
       if (currentMillis - previousMillis >= interval) {   
          previousMillis = currentMillis;
          ledState=!ledState;
       }
    digitalWrite(LED_Y, ledState);
}
  */
