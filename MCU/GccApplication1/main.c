#define F_CPU 16000000L
#include <avr/io.h>
#include "Common/project.h"
#include <stdlib.h>

int main(void)
{
	ISR_Init();
	UART_Init(9600);
	ADC_Init();
	pwm_init();
	Sonic_Init();
	lcd_init();
	lcd_clear();
	while (1) 
	{
	}
	return 1;
}

void task_5ms(void) {
	ADC_Start();
}

void task_20ms(void){
	Steering_Control(steer);
	Speed_Control(motor, brek);
}

void task_60ms(void) {
	Sonic_Task();
}
