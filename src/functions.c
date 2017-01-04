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

	rx_init();

	desired_roll = 0;
	desired_pitch = 0;
	desired_yaw = 0;

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

void TIM5_sampling_timer(int period_in_miliseconds)
{
	uint32_t SystemTimeClock = 16000000;		//mame 16MHz vstup!!!
	unsigned short inputPeriodValue = 10000;		//input clock = 10000Hz = 0,1ms
	unsigned short prescalerValue = (unsigned short) (SystemTimeClock / inputPeriodValue) - 1;

	/*Structure for timer settings*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	TIM_TimeBaseStructure.TIM_Period = period_in_miliseconds*10 - 1;		// 10 period * 0,0001s = 0,001s = 1ms vzorkovaci cas
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Prescaler = prescalerValue;
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
	/* TIM Interrupts enable */
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM5, ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;
	/* Enable the TIM5 gloabal Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}



void TIM4_IRQHandler()
{
	//GPIO_ResetBits(GPIOA, GPIO_Pin_10);
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
        complementary_filter();
        PID_yaw_control();
        //PID_pitch_control();
        //PID_stabilization_control();
    }
    //GPIO_SetBits(GPIOA, GPIO_Pin_10);
}

void TIM5_IRQHandler()
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_10);
    if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
        read_rot();
        for(uint8_t i = 0;i<3;i++){
        	gyroscope_data_avg[i]= (gyroscope_data_avg[i]*(moveing_average_samples-1) + gyroscope_data[i])/moveing_average_samples;
        }
        read_acc();
        for(uint8_t i = 0;i<3;i++){
        	accelerometer_data_avg[i]= (accelerometer_data_avg[i]*(moveing_average_samples-1) + accelerometer_data[i])/moveing_average_samples;
        }
    }
    GPIO_SetBits(GPIOA, GPIO_Pin_10);
}
/**COMPLEMENTARY FILTER **/

#define M_PI 3.14159265359

void complementary_filter()
{
	float pitchAcc, rollAcc;

    // Integrate the gyroscope data -> int(angularSpeed) = angle

	if (!(gyroscope_data_avg[0] < 2 && gyroscope_data_avg[0] > -2)){
		roll = roll + gyroscope_data_avg[0] * angle_sampling;
	}
	if (!(gyroscope_data_avg[1] < 2 && gyroscope_data_avg[1] > -2)){
		pitch = pitch  - gyroscope_data_avg[1] * angle_sampling;
	}
	if (!(gyroscope_data_avg[2] < 2 && gyroscope_data_avg[2] > -2)){
		yaw = yaw  + gyroscope_data_avg[2] * angle_sampling;
	}

	//Turning around the X axis results in a vector on the Y-axis
    rollAcc = atan2f((float)accelerometer_data_avg[1], (float)accelerometer_data_avg[2]) * M_PI_deg;
    roll = roll * 0.6 + rollAcc * 0.4;

	// Turning around the Y axis results in a vector on the X-axis
    pitchAcc = atan2f((float)accelerometer_data_avg[0], (float)accelerometer_data_avg[2]) * M_PI_deg;
    pitch = pitch * 0.6 + pitchAcc * 0.4;

}
void PID_yaw_control(){

	int8_t action_throttle_yaw;


	action_throttle_yaw = (int)(((desired_yaw - gyroscope_data_avg[2])) * KP);

	if(action_throttle_yaw > 10)
		action_throttle_yaw = 10;
	else if(action_throttle_yaw < -10)
		action_throttle_yaw = -10;

	int32_t contrroller_throttle = pulse_length_throttle - 100;

	if(contrroller_throttle > MAX_THROTTLE)
		contrroller_throttle = MAX_THROTTLE;

	if (contrroller_throttle + action_throttle_yaw > MAX_THROTTLE)
		contrroller_throttle = contrroller_throttle - (contrroller_throttle + action_throttle_yaw - MAX_THROTTLE);

	if (contrroller_throttle > 5){
		set_throttle(1,contrroller_throttle-action_throttle_yaw);
		set_throttle(2,contrroller_throttle+action_throttle_yaw);
		set_throttle(3,contrroller_throttle-action_throttle_yaw);
		set_throttle(4,contrroller_throttle+action_throttle_yaw);
	}
	else
	{
		set_throttle(4,0);
		set_throttle(1,0);
		set_throttle(2,0);
		set_throttle(3,0);
	}


}

/*
void PID_roll_control(){

	int8_t action_throttle_roll;

	action_throttle_roll = (int)(((desired_roll - roll)) * Kp);

	int32_t contrroller_throttle = pulse_length_throttle - 100;

	if(action_throttle_roll > 30)
		action_throttle_roll = 30;
	else if(action_throttle_roll < -30)
		action_throttle_roll = -30;

	int32_t contrroller_throttle = pulse_length_throttle - 100;

	if (contrroller_throttle > 10){
		set_throttle(1,contrroller_throttle - action_throttle_roll);
		set_throttle(2,contrroller_throttle - action_throttle_roll);
		set_throttle(3,contrroller_throttle + action_throttle_roll);
		set_throttle(4,contrroller_throttle + action_throttle_roll);
	}
	else
	{
		set_throttle(4,0);
		set_throttle(1,0);
		set_throttle(2,0);
		set_throttle(3,0);
	}


}*/
/*
void PID_pitch_control(){


	int8_t action_throttle_pitch;

	action_throttle_pitch = (int)(desired_pitch - pitch)*KP;
	int32_t contrroller_throttle = pulse_length_throttle - 100;

	if(action_throttle_pitch > 30)
		action_throttle_pitch = 30;
	else if(action_throttle_pitch < -30)
		action_throttle_pitch = -30;

	if (contrroller_throttle > 10){
		set_throttle(4,contrroller_throttle + action_throttle_pitch);
		set_throttle(1,contrroller_throttle + action_throttle_pitch);
		set_throttle(2,contrroller_throttle - action_throttle_pitch);
		set_throttle(3,contrroller_throttle - action_throttle_pitch);
	}
	else
	{
		set_throttle(4,0);
		set_throttle(1,0);
		set_throttle(2,0);
		set_throttle(3,0);
	}
}*/
/*
void PID_stabilization_control(){

	int8_t action_throttle_roll;
	int8_t action_throttle_pitch;

	int32_t contrroller_throttle = pulse_length_throttle - 100;

	action_throttle_pitch = (int8_t)(desired_pitch - pitch)*KP;
	if(action_throttle_pitch > 30)
		action_throttle_pitch = 30;
	else if(action_throttle_pitch < -30)
		action_throttle_pitch = -30;

	action_throttle_roll = (int8_t)(desired_roll - roll)*KP;
	if(action_throttle_roll > 30)
		action_throttle_roll = 30;
	else if(action_throttle_roll < -30)
		action_throttle_roll = -30;

	if (contrroller_throttle > 10){
		set_throttle(4,contrroller_throttle + action_throttle_pitch + action_throttle_roll);
		set_throttle(1,contrroller_throttle + action_throttle_pitch - action_throttle_roll);
		set_throttle(2,contrroller_throttle - action_throttle_pitch - action_throttle_roll);
		set_throttle(3,contrroller_throttle - action_throttle_pitch + action_throttle_roll);
	}
	else
	{
		set_throttle(4,0);
		set_throttle(1,0);
		set_throttle(2,0);
		set_throttle(3,0);
	}

}*/

