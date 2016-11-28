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

#define WHO_AM_I 0x71
#define TEMP_OUT_H 0x41
#define TEMP_OUT_L 0x42
#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40
#define GYRO_XOUT_H 0x43
#define GYRO_XOUT_L 0x44
#define GYRO_YOUT_H 0x45
#define GYRO_YOUT_L 0x46
#define GYRO_ZOUT_H 0x47
#define GYRO_ZOUT_L 0x48

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
//SPI
uint8_t SPI1_receive_send(uint8_t data);

#endif /* FUNCTIONS_H_ */
