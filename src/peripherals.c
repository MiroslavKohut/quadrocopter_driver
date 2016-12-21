/*
 * peripherals.c
 *
 *  Created on: 2. 12. 2016
 *      Author: Stancoj
 */
#include "peripherals.h"
#include <stddef.h>
#include "stm32l1xx.h"
#include <stdio.h>

//Driver

void Timer_init(void)
{

	TIM_TimeBaseInitTypeDef TIM_BaseStruct;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_BaseStruct.TIM_Prescaler = 0;
	TIM_BaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_BaseStruct.TIM_Period = 1999; // f[Hz] PWM bude 10KHz - 32MHz/10KHz - 1
    TIM_BaseStruct.TIM_ClockDivision = 0;

    TIM_TimeBaseInit(TIM2, &TIM_BaseStruct);
    TIM_Cmd(TIM2, ENABLE);

    RCC_ClocksTypeDef RCC_Clocks;
    RCC_GetClocksFreq(&RCC_Clocks);
}

void PWM_init(uint8_t duty_cycle)
{
	TIM_OCInitTypeDef TIM_OCStruct;
	uint16_t pulse_length = ((2000 * duty_cycle) / 100) - 1;

    TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;

    TIM_OCStruct.TIM_Pulse = pulse_length;
    TIM_OC2Init(TIM2, &TIM_OCStruct);
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);

}

void GPIO_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM2);
    GPIO_Init(GPIOA, &GPIO_InitStruct);

}
