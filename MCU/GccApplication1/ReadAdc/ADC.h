/*
 * ADC.h
 *
 * Created: 2025-09-20 오후 1:10:57
 *  Author: kym11
 */ 
#ifndef ADC_H_
#define ADC_H_

// ADC channels
#define BREAK 0
#define MOTOR 1
#define STEER 2
#define BUTTON 3

#include <stdint.h>

extern volatile uint16_t steer;
extern volatile uint16_t motor;
extern volatile uint16_t brek;
extern volatile uint16_t button_val;

void ADC_Start(void);

#endif /* ADC_H_ */