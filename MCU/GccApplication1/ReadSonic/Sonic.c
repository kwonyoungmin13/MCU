/*
 * Sonic.c
 *
 * Created: 2025-09-22 오전 11:19:41
 *  Author: kym11
 */ 

#include "Sonic.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "../VehicleControl/Vehicle_Control.h"
#include "../BSW/UART.h"
#include "../LCDControl/LCD.h"

// --- 모듈 내부 변수 (Module-Internal Variables) ---
static volatile uint16_t pulse_start_time[NUM_SENSORS];
static volatile uint8_t last_pind_state = 0;

// --- 전역 변수 정의 ---
volatile uint16_t g_pulse_duration[NUM_SENSORS] = {0};
volatile uint8_t g_is_measured[NUM_SENSORS] = {0};
volatile uint16_t distance_cm[NUM_SENSORS] = {0};

static void Read_Sonic(void)
{
	// LEFT TRIG: PD2
	PORTD |= (1 << PORTD2);
	_delay_us(10);
	PORTD &= ~(1 << PORTD2);
	// CENTER TRIG: PD4
	PORTD |= (1 << PORTD4);
	_delay_us(10);
	PORTD &= ~(1 << PORTD4);
	// RIGHT TRIG: PD6
	PORTD |= (1 << PORTD6);
	_delay_us(10);
	PORTD &= ~(1 << PORTD6);	
}

static void GetDistance(uint8_t index){
	if (g_is_measured[index] == 1) {
		distance_cm[index] = (uint16_t)((unsigned long)g_pulse_duration[index] * 5 / 58 / 10);
		if(distance_cm[index] <= 10){
			switch(index){
				case SENSOR_LEFT:
					mode = MODE_EMERGENCY_LEFT;
					break;
				case SENSOR_CENTER:
					mode = MODE_EMERGENCY_CENTER;
					break;
				case SENSOR_RIGHT:
					mode = MODE_EMERGENCY_RIGHT;
					break;
				default:
					break;
			}
		}
		g_is_measured[index] = 0;
	}
}

static void ModeUdate(void){
	if(distance_cm[0] > 10 && distance_cm[1] > 10 && distance_cm[2] > 10) mode = MODE_NONE;
}

void Sonic_Task(void){
	Read_Sonic();
	ModeUdate();
}

ISR(PCINT2_vect)
{
	uint8_t current_pind_state = PIND;
	uint8_t changed_bits = current_pind_state ^ last_pind_state;

	// Sensor 0 (PD3 echo)
	if (changed_bits & (1 << PIND3)) {
		if (current_pind_state & (1 << PIND3)) {
			pulse_start_time[SENSOR_LEFT] = TCNT1;
		} else {
			g_pulse_duration[SENSOR_LEFT] = TCNT1 - pulse_start_time[SENSOR_LEFT];
			g_is_measured[SENSOR_LEFT] = 1;
			GetDistance(SENSOR_LEFT);
		}
	}

	// Sensor 1 (PD5 echo)
	if (changed_bits & (1 << PIND5)) {
		if (current_pind_state & (1 << PIND5)) {
			pulse_start_time[SENSOR_CENTER] = TCNT1;
		} else {
			g_pulse_duration[SENSOR_CENTER] = TCNT1 - pulse_start_time[SENSOR_CENTER];
			g_is_measured[SENSOR_CENTER] = 1;
			GetDistance(SENSOR_CENTER);
		}
	}
	
	// Sensor 2 (PD7 echo)
	if (changed_bits & (1 << PIND7)) {
		if (current_pind_state & (1 << PIND7)) {
			pulse_start_time[SENSOR_RIGHT] = TCNT1;
		} else {
			g_pulse_duration[SENSOR_RIGHT] = TCNT1 - pulse_start_time[SENSOR_RIGHT];
			g_is_measured[SENSOR_RIGHT] = 1;
			GetDistance(SENSOR_RIGHT);
		}
	}
	last_pind_state = current_pind_state;
}