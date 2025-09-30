/*
 * ISR.c
 *
 * Created: 2025-09-19 오후 6:13:19
 *  Author: kym11
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "ISR.h"
int ms_count = 0;

void ISR_Init(void){
	TCCR2A |= (1<<WGM21);
	//CTC모드 ON (dataSheet 109p 참고)
	TCCR2B |= (1<<CS22);
	//prescaler 설정
	OCR2A = 249;
	//TOP 설정
	TIMSK2 |= (1<<OCIE2A);
	sei();
}

ISR(TIMER2_COMPA_vect){
	ms_count++;
	if(ms_count%5==0){
		task_5ms();
	}
	else
	{
		//do nothing
	}
	
	if(ms_count%20==0){
		task_20ms();
	}
	else
	{
		//do nothing
	}

	if(ms_count%60==0){
		task_60ms();
		ms_count = 0;
	}
	else
	{
		//do nothing
	}
	
}


