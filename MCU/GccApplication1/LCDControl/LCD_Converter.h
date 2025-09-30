/*
 * LCD_Converter.h
 *
 * Created: 2025-09-29 오전 3:33:58
 *  Author: kym11
 */ 


#ifndef LCD_CONVERTER_H_
#define LCD_CONVERTER_H_

// 직접 구현한 초경량 itoa 함수 (양수만 처리)
// value: 변환할 숫자, buffer: 변환된 문자열을 저장할 배열
// 반환값: 변환된 문자열의 길이
int my_itoa(int value, char* buffer);

#endif /* LCD_CONVERTER_H_ */