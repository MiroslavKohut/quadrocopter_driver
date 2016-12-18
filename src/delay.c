/*
 * Delay.c
 *
 *  Created on: 8. 12. 2016
 *      Author: Stancoj
 */
#include "delay.h"
#include "stm32l1xx.h"

__IO static uint32_t time_ms = 0;

void delay_decrement (void)
{
	  if (time_ms > 0x00)
	  {
		  time_ms--;
	  }
}

void delay_ms(uint32_t t)
{
	time_ms = t;
	while(time_ms > 0)
	{
		// decrement from hero to zero
	}
}
