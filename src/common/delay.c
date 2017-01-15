
/*
 * Delay.c
 *
 *  Created on: 8. 12. 2016
 *      Author: Stancoj
 */
#include <common/delay.h>

__IO static uint32_t time_ms = 0;

void init_delay(){
	SystemInit();
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock/delay_divider_ms);
}

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


