/*
 * LCD.c
 *
 * Created: 2025-09-29 오전 3:33:14
 *  Author: kym11
 */ 
#include <avr/io.h>
#include "LCD.h"
#include <util/delay.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

// PCF8574와 LCD의 핀 연결 정의
#define PIN_RS        (1 << 0)
#define PIN_RW        (1 << 1)
#define PIN_E         (1 << 2)
#define PIN_BACKLIGHT (1 << 3)
#define PINS_DATA     0xF0 // 데이터 핀 마스크 (D7-D4)

// 내부 함수 선언
static void i2c_init(void);
static void i2c_start(void);
static void i2c_stop(void);
static void i2c_write(uint8_t data);
static uint8_t i2c_read(uint8_t ack); // I2C 읽기 함수 추가
static void lcd_send_i2c(uint8_t data, uint8_t write_mode); // 모드 파라미터 추가
static void lcd_write_nibble(uint8_t nibble);
static uint8_t lcd_read_busy_flag(void); // Busy Flag 읽기 함수 추가
static void lcd_wait_busy(void); // Busy 대기 함수 추가

// I2C 초기화 (안정성을 위해 50kHz로 설정)
static void i2c_init(void) {
	TWSR = 0x00;
	TWBR = 152; // 50kHz SCL @ 16MHz F_CPU
	TWCR = (1 << TWEN);
}

static void i2c_start(void) {
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
}

static void i2c_stop(void) {
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

static void i2c_write(uint8_t data) {
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
}

// I2C 읽기 함수 (ACK/NACK 제어)
static uint8_t i2c_read(uint8_t ack) {
	TWCR = (1 << TWINT) | (1 << TWEN) | (ack ? (1 << TWEA) : 0);
	while (!(TWCR & (1 << TWINT)));
	return TWDR;
}

static void lcd_send_i2c(uint8_t data, uint8_t write_mode) {
	i2c_start();
	i2c_write((LCD_ADDRESS << 1) | (write_mode ? 1 : 0)); // 0: 쓰기, 1: 읽기
	if (write_mode == 0) { // 쓰기 모드일 때만 데이터 전송
		i2c_write(data);
	}
}

// 4비트 데이터(니블)만 전송 (초기화용)
static void lcd_write_nibble(uint8_t nibble) {
	uint8_t data = (nibble << 4) | PIN_BACKLIGHT;
	lcd_send_i2c(data | PIN_E, 0);
	i2c_stop();
	_delay_us(1); // E 펄스 폭 유지를 위한 최소 Delay
	lcd_send_i2c(data, 0);
	i2c_stop();
}


// --- Busy Flag 관련 함수 (새로 추가) ---
static uint8_t lcd_read_busy_flag(void) {
	uint8_t high_nibble, low_nibble;

	lcd_send_i2c(PINS_DATA | PIN_RW | PIN_BACKLIGHT, 0); // 데이터 핀을 입력으로 설정 + RW=1(읽기)
	i2c_stop();
	
	// 상위 니블 읽기 (D7~D4)
	lcd_send_i2c(PINS_DATA | PIN_RW | PIN_BACKLIGHT | PIN_E, 0);
	i2c_stop();
	_delay_us(1);
	lcd_send_i2c(0, 1); // 읽기 모드로 주소 전송
	high_nibble = i2c_read(1); // ACK와 함께 데이터 읽기
	i2c_stop();
	lcd_send_i2c(PINS_DATA | PIN_RW | PIN_BACKLIGHT, 0); // E핀 내리기
	i2c_stop();

	// 하위 니블 읽기 (D3~D0, 실제로는 주소 카운터 값)
	lcd_send_i2c(PINS_DATA | PIN_RW | PIN_BACKLIGHT | PIN_E, 0);
	i2c_stop();
	_delay_us(1);
	lcd_send_i2c(0, 1); // 읽기 모드로 주소 전송
	low_nibble = i2c_read(0); // NACK와 함께 데이터 읽기
	i2c_stop();
	lcd_send_i2c(PINS_DATA | PIN_RW | PIN_BACKLIGHT, 0); // E핀 내리기
	i2c_stop();

	return (high_nibble & 0x80); // D7핀이 Busy Flag
}

static void lcd_wait_busy(void) {
	while (lcd_read_busy_flag());
}
// --- 여기까지 추가 ---

// 공개 함수 구현
void lcd_send(uint8_t data, uint8_t rs_flag) {
	lcd_wait_busy(); // <--- 여기가 핵심! 긴 Delay를 대체합니다.

	uint8_t rs_val = (rs_flag == FLAG_RS_DATA) ? PIN_RS : 0;
	
	// 상위 니블 전송
	uint8_t high_nibble = (data & 0xF0) | rs_val | PIN_BACKLIGHT;
	lcd_send_i2c(high_nibble | PIN_E, 0);
	i2c_stop();
	_delay_us(1); // E 펄스 폭 유지를 위한 최소 Delay
	lcd_send_i2c(high_nibble, 0);
	i2c_stop();

	// 하위 니블 전송
	uint8_t low_nibble = ((data << 4) & 0xF0) | rs_val | PIN_BACKLIGHT;
	lcd_send_i2c(low_nibble | PIN_E, 0);
	i2c_stop();
	_delay_us(1); // E 펄스 폭 유지를 위한 최소 Delay
	lcd_send_i2c(low_nibble, 0);
	i2c_stop();
}

void lcd_init(void) {
	i2c_init();
	_delay_ms(50); // 전원 안정화 대기 (필수)

	// 4비트 모드 진입을 위한 초기화 시퀀스 (데이터시트 기반, 필수)
	lcd_write_nibble(0x03); _delay_ms(5);
	lcd_write_nibble(0x03); _delay_us(200);
	lcd_write_nibble(0x03); _delay_us(200);
	lcd_write_nibble(0x02); // 4비트 모드로 최종 진입

	// 4비트 모드 설정 후 기본 설정
	lcd_send(0x28, FLAG_RS_CMD); // 기능: 4비트, 2라인, 5x8 폰트
	lcd_send(0x0C, FLAG_RS_CMD); // 디스플레이: ON, 커서 OFF
	lcd_send(0x06, FLAG_RS_CMD); // 엔트리 모드: 커서 우측 이동
	lcd_clear();
}

void lcd_send_string(char *str) {
	while (*str) {
		lcd_send(*str++, FLAG_RS_DATA);
	}
}

void lcd_clear(void) {
	lcd_send(0x01, FLAG_RS_CMD);
	lcd_wait_busy(); // Clear 명령어는 특히 오래 걸리므로 Busy 대기
}

void lcd_goto_xy(uint8_t row, uint8_t col) {
	uint8_t address = col + (row == 1 ? 0x40 : 0x00);
	lcd_send(0x80 | address, FLAG_RS_CMD);
}

void lcd_EMERGENCY_LEFT(){
		lcd_goto_xy(0, 0);
		lcd_send_string("Emergency!!");
		lcd_goto_xy(1, 0);
		lcd_send_string(">> Go LEFT << ");
}

void lcd_EMERGENCY_RIGHT(){
	lcd_goto_xy(0, 0);
	lcd_send_string("Emergency!!");
	lcd_goto_xy(1, 0);
	lcd_send_string(">> Go RIGHT << ");
}

void lcd_EMERGENCY_CENTER(){
	lcd_goto_xy(0, 0);
	lcd_send_string("Emergency!!");
	lcd_goto_xy(1, 0);
	lcd_send_string(">> STOP << ");
}