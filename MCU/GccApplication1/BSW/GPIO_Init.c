/*
 * GPIO_Init.c
 *
 * Created: 2025-09-20 오후 2:12:37
 *  Author: kym11
 */ 

#include <avr/io.h>
#include "GPIO_Init.h"

void GPIO_Init(void){
	DDRB |= (1<<DDB5) | (1<<DDB4) | (1<<DDB3) | (1<<DDB0);
}