#ifndef COMMON_DEFS_H_
#define COMMON_DEFS_H_

#include <stdint.h>

// 시스템 모드
typedef enum {
	MODE_NONE = 0,
	MODE_EMERGENCY_LEFT = 1,
	MODE_EMERGENCY_CENTER = 2,
	MODE_EMERGENCY_RIGHT = 3
} Mode;

// 차량버튼인덱스
typedef enum {
	BUTTON_HOTASS = 1,
	BUTTON_AC = 2,
	BUTTON_SORRY = 3,
	BUTTON_GETOUT = 4,
	BUTTON_PROGRAMOFF = 5
} ButtonId;

// 초음파위치
typedef enum {
	SENSOR_LEFT = 0,
	SENSOR_CENTER = 1,
	SENSOR_RIGHT = 2
} SensorIndex;

#define NUM_BUTTONS 5

// 버튼 ADC 임계값
#define BTN_1_LOW 150
#define BTN_1_HIGH 230
#define BTN_2_LOW 320
#define BTN_2_HIGH 410
#define BTN_3_LOW 500
#define BTN_3_HIGH 590
#define BTN_4_LOW 710
#define BTN_4_HIGH 790
#define BTN_5_LOW 980

// 차량모드
extern Mode mode;

//비상상황시 조향값
#define GOLEFT 100
#define GORIGHT 1000

#endif /* COMMON_DEFS_H_ */ 