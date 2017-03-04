
#include "uartme.h"

#define echoPin 3 // Echo Pin
#define trigPin 2 // Trigger Pin
#define analogPin A0
int read_distance();
int read_ir();

void setup()
{
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  USART_init(); 
 
}

void loop()
{
 char String[8];

  while(1){
    //int distanceus = read_distance();
    int distanceir = read_ir();
    
    itoa (distanceir,String,10);
    USART_putstring(String);
    USART_putstring("\r\n");
    
    delay(100);
  }
}


int read_ultrasonic()
{
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
 
  digitalWrite(trigPin, LOW);
  int duration = pulseIn(echoPin, HIGH);
  int distanceus = duration/58.2;

  return distanceus;
}

int read_ir()
{
  int val = 0;
   
  val = analogRead(analogPin); 
  
  int distanceir = 12343.85*pow(val, -1.15);
  if (distanceir <9 || distanceir >80) {
    distanceir=0;
  }
  return distanceir;
}
