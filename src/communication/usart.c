/*
 * usart.c
 *
 *  Created on: 18. 12. 2016
 *      Author: Miroslav Kohút
 */

#include <communication/usart.h>

void usart_init(){

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

	GPIO_InitTypeDef GPIO_usart;

	GPIO_usart.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_usart.GPIO_Mode = GPIO_Mode_AF;
	GPIO_usart.GPIO_OType = GPIO_OType_PP;
	GPIO_usart.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_usart.GPIO_Speed = GPIO_Speed_40MHz;

	GPIO_Init(GPIOA,&GPIO_usart);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	USART_Cmd(USART2, ENABLE);

	//interrupt
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	  /* Enable the USARTx Interrupt */
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void USART_send_function(char text[]){

	uint16_t i = 0;
	while(text[i] != '\0'){
		USART_SendData(USART2, text[i]);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
		i++;
	}
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);

}
void USART_send_function_number(float number){

	uint16_t i = 0;
	char text[20];
	int num1 = (int)number;
	int num2 = (int)((number-num1)*1000);
	if (num2 > 0)
		sprintf(text,"%d.%d", num1, num2);
	else{
		num2=num2*-1;
		sprintf(text,"%d.%d", num1, num2);
	}
	while(text[i] != '\0'){
		USART_SendData(USART2, text[i]);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
		i++;
	}
	USART_SendData(USART2,'\r');
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);

}
/*
void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		rec_data = USART_ReceiveData(USART2);
		if (rec_data == '+'){
			TIM2->CCR1 += 5;
			TIM2->CCR2 += 5;
			TIM2->CCR3 += 5;
			TIM2->CCR4 += 5;
		}
		else if (rec_data == '-'){
			TIM2->CCR1 -= 5;
			TIM2->CCR2 -= 5;
			TIM2->CCR3 -= 5;
			TIM2->CCR4 -= 5;
		}
		else{
			TIM2->CCR1 = 100;
			TIM2->CCR2 = 100;
			TIM2->CCR3 = 100;
			TIM2->CCR4 = 100;
		}
    }
}*/

