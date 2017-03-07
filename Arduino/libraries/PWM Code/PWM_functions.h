/*
 * PWM_functions.h
 *
 *  Created on: 7 Mar 2017
 *      Author: brendanelmes
 */

#ifndef PWM_FUNCTIONS_H_
#define PWM_FUNCTIONS_H_

#include <avr/io.h>
#include <util/delay.h>

void init_pwm(void)
{
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

#endif /* PWM_FUNCTIONS_H_ */
