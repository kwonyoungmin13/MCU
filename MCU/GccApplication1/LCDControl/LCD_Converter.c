/*
 * LCD_Converter.c
 *
 * Created: 2025-09-29 오전 3:34:22
 *  Author: kym11
 */ 
#include "lcd_converter.h"

// 문자열을 뒤집기 위한 내부용(static) 함수
static void reverse(char* str, int len) {
	int i = 0, j = len - 1;
	while (i < j) {
		char temp = str[i];
		str[i] = str[j];
		str[j] = temp;
		i++;
		j--;
	}
}

// 직접 구현한 my_itoa 함수
int my_itoa(int value, char* buffer) {
	int i = 0;
	
	if (value == 0) {
		buffer[i++] = '0';
		buffer[i] = '\0';
		return i;
	}
	
	while (value > 0) {
		int remainder = value % 10;
		buffer[i++] = remainder + '0';
		value = value / 10;
	}
	
	buffer[i] = '\0';
	
	reverse(buffer, i);
	
	return i;
}