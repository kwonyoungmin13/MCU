/*
 * ADC_Init.c
 *
 * Created: 2025-09-19
 * Author: rakggii
 */

#include <avr/io.h>
#include "ADC_Init.h"


void ADC_Init(void) {
	ADMUX = (1 << REFS0) | (0x03 & 0x07); // AVcc 기준, ADC3 선택
	ADCSRA = (1 << ADEN) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1);
}

