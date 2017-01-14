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

//project library includes
#include "common/delay.h"

#include "device_libraries/MPU_9250.h"
#include "device_libraries/motor_control.h"

#include "communication/rx.h"
#include "communication/usart.h"

/* PID parameters */
#define KP_yaw ((float)0.25)
#define KP_roll ((float)0.25)
#define KP_pitch ((float)0.25)

/* Controller configurations */

#define MAXIMUM_SATURATION_YAW		15
#define MAXIMUM_SATURATION_PITCH	20
#define MAXIMUM_SATURATION_ROLL		20

#define MINIMUM_SATURATION			5

#define THROTTLE_CONSTANT			100
#define YAW_CONSTANT				150

/* Constants */
#define M_PI_deg 					180/M_PI

/* Timer configurations */
#define angle_sampling 				((float)0.010) // integrator sampling in seconds
#define moveing_average_sampling	((float)0.005) //moveing average reading in seconds
#define moveing_average_samples     3             //number of samples used in moveing average

#define DEFAULT_FREQUENCY			16000000	//mame 16MHz vstup!!!
#define INPUT_CLOCK					10000		//input clock = 10000Hz = 0,1ms

/* Global variables */
float gyroscope_data_avg[3];
float accelerometer_data_avg[3];

float roll;
float pitch;
float yaw;
float desired_roll;
float desired_pitch;
float desired_yaw;

/*** Functions ****/
/* Main initializations */
void global_init();
void timers_init();

/* Timer inits */
void TIM4_controller_timer(int period_in_miliseconds);
void TIM5_sampling_timer(int period_in_miliseconds);

/* Filter Roll/Pitch */
void complementary_filter();

/*Controller functions*/
void PID_stabilization_control();
void PID_yaw_control();
void PID_pitch_control();
void PID_roll_control();

#endif /* FUNCTIONS_H_ */
