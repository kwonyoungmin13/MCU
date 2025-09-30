/*
 * ADC.c
 *
 * Created: 2025-09-20 오후 1:11:39
 *  Author: kym11
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "ADC.h"
#include "../Common/defs.h"
#include "../BSW/UART.h"
#include <avr/sleep.h>

volatile uint16_t steer;
volatile uint16_t motor;
volatile uint16_t brek;
volatile uint16_t button_val;
volatile uint8_t current_channel = STEER;

//ADC값으로부터 몇번 버튼 눌렸는지 확인
static uint8_t get_button_from_adc(uint16_t adc_val) {
	if (adc_val > BTN_5_LOW) return BUTTON_PROGRAMOFF;
	if (adc_val >= BTN_4_LOW && adc_val <= BTN_4_HIGH) return BUTTON_GETOUT;
	if (adc_val >= BTN_3_LOW && adc_val <= BTN_3_HIGH) return BUTTON_SORRY;
	if (adc_val >= BTN_2_LOW && adc_val <= BTN_2_HIGH) return BUTTON_AC;
	if (adc_val >= BTN_1_LOW && adc_val <= BTN_1_HIGH) return BUTTON_HOTASS;
	return 0;
}

//ADC변환 시작
void ADC_Start(void){
	ADCSRA |= (1 << ADSC);
}

//5ms마다 하나씩 변환하도록 설정
ISR(ADC_vect) {
	if(current_channel == STEER) steer = ADC;
	else if(current_channel == MOTOR) motor = ADC;
	else if(current_channel == BREAK) brek = ADC;
	else if(current_channel == BUTTON) {
		button_val = ADC;
	}

	if(current_channel == STEER) current_channel = MOTOR;
	else if(current_channel == MOTOR) current_channel = BREAK;
	else if(current_channel == BREAK) current_channel = BUTTON;
	else current_channel = STEER;
	
	ADMUX = (1 << REFS0) | (current_channel & 0x07);
}
