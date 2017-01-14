/*
 * spi.h
 *
 *  Created on: 18. 12. 2016
 *      Author: Miroslav Kohút
 */

#ifndef SPI_H_
#define SPI_H_

/* Includes */
#include <stddef.h>
#include "stm32l1xx.h"
#include <stdio.h>

/* Initialization */
void init_SPI1();

/* Functions for write and read */
void write_reg( uint8_t WriteAddr, uint8_t WriteData);
uint8_t  read_reg( uint8_t WriteAddr);
void read_regs( uint8_t ReadAddr, uint8_t *ReadBuf, unsigned int Bytes );

/* Support functions */
void chip_select();
void chip_deselect();

#endif
