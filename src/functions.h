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

//common functions

//Timers
void TIM2_init(int period_in_miliseconds);

#endif /* FUNCTIONS_H_ */
