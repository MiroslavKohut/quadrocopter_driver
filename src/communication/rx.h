/*
 * rx.h
 *
 *  Created on: 4. 1. 2017
 *      Author: Jaromir Stanko
 */

#ifndef COMMUNICATION_RX_H_
#define COMMUNICATION_RX_H_

#include "stm32l1xx_it.h"
#include <common/delay.h>
#include <math.h>

/* Global variables ---------------------------------------------------------*/

#include <communication/rx.h>

__IO float IC2Value;
__IO float IC2Value_roll;
__IO float IC2Value_yaw;
__IO float dutyCycle_throttle;
__IO float dutyCycle_yaw;
__IO float dutyCycle_pitch;
__IO float frequency_throttle;
__IO float frequency_yaw;
__IO float frequency_pitch;
__IO int16_t pulse_length_throttle;
__IO int16_t pulse_length_yaw;
__IO int16_t pulse_length_pitch;

__IO float dutyCycle_roll;
__IO float frequency_roll;
__IO int16_t pulse_length_roll;

void NVIC_init(void);
void Timer_init(void);
void GPIO_init(void);
void rx_init();

#endif /* COMMUNICATION_RX_H_ */
