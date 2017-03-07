/*
 * PWM-integration.c
 *
 *  Created on: 7 Mar 2017
 *      Author: brendanelmes
 */

int main(void)
{
	init_pwm();
	for(;;)
		{
			change_pwm(left_front,left_back,right_front,right_back);
		}
}

