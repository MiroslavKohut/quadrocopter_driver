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

/* IMU 9250 registers define */

/* Private macro */

/* Private variables */

/* Private function prototypes */

/* Private functions */

// init functions
void usart_init();
void init_SPI1();

//common functions
//Usart
void USART_send_function();
void USART_send_function_number(float number);
//sleep
void sleep(uint64_t time);

void chip_select(void);
void chip_deselect(void);
uint16_t  read_reg( uint8_t WriteAddr);
void write_reg( uint8_t WriteAddr, uint8_t WriteData);


#endif /* FUNCTIONS_H_ */

/* ---- SLEEP DEFINITIONS ----------------------------------------------- */
#define SLEEP_50_us 37
