#include <avr/io.h>
#include "Motor_Control.h"

// Servo/Motor PWM configuration
#define ICR1_TOP 39999
#define OCR_MIN  1000
#define OCR_MAX  5000

void Servo_SetAngle(uint16_t steer) {
	uint8_t angle = (steer * 180L) / 1023;
	uint16_t ocr_value = OCR_MIN + (uint16_t)(((float)angle / 180.0) * (OCR_MAX - OCR_MIN));
	OCR1A = ocr_value;
}

void Motor_Speed(uint16_t speed, uint16_t brek){
	if(speed > brek) speed = speed - brek;
	else speed = 0;
	OCR1B = ((uint32_t)speed) * 39999 / 1024;
} 