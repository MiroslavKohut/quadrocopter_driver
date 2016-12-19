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
#include <common/delay.h>
#include <device_libraries/MPU_9250.h>


//definitions
#define angle_sampling 				((float)0.05) // integrator sampling in seconds
#define moveing_average_sampling	((float)0.005) //moveing average reading in seconds
#define moveing_average_samples     10             //number of samples used in moveing average

//global variables
float gyroscope_data_avg[3];

//Timers
void TIM4_integrating_timer(int period_in_miliseconds);
void TIM3_sampling_timer(int period_in_miliseconds);
void calculate_angle();

#endif /* FUNCTIONS_H_ */
