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
#include "i2c.h"
#include "MPU9250_reg.h"
#include <math.h>

void init_MPU9250(void);
float getacc(void);
int16_t whoami(void);

char buffer[8]; 
int16_t distance;
int16_t raw_x = 0;
int16_t raw_y = 0;
int16_t raw_z = 0;
int16_t value;



enum Ascale {
  AFS_2G = 0,
  AFS_4G,
  AFS_8G,
  AFS_16G
};

enum Gscale {
  GFS_250DPS = 0,
  GFS_500DPS,
  GFS_1000DPS,
  GFS_2000DPS
};
uint8_t Ascale = AFS_2G;     
uint8_t Gscale = GFS_250DPS;

int main (void)
{
  DDRB |= _BV(DDB5);   /* set pin 5 of PORTB for output*/
  USART_init();  
  i2c_init();
  USART_putstring("initialised i2c"); 

  init_MPU9250();
 
USART_putstring("initialised imu \t"); 
  while(1) {

  // acc =  getacc();

 
  whoami();
USART_putstring("after whoami function \t"); 
  itoa (value,buffer,10);
  USART_putstring(buffer);
  USART_putstring("\n");



 
    
    _delay_ms(1000);
 }

 return 0;
}


void init_MPU9250(void){

  USART_putstring("start int loop"); 

  int16_t c;
  i2c_start(MPU9250_WRITE);
  USART_putstring("2"); 

  i2c_write(PWR_MGMT_1); 
  i2c_write(0x00); // clear sleep mode bit (6), enable all sensors
  
  USART_putstring("2"); 


}


int16_t  whoami(void){
//i2c_readReg(MPU9250_WRITE, WHO_AM_I, value, 16);

USART_putstring("entered whoami\t"); 
  i2c_start(MPU9250_WRITE);
  i2c_write(WHO_AM_I); // set pointer to X axis MSB
  i2c_stop();
USART_putstring("imu write function \t"); 
  
  i2c_start(MPU9250_READ);
USART_putstring("1 \t");
  value = ((uint8_t)i2c_read_ack())<<8;
 

  USART_putstring("2 \n");
  value |= i2c_read_ack();
i2c_stop();

USART_putstring("after read \n"); 


  itoa (value,buffer,10);
  USART_putstring(buffer);
  USART_putstring("\n");
USART_putstring("after loop\n"); 

return value;
}