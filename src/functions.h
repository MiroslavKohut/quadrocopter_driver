/*
 * functions.h
 *
 *  Created on: 27. 11. 2016
 *      Author: Asus
 */

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

/* Includes */
#include <stddef.h>
#include "stm32l1xx.h"
#include <stdio.h>

/* Private typedef */

/* Private define  */

/* Private macro */

/* Private variables */

/* Private function prototypes */

/* Private functions */


void usart_init();
void USART_send_function();
void USART_send_function_number(float number);

#endif /* FUNCTIONS_H_ */
