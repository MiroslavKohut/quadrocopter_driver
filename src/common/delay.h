
/*
 * delay.h
 *
 *  Created on: 8. 12. 2016
 *      Author: Stancoj
 */

/*
 * delay_decrement needs to be called in stm32l1xx_it.c in function SysTick_Handler
 */

#ifndef DELAY_H_
#define DELAY_H_

#include <stdint.h>
#include "stm32l1xx.h"

#define delay_divider_ms 1000
void init_delay();
void delay_ms(uint32_t t);
void delay_decrement(void);

#endif /* DELAY_H_ */
