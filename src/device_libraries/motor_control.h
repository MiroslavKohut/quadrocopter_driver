/*
 * motor_control.h
 *
 *  Created on: 21. 12. 2016
 *      Author: Asus
 */

#ifndef DEVICE_LIBRARIES_MOTOR_CONTROL_H_
#define DEVICE_LIBRARIES_MOTOR_CONTROL_H_

#define NOMINAL_THROTTLE 30
#define MAX_THROTTLE 25

#include <stddef.h>
#include "stm32l1xx.h"
#include <stdio.h>

//common includes

#include <common/delay.h>

uint8_t current_throttle;
void TIM2_PWM_init(void);
void PWM_init(void);
void GPIO_PWM_init(void);
void BLDC_Regulator_calibration(void);
void motor_init();
void set_throttle(uint8_t motor, int8_t data);

#endif /* DEVICE_LIBRARIES_MOTOR_CONTROL_H_ */
