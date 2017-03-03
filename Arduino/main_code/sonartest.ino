#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "sonar.h"
#include "uart.h"
//int TRIGGER = 2;  
//int ECHO    = 3;  

void setup()
{

 // Serial.begin(115200);
  //pinMode(TRIGGER, OUTPUT);
  //pinMode(ECHO, INPUT);
 USART_init(); 
 
}

void loop()
{
  char String[8]= "Hello";
  
while(1){

  int distance=read_sonar();

  itoa (distance,String,10);
  USART_putstring(String);
  USART_putstring("\n");

 
  delay(100);
}
}
