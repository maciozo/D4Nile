//PD2 Trigger
//PD3 Echo
//PC0 ACD IR sensor

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "sonar.h"
#include "uart.h"
#include "ir.h"
#include <math.h>




char String[8]; 

int main (void)
{

  ADCSRA |= _BV(ADEN);  /* Enable the ADC */

 DDRB |= _BV(DDB5);   /* set pin 5 of PORTB for output*/
 USART_init();  
 adc_init();

 while(1) {

  //int distance=read_sonar();

  uint16_t adc_value = adc_read(ADC_PIN);
  uint16_t distance = 12343.85* pow(adc_value, -1.15);
  itoa (distance,String,10);
  USART_putstring(String);
  USART_putstring("\n");



    if (adc_value > ADC_THRESHOLD)
      PORTB |= _BV(PORTB5);
    else
      PORTB &= ~_BV(PORTB5);
    
    _delay_ms(100);
 }

 return 0;
}

