/*
 * usart.h
 *
 *  Created on: 18. 12. 2016
 *      Author: Miroslav Kohút
 */

#ifndef USART_H_
#define USART_H_

#include <stddef.h>
#include "stm32l1xx.h"
#include <stdio.h>


float rec_data;
void usart_init();

void USART_send_function();
void USART_send_function_number(float number);

#endif
