/*
 * LCD.h
 *
 * Created: 2025-09-29 오전 3:32:47
 *  Author: kym11
 */ 


#ifndef LCD_H_
#define LCD_H_

#include <stdint.h>

// 사용자 설정: I2C LCD 모듈의 주소
#define LCD_ADDRESS   0x27

// 명령어/데이터 전송 플래그
#define FLAG_RS_CMD   0
#define FLAG_RS_DATA  1

// 공개 함수 목록 (API)
void lcd_init(void);
void lcd_send_string(char *str);
void lcd_clear(void);
void lcd_goto_xy(uint8_t row, uint8_t col);
void lcd_send(uint8_t data, uint8_t rs_flag);
void lcd_EMERGENCY_LEFT(void);
void lcd_EMERGENCY_RIGHT(void);
void lcd_EMERGENCY_CENTER(void);

#endif /* LCD_H_ */