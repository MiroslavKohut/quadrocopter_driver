/*
 * functions.h
 *
 *  Created on: 27. 11. 2016
 *      Author: Miroslav Kohút
 */

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

/* Includes */
#include <stddef.h>
#include "stm32l1xx.h"
#include <stdio.h>

//common includes
#include "common/delay.h"
#include "device_libraries/MPU_9250.h"
#include "device_libraries/motor_control.h"
#include "communication/rx.h"
#include "communication/usart.h"
#include "device_libraries/motor_control.h"

//PID CONSTANTS
#define KP_yaw ((float)0.25)
#define KP_roll ((float)0.25)
#define KP_pitch ((float)0.25)
//constats
#define M_PI_deg 180/M_PI

//definitions
#define angle_sampling 				((float)0.010) // integrator sampling in seconds
#define moveing_average_sampling	((float)0.005) //moveing average reading in seconds
#define moveing_average_samples     3             //number of samples used in moveing average

//global variables
float gyroscope_data_avg[3];
float accelerometer_data_avg[3];

float roll;
float pitch;
float yaw;
float desired_roll;
float desired_pitch;
float desired_yaw;

float nominal_throttle;
//Timers
void TIM2_PWM_init(void);
void PWM_init(void);
void GPIO_PWM_init(void);
void BLDC_Regulator_calibration(void);
void TIM4_integrating_timer(int period_in_miliseconds);
void TIM5_sampling_timer(int period_in_miliseconds);
float commute_pid(float regulation_error);
void complementary_filter();
void PID_stabilization_control();
void PID_yaw_control();
void PID_pitch_control();
void PID_roll_control();
void global_init();
void timers_init();

#endif /* FUNCTIONS_H_ */
