//PD2 Trigger
//PD3 Echo

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "sonar.h"
#include "uart.h"

//Declaration of our functions

 
char String[8];   //String[] is in fact an array but when we put the text between the " " symbols the compiler threats it as a String and automatically puts the null termination character in the end of the text
//int i=1;

int main (void)
{
 /* set pin 5 of PORTB for output*/
 DDRB |= _BV(DDB5);
 USART_init();  

 while(1) {
  /* set pin 5 high to turn led on */
  //PORTB |= _BV(PORTB5);
  //_delay_ms(BLINK_DELAY_MS);

  /* set pin 5 low to turn led off */
  //PORTB &= ~_BV(PORTB5);
  //_delay_ms(BLINK_DELAY_MS);

  int distance=read_sonar();
  _delay_ms(100);

  itoa (distance,String,10);
  USART_putstring(String);
  USART_putstring("\n");
  //i++;
 }

 return 0;
}




