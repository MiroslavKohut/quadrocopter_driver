/*
 * usart.h
 *
 *  Created on: 18. 12. 2016
 *      Author: Miroslav Kohút
 *      Usart library is mainly used for debug and calibrations
 */

#ifndef USART_H_
#define USART_H_

/* Includes */
#include <stddef.h>
#include "stm32l1xx.h"
#include <stdio.h>

/* Define variables */

#define	BAUDRATE 9600

/* Global variables */
float rec_data;

/* Functions */
/* Initialization*/
void usart_init();

void USART_send_function();
void USART_send_function_number(float number);

#endif
