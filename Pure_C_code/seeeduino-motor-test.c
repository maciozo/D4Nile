/*
 * seeeduino-motor-test.c
 *
 *  Created on: 6 Mar 2017
 *      Author: brendanelmes
 */

#include <avr/io.h>
#include <util/delay.h>

void init_pwm(void);
void change_pwm(int motor, float duty);

int main(void)
{
	init_pwm();
    //set initial value for OCRs
    OCR0A = 122;
    OCR0B = 122;
    OCR2A = 122;
    OCR2B = 122;

   _delay_ms(10000);

   OCR0A = 62;
   OCR0B = 62;
   OCR2A = 62;
   OCR2B = 62;

   _delay_ms(10000);
	for(;;)
		{
			change_pwm(/*left_front*/1,120);
			change_pwm(/*right_front*/2,90);
			change_pwm(/*left_back*/3,80);
			change_pwm(/*right_back*/4,70);
		}
}

void change_pwm(int motor, float duty)
{
	//int left_front, left_back, right_front, right_back;
	switch(motor)
	{
      case /*left_front*/ 1:
    	  	  OCR0A = duty; // Port Pin -- Seeeduino Pin 6
                  break;

      case /*right_front*/ 2:
    	  	  OCR0B = duty; // Port Pin -- Seeeduino Pin 5
    	  	  	  break;

      case /*left_back*/ 3:
    	  	  OCR2A = duty; // Port Pin -- Seeeduino Pin 11
                  break;

      case /*right_back*/ 4:
    	  	  OCR2B = duty; //Port Pin -- Seeeduino Pin 3
    	  	  	  break;
      default:
    	  	break;
   }
}

void init_pwm(void)
{
        //set initial value for OCRs
        OCR0A = 122;
        OCR0B = 122;
        OCR2A = 122;
        OCR2B = 122;

        //set output pins for OCRs
        DDRB |= _BV(PB3);
        DDRD |= _BV(PD5);
        DDRD |= _BV(PD6);
        DDRD |= _BV(PD3);

        //set input pins for incoming instructions
        //DDRB &= ~_BV(PB1);
        //DDRB &= ~_BV(PB2);
        //DDRB &= ~_BV(PB3);
        //DDRD &= ~_BV(PD3);

        //set non-inverting mode fast PWM with top value 0xFF (not OCR)
        TCCR0A = _BV(COM0A1) | _BV(COM0B1) | _BV(WGM00) | _BV(WGM01);
        TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM20) | _BV(WGM21);

        //select clock to give prescaler 256, giving a frequency of --Hz
        TCCR0B = _BV(CS02);
        TCCR2B = _BV(CS22) | _BV(CS21);
}
