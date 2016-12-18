/*
 * functions.c
 *
 *  Created on: 27. 11. 2016
 *      Author: Asus
 */
/* Includes */
#include <functions.h>

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
}

void USART_send_function(char text[]){

	uint16_t i = 0;
	while(text[i] != '\0'){
		USART_SendData(USART2, text[i]);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
		i++;
	}
	//USART_SendData(USART2,'\r');
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

void init_SPI1(void){

	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;

	// enable clock for used IO pins
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	// connect SPI1 pins to SPI alternate function
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);

	/* Configure the chip select pin
	   in this case we will use PA8 */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	// enable peripheral clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	/* configure SPI1 in Mode 0
	 * CPOL = 0 --> clock is low when idle
	 * CPHA = 0 --> data is sampled at the first edge
	 */
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex; // set to full duplex mode, seperate MOSI and MISO lines
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;     // transmit in master mode, NSS pin has to be always high
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b; // one packet of data is 8 bits wide
	//SPI_InitStruct.SPI_CRCPolynomial = SPI_CRC_Rx;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;        // clock is high when idle
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;      // data sampled at first edge
	//SPI_InitStruct.SPI_NSS = SPI_NSS_Soft | SPI_NSSInternalSoft_Set; // set the NSS management to internal and pull internal NSS high
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32; // SPI frequency is APB2 frequency / 32 changed by Miroslav Kohut
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;// data is transmitted MSB first
	SPI_Init(SPI1, &SPI_InitStruct);

	SPI_Cmd(SPI1, ENABLE); // enable SPI1
}

void write_reg( uint8_t WriteAddr, uint8_t WriteData )
{
	chip_select();
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
	SPI_I2S_SendData(SPI1, WriteAddr);
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
	SPI_I2S_ReceiveData(SPI1);

	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
	SPI_I2S_SendData(SPI1, WriteData);
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
	SPI_I2S_ReceiveData(SPI1);
	chip_deselect();
}

uint8_t read_reg( uint8_t ReadAddr)
{
	chip_select();

	ReadAddr = READ_FLAG | ReadAddr;
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
	SPI_I2S_SendData(SPI1, ReadAddr);
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
	SPI_I2S_ReceiveData(SPI1); //Clear RXNE bit

	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
	SPI_I2S_SendData(SPI1, 0x00); //Dummy byte to generate clock
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
	uint8_t data =  SPI_I2S_ReceiveData(SPI1);
	chip_deselect();

	return data;
}

void read_regs( uint8_t ReadAddr, uint8_t *ReadBuf, unsigned int Bytes )
{
    uint16_t  i = 0;
    for(i=0; i<Bytes; i++){
    	ReadBuf[i] = read_reg(ReadAddr+i);
    }
}

void chip_select(void){
	GPIO_ResetBits(GPIOA, GPIO_Pin_8);
}
void chip_deselect(void){
	GPIO_SetBits(GPIOA, GPIO_Pin_8);
}
// function sleep for specific time in def, +- 2 us
void sleep(uint64_t time){
	for(uint64_t c = 0; c < time;c++);
}

void TIM2_init(int period_in_miliseconds)
{
	uint32_t SystemTimeClock = 16000000;		//mame 16MHz vstup!!!
	unsigned short inputPeriodValue = 10000;		//input clock = 10000Hz = 0,1ms
	unsigned short prescalerValue = (unsigned short) (SystemTimeClock / inputPeriodValue) - 1;

	/*Structure for timer settings*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseStructure.TIM_Period = period_in_miliseconds*10 - 1;		// 10 period * 0,0001s = 0,001s = 1ms vzorkovaci cas
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Prescaler = prescalerValue;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	/* TIM Interrupts enable */
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;
	/* Enable the TIM2 gloabal Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM3_init(int period_in_miliseconds)
{
	uint32_t SystemTimeClock = 16000000;		//mame 16MHz vstup!!!
	unsigned short inputPeriodValue = 10000;		//input clock = 10000Hz = 0,1ms
	unsigned short prescalerValue = (unsigned short) (SystemTimeClock / inputPeriodValue) - 1;

	/*Structure for timer settings*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_TimeBaseStructure.TIM_Period = period_in_miliseconds*10 - 1;		// 10 period * 0,0001s = 0,001s = 1ms vzorkovaci cas
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Prescaler = prescalerValue;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	/* TIM Interrupts enable */
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM3, ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;
	/* Enable the TIM3 gloabal Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}



void TIM2_IRQHandler()
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        calculate_angle();
    }
}

void TIM3_IRQHandler()
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
        read_rot();
    }
}


