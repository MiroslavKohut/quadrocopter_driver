
/*
 * Delay.c
 *
 *  Created on: 8. 12. 2016
 *      Author: Stancoj
 */
#include <common/delay.h>

__IO static uint32_t time_us = 0;

void init_delay(){
	SystemInit();
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock/delay_divider_us);
}

void delay_decrement (void)
{
	  if (time_us > 0x00)
	  {
		  time_us--;
	  }
}

void delay_ms(uint32_t t)
{
	time_us = t;
	while(time_us > 0)
	{
		// decrement from hero to zero
	}
}


