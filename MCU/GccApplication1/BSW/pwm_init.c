/*
 * pwm_init.c
 */ 
#include <avr/io.h>
#include "pwm_init.h"

#define ICR1_TOP 39999

void pwm_init(void) {
	DDRB |= (1 << DDB1) | (1 << DDB2);
	TCCR1A = (1 << COM1A1) | (1 << COM1B0) | (1 << COM1B1) | (1 << WGM11);
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);
	ICR1 = ICR1_TOP;
}

