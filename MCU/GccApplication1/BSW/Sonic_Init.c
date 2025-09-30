#include <avr/io.h>
#include <avr/interrupt.h>
#include "Sonic_Init.h"

void Sonic_Init(void)
{
	// 1. TRIG 핀 (PD2, PD4, PD6)을 출력으로 설정
	DDRD |= (1 << DDD2) | (1 << DDD4) | (1 << DDD6);
	PORTD &= ~((1 << PORTD2) | (1 << PORTD4) | (1 << PORTD6));

	// 2. ECHO 핀 (PD3, PD5, PD7)을 입력으로 설정
	DDRD &= ~((1 << DDD3) | (1 << DDD5) | (1 << DDD7));

	// 3. Pin Change Interrupt 활성화
	PCICR |= (1 << PCIE2);

	// 4. ECHO 핀에 해당하는 인터럽트 마스크 설정
	PCMSK2 |= (1 << PCINT19) | (1 << PCINT21) | (1 << PCINT23); 
}
