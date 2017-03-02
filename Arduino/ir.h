#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
 

#define ADC_PIN 0
#define ADC_THRESHOLD  512

uint16_t adc_read(uint8_t adcx);
void adc_init(void);