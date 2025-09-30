#ifndef MOTOR_CONTROL_H_
#define MOTOR_CONTROL_H_

#include <stdint.h>

void Servo_SetAngle(uint16_t steer);
void Motor_Speed(uint16_t speed, uint16_t brek);

#endif /* MOTOR_CONTROL_H_ */ 