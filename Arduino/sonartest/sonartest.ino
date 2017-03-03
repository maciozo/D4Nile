
#include "uart.h"

#define echoPin 3 // Echo Pin
#define trigPin 2 // Trigger Pin
int read_distance();

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
    int distance = read_distance();
    
    itoa (distance,String,10);
    USART_putstring(String);
    USART_putstring("\n");
    
    delay(100);
  }
}


int read_distance()
{
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
 
  digitalWrite(trigPin, LOW);
  int duration = pulseIn(echoPin, HIGH);
  int distance = duration/58.2;

  return distance;
}

