/*
 * functions.c
 *
 *  Created on: 27. 11. 2016
 *      Author: Asus
 */
/* Includes */
#include <functions.h>

// function sleep for specific time in def, +- 2 us

void functions_init(){
    for(uint8_t i = 0;i<3;i++){
    	gyroscope_data_avg[i] = 0;
    }
    for(uint8_t i = 0;i<3;i++){
    	accelerometer_data_avg[i] = 0;
    }
    return;
}

void TIM4_integrating_timer(int period_in_miliseconds)
{
	uint32_t SystemTimeClock = 16000000;		//mame 16MHz vstup!!!
	unsigned short inputPeriodValue = 10000;		//input clock = 10000Hz = 0,1ms
	unsigned short prescalerValue = (unsigned short) (SystemTimeClock / inputPeriodValue) - 1;

	/*Structure for timer settings*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	TIM_TimeBaseStructure.TIM_Period = period_in_miliseconds*10 - 1;		// 10 period * 0,0001s = 0,001s = 1ms vzorkovaci cas
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Prescaler = prescalerValue;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	/* TIM Interrupts enable */
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM4, ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;
	/* Enable the TIM4 gloabal Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM3_sampling_timer(int period_in_miliseconds)
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



void TIM4_IRQHandler()
{
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
        complementary_filter();
    }
}

void TIM3_IRQHandler()
{

    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
        read_rot();
        for(uint8_t i = 0;i<3;i++){
        	gyroscope_data_avg[i]= (gyroscope_data_avg[i]*(moveing_average_samples-1) + gyroscope_data[i])/moveing_average_samples;
        }
        read_acc();
        for(uint8_t i = 0;i<3;i++){
        	accelerometer_data_avg[i]= (accelerometer_data_avg[i]*(moveing_average_samples-1) + accelerometer_data[i])/moveing_average_samples;
        }
    }

}
/*
void calculate_angle()
{
	uint8_t i;
	for (i=0; i<3 ; i++)
	{
		if (!(gyroscope_data[i] < 2 && gyroscope_data[i] > -2)){
			gyroscope_angle[i] = gyroscope_angle[i] + gyroscope_data_avg[i] * angle_sampling;
		}
	}
}*/

/**COMPLEMENTARY FILTER **/

#define M_PI 3.14159265359

void complementary_filter()
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_10);
	float pitchAcc, rollAcc;

    // Integrate the gyroscope data -> int(angularSpeed) = angle

	if (!(gyroscope_data_avg[0] < 2 && gyroscope_data_avg[0] > -2)){
		roll = roll + gyroscope_data_avg[0] * angle_sampling;
	}
	if (!(gyroscope_data_avg[1] < 2 && gyroscope_data_avg[1] > -2)){
		pitch = pitch  - gyroscope_data_avg[1] * angle_sampling;
	}

	if (!(gyroscope_data_avg[2] < 2 && gyroscope_data_avg[2] > -2)){
		yaw = yaw  - gyroscope_data_avg[2] * angle_sampling;
	}

	//Turning around the X axis results in a vector on the Y-axis
    rollAcc = atan2f((float)accelerometer_data_avg[1], (float)accelerometer_data_avg[2]) * 180 / M_PI;
    roll = roll * 0.6 + rollAcc * 0.4;

	// Turning around the Y axis results in a vector on the X-axis
    pitchAcc = atan2f((float)accelerometer_data_avg[0], (float)accelerometer_data_avg[2]) * 180 / M_PI;
    pitch = pitch * 0.6 + pitchAcc * 0.4;
    GPIO_SetBits(GPIOA, GPIO_Pin_10);
}

