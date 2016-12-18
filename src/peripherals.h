/*
 * peripherals.h
 *
 *  Created on: 2. 12. 2016
 *      Author: Stancoj
 */

#ifndef PERIPHERALS_H_
#define PERIPHERALS_H_
#include <stdio.h>

void GPIO_init(void);
void PWM_init(uint8_t value);
void Timer_init(void);
void BLDC_Regulator_calibration(void);

#endif /* PERIPHERALS_H_ */
