/*
 * Vehicle_Control.h
 *
 * Created: 2025-09-22 오후 3:37:02
 *  Author: kym11
 */ 


#ifndef VEHICLE_CONTROL_H_
#define VEHICLE_CONTROL_H_

#include <stdint.h>
#include "../Common/defs.h"

#define BRIGHT 512

void Speed_Control(uint16_t speed, uint16_t brek);
void Steering_Control(uint16_t steer);
void button_control(void);

#endif /* VEHICLE_CONTROL_H_ */