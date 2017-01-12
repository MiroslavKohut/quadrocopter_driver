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
__IO float DutyCycle;
__IO float Frequency;
__IO int16_t pulse_length_throttle;
__IO int16_t pulse_length_yaw;

void NVIC_init(void);
void Timer_init(void);
void GPIO_init(void);
void rx_init();

#endif /* COMMUNICATION_RX_H_ */
