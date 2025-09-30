/*
 * Vehicle_Control.c
 *
 * Created: 2025-09-22 오후 3:37:16
 * Author: kym11
 */

#include "Vehicle_Control.h"
#include "../BSW/UART.h"
#include "../ReadAdc/ADC.h"
#include "../LCDControl/LCD.h"
#include "../MotorControl/Motor_Control.h"
#include <avr/sleep.h>

int previous_mode = 0xFF; 
Mode mode = MODE_NONE;
uint8_t button_active[NUM_BUTTONS] = {0};

void Speed_Control(uint16_t speed, uint16_t brek){
     if(mode != previous_mode) {
         switch(mode){
             case MODE_NONE:
                 lcd_clear();
                 break;
             case MODE_EMERGENCY_LEFT:
                 lcd_EMERGENCY_LEFT();
                 break;
             case MODE_EMERGENCY_RIGHT:
                 lcd_EMERGENCY_RIGHT();
                 break;
             case MODE_EMERGENCY_CENTER:
                 lcd_EMERGENCY_CENTER();
                 break;
             default:
                 break;
         }
         previous_mode = (int)mode;
     }

     if (mode == MODE_EMERGENCY_CENTER) {
         Motor_Speed(0, 0);
     } else {
         Motor_Speed(speed, brek);
     }
}

void Steering_Control(uint16_t steer){
    switch(mode){
        case MODE_NONE:
            Servo_SetAngle(steer);
            break;
        case MODE_EMERGENCY_LEFT:
            Servo_SetAngle(GORIGHT);
            break;
        case MODE_EMERGENCY_RIGHT:
            Servo_SetAngle(GOLEFT);
            break;
        case MODE_EMERGENCY_CENTER:
            Servo_SetAngle(steer);
            break;
        default:
            break;
    }
}

//버튼이 켜졌는지 꺼졌는지 확인
void button_control(void) {
	static uint8_t last_detected_button = 0;
	uint8_t current_button = get_button_from_adc(button_val);
	if (current_button != last_detected_button) {
		if (current_button != 0) {
			uint8_t idx = current_button - 1;
			button_active[idx] = !button_active[idx];
			switch (current_button) {
				case BUTTON_HOTASS:
				if (button_active[idx]) UART_SendString("엉뜨를 켰습니다");
				else UART_SendString("엉뜨를 껐습니다\r\n");
				break;
				case BUTTON_AC:
				if (button_active[idx]) UART_SendString("에어컨을 켰습니다");
				else UART_SendString("에어컨을 껐습니다\r\n");
				break;
				case BUTTON_SORRY:
				if (button_active[idx]) UART_SendString("비상등을 켰습니다");
				else UART_SendString("비상등을 껐습니다\r\n");
				break;
				case BUTTON_GETOUT:
				if (button_active[idx]) UART_SendString("상향등을 켰습니다");
				else UART_SendString("상향등을 껐습니다\r\n");
				break;
				case BUTTON_PROGRAMOFF:
				if (button_active[idx]) {
					UART_SendString("프로그램을 종료하겠습니다\r\n");
					set_sleep_mode(SLEEP_MODE_PWR_DOWN);
					sleep_enable();
					sleep_cpu();
				}
				break;
				default:
				break;
			}
		}
		last_detected_button = current_button;
	}
}