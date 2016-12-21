/*
 * motor_control.c
 *
 *  Created on: 21. 12. 2016
 *      Author: Asus
 */
#include <device_libraries/motor_control.h>

void motor_init(){

	 TIM2_PWM_init();
	 PWM_init();
	 GPIO_PWM_init();
	 delay_ms(200);
	 for (int i=1;i<=4;i++){
		 set_throttle(i, 0);
	 }
	 delay_ms(1000);
}

void motor_start(){
	 for (int i=1;i<=4;i++){
		 set_throttle(i,NOMINAL_THROTTLE/2);
	 }
	 current_throttle = NOMINAL_THROTTLE/2;
	 delay_ms(1000);
	 for (int i=1;i<=4;i++){
		 set_throttle(i,NOMINAL_THROTTLE);
	 }
	 current_throttle =  NOMINAL_THROTTLE;
}

void set_throttle(uint8_t motor, int8_t data){

	switch(motor)
	{
		case 1: {
			TIM2->CCR1 = 100+(int8_t)(data);
			break;
		}
		case 2: {
			TIM2->CCR2 = 100+(int8_t)(data);
			break;
		}
		case 3: {
			TIM2->CCR3 = 100+(int8_t)(data);
			break;
		}
		case 4: {
			TIM2->CCR4 = 100+(int8_t)(data);
			break;
		}
	}
	return;
}

void TIM2_PWM_init(void)
{

	TIM_TimeBaseInitTypeDef TIM_BaseStruct;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_BaseStruct.TIM_Prescaler = 160;
	TIM_BaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_BaseStruct.TIM_Period = 1999; // f[Hz] PWM bude 50hz, nastavenie periody casovaca
    TIM_BaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;

    TIM_TimeBaseInit(TIM2, &TIM_BaseStruct);
    TIM_Cmd(TIM2, ENABLE);
}

void PWM_init(void)
{
	TIM_OCInitTypeDef TIM_OCStruct;
	uint16_t pulse_length = 0;

    TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OCStruct.TIM_Pulse = pulse_length;
    TIM_OC1Init(TIM2, &TIM_OCStruct);
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_OC2Init(TIM2, &TIM_OCStruct);
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_OC3Init(TIM2, &TIM_OCStruct);
    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_OC4Init(TIM2, &TIM_OCStruct);
    TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
}

void GPIO_PWM_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct, GPIO_Button;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM2);
    GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_TIM2);
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_Button.GPIO_Pin = GPIO_Pin_13;
    GPIO_Button.GPIO_OType = GPIO_OType_PP;
    GPIO_Button.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Button.GPIO_Mode = GPIO_Mode_IN;
    GPIO_Init(GPIOC, &GPIO_Button);
}

void BLDC_Regulator_calibration(void)
{
	uint8_t button = 0, buttonState = 0;

	while(1)
	{
	  button = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13);


	  if (button == 0)
	  {
		  while (button == 0)
		  {
			  button = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13);
		  }
		  if (buttonState == 0)
		  {
			  buttonState = 1;
		  }
		  else
		  {
			  buttonState = 0;
		  }

		  if (buttonState == 1)
		  {

			  TIM2->CCR1 = 100;
			  TIM2->CCR2 = 100;
			  TIM2->CCR3 = 100;
			  TIM2->CCR4 = 100;
		  }
		  else if (buttonState == 0)
		  {
			  TIM2->CCR1 = 120;
			  TIM2->CCR2 = 120;
			  TIM2->CCR3 = 120;
			  TIM2->CCR4 = 120;
			  delay_ms(10000);
			  TIM2->CCR1 = 100;
			  TIM2->CCR2 = 100;
			  TIM2->CCR3 = 100;
			  TIM2->CCR4 = 100;
			  delay_ms(3000);
			  break;
		  }
	  }
	}
}
