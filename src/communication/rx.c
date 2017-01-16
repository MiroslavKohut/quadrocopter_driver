/*
 * rx.c
 *
 *  Created on: 4. 1. 2017
 *      Author: Jaromir stanko
*/
/* Private variables ---------------------------------------------------------*/

#include <communication/rx.h>

void rx_init(){

	IC2Value = 0;
	dutyCycle_throttle = 0.0;
	dutyCycle_yaw = 0.0;
	dutyCycle_pitch = 0.0;
	dutyCycle_roll = 0.0;

	frequency_throttle = 0.0;
	frequency_yaw = 0.0;
	frequency_pitch = 0.0;
	frequency_roll=0.0;

	pulse_length_throttle=0;
	pulse_length_yaw=0;
	pulse_length_pitch=0;
	pulse_length_roll=0;

	NVIC_init();
	GPIO_init();
	Timer_init();
}
void NVIC_init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

 	NVIC_InitStructure.NVIC_IRQChannel = TIM9_IRQn;
 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 	NVIC_Init(&NVIC_InitStructure);

 	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 	NVIC_Init(&NVIC_InitStructure);
 	//nove
 	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 	NVIC_Init(&NVIC_InitStructure);
}

void Timer_init(void)
{
	//TIM3&TIM9 - input PWM
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);




    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x0;

    TIM_PWMIConfig(TIM3, &TIM_ICInitStructure);

    TIM_SelectInputTrigger(TIM3, TIM_TS_TI2FP2);
    TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);
    TIM_SelectMasterSlaveMode(TIM3,TIM_MasterSlaveMode_Enable);
    TIM_ITConfig(TIM3, TIM_IT_CC2, ENABLE);
    TIM_Cmd(TIM3, ENABLE);


    TIM_PWMIConfig(TIM9, &TIM_ICInitStructure);
    TIM_SelectInputTrigger(TIM9, TIM_TS_TI2FP2);
    TIM_SelectSlaveMode(TIM9, TIM_SlaveMode_Reset);
    TIM_SelectMasterSlaveMode(TIM9,TIM_MasterSlaveMode_Enable);
    TIM_ITConfig(TIM9, TIM_IT_CC2, ENABLE);
    TIM_Cmd(TIM9, ENABLE);

    //nove

    TIM_PWMIConfig(TIM4, &TIM_ICInitStructure);
    TIM_SelectInputTrigger(TIM4, TIM_TS_TI2FP2);
    TIM_SelectSlaveMode(TIM4, TIM_SlaveMode_Reset);
    TIM_SelectMasterSlaveMode(TIM4,TIM_MasterSlaveMode_Enable);
    TIM_ITConfig(TIM4, TIM_IT_CC2, ENABLE);

    TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x0;
    TIM_PWMIConfig(TIM4, &TIM_ICInitStructure);

    TIM_PWMIConfig(TIM4, &TIM_ICInitStructure);
    TIM_SelectInputTrigger(TIM4, TIM_TS_TI2FP2);
    TIM_SelectSlaveMode(TIM4, TIM_SlaveMode_Reset);
    TIM_SelectMasterSlaveMode(TIM4,TIM_MasterSlaveMode_Enable);
    TIM_ITConfig(TIM4, TIM_IT_CC4, ENABLE);

    TIM_Cmd(TIM4, ENABLE);




}

void GPIO_init(void)
{
	//PWM input

	GPIO_InitTypeDef GPIO_InitStruct;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

    //Throttle
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_14;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP ;

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_TIM9);
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    //yaw
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP ;

    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3);
    GPIO_Init(GPIOC, &GPIO_InitStruct);

    //roll
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP ;

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4);
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    //pitch
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP ;

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_TIM4);
    GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void TIM9_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM9, TIM_IT_CC2) != RESET)
	{
	  TIM_ClearFlag(TIM9, TIM_IT_CC2);
	  TIM_ClearITPendingBit(TIM9, TIM_IT_CC1);
	  IC2Value = TIM_GetCapture2(TIM9);

	  if (IC2Value != 0)
	  {
		/* Duty cycle computation */
		dutyCycle_throttle = (TIM_GetCapture1(TIM9) * 100) / IC2Value;
		/* Pulse length computation */
		pulse_length_throttle = ceil(161 * dutyCycle_throttle / 100);
		/* Frequency computation */
		frequency_throttle = SystemCoreClock / IC2Value;
	  }
	  else
	  {
		  dutyCycle_throttle = 0;
		  frequency_throttle = 0;
	  }
	}
}

void TIM3_IRQHandler(void)
{

	if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)
	{
	  TIM_ClearFlag(TIM3, TIM_IT_CC2);
	  TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
	  IC2Value = TIM_GetCapture2(TIM3);

	  if (IC2Value != 0)
	  {
		/* Duty cycle computation */
		dutyCycle_yaw = (TIM_GetCapture1(TIM3) * 100) / IC2Value;
		/* Pulse length computation */
		pulse_length_yaw = ceil(161 * dutyCycle_yaw / 100);
		/* Frequency computation */
		frequency_yaw = SystemCoreClock / IC2Value;
	  }
	  else
	  {
		dutyCycle_yaw = 0;
		frequency_yaw = 0;
	  }

	}
}

void TIM4_IRQHandler(void)
{

	if (TIM_GetITStatus(TIM4, TIM_IT_CC2) != RESET)
	{
	  TIM_ClearFlag(TIM4, TIM_IT_CC2);
	  TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
	  IC2Value = TIM_GetCapture2(TIM4);

	  if (IC2Value != 0)
	  {
		/* Duty cycle computation */
		dutyCycle_pitch = (TIM_GetCapture1(TIM4) * 100) / IC2Value;
		/* Pulse length computation */
		pulse_length_pitch = ceil(161 * dutyCycle_pitch / 100);
		/* Frequency computation */
		frequency_pitch = SystemCoreClock / IC2Value;
	  }
	  else
	  {
		dutyCycle_pitch = 0;
		frequency_pitch = 0;
	  }
	}

	if (TIM_GetITStatus(TIM4, TIM_IT_CC4) != RESET)
		{
		  IC2Value = TIM_GetCapture4(TIM4);
		  TIM_ClearFlag(TIM4, TIM_IT_CC4);
		  TIM_ClearITPendingBit(TIM4, TIM_IT_CC3);
		  IC2Value = TIM_GetCapture4(TIM4);

		  if (IC2Value != 0)
		  {
			/* Duty cycle computation */
			dutyCycle_roll = (TIM_GetCapture3(TIM4) * 100) / IC2Value;
			/* Pulse length computation */
			pulse_length_roll = ceil(161 * dutyCycle_roll / 100);
			/* Frequency computation */
			frequency_roll = SystemCoreClock / IC2Value;
		  }
		  else
		  {
			dutyCycle_roll = 0;
			frequency_roll = 0;
		  }
		}
}
