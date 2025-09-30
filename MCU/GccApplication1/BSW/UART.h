/*
 * UART.h
 *
 * Created: 2025-09-22 오후 6:39:23
 *  Author: kym11
 */ 


#ifndef UART_H_
#define UART_H_
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <stdint.h>
#include <avr/io.h>

void UART_Init(uint16_t baud);
void UART_Transmit(char data);
void UART_TransmitADC(uint16_t data);
void UART_SendString(const char* str);

#endif /* UART_H_ */