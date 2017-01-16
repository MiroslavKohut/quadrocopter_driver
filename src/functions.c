/*
 * functions.c
 *
 *  Created on: 27. 11. 2016
 *      Author: Miroslav Kohút
 *      Basic functions for main program. All needed controllers are implemented int this library and
 *      also raw data from imu are read and filtered here.
 */
/* Includes */
#include <functions.h>

/* Functions */

/* Global main initialization */
void global_init(){

	/******* Common init ******/
	//needs to be initialized every function used delay library
	init_delay();

	/******* Variables init *******/
	desired_roll = 0;
	desired_pitch = 0;
	desired_yaw = 0;

    for(uint8_t i = 0;i<3;i++){
    	gyroscope_data_avg[i] = 0;
    }
    for(uint8_t i = 0;i<3;i++){
    	accelerometer_data_avg[i] = 0;
    }

	/****** Communication init *******/
	rx_init();
	usart_init();
	//spi is initialized inside of IMU unit

	/****** Devices_init *******/
	motor_init();

	if(mpu9250_init(1,BITS_DLPF_CFG_188HZ)){  //INIT the mpu9250
    	USART_send_function("\nCouldn't initialize MPU9250 via SPI!\r");
    }
    else
    	USART_send_function("\nMPU9250 WAS SUCCESFULLY INITIALIZED!\r");

	calib_acc();
    delay_ms(1000);

    return;
}
/* Global main timer initialization */
void timers_init(){
    //sampling timer
    TIM6_sampling_timer(moveing_average_sampling*1000);
    delay_ms(moveing_average_sampling*1000*moveing_average_samples);

    //integrating and controller timer
    TIM5_controller_timer(angle_sampling*1000);
}

void TIM5_controller_timer(int period_in_miliseconds)
{
	uint32_t SystemTimeClock = DEFAULT_FREQUENCY;
	unsigned short inputPeriodValue = INPUT_CLOCK;
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
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM6_sampling_timer(int period_in_miliseconds)
{
	uint32_t SystemTimeClock = DEFAULT_FREQUENCY;
	unsigned short inputPeriodValue = INPUT_CLOCK;
	unsigned short prescalerValue = (unsigned short) (SystemTimeClock / inputPeriodValue) - 1;

	/*Structure for timer settings*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	TIM_TimeBaseStructure.TIM_Period = period_in_miliseconds*10 - 1;		// 10 period * 0,0001s = 0,001s = 1ms vzorkovaci cas
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Prescaler = prescalerValue;
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
	/* TIM Interrupts enable */
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM6, ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;
	/* Enable the TIM6 gloabal Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM5_IRQHandler()
{
    if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM5, TIM_IT_Update);

        complementary_filter();
        /* For another controller testing you can add specific controller function*/
        PID_rate_control();
        //PID_pitch_control();
        //PID_stabilization_control();
    }
}

void TIM6_IRQHandler()
{
    if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
        /* reading data from imu with specific sample rate and implementing moveing average with specific number of samples */
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

/* COMPLEMENTARY FILTER */
void complementary_filter()
{
	float pitchAcc, rollAcc;

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

/* Yaw controller tested */
void PID_rate_control(){

	int8_t action_throttle_yaw;

	desired_yaw = (float)(pulse_length_yaw - YAW_CONSTANT)*4;
	action_throttle_yaw = (int)(((desired_yaw - gyroscope_data_avg[2])) * KP_yaw);
	int32_t controller_throttle = pulse_length_throttle - THROTTLE_CONSTANT;

	if(action_throttle_yaw > MAXIMUM_SATURATION_YAW)
		action_throttle_yaw = MAXIMUM_SATURATION_YAW;
	else if(action_throttle_yaw < -MAXIMUM_SATURATION_YAW)
		action_throttle_yaw = -MAXIMUM_SATURATION_YAW;


	if(controller_throttle > MAX_THROTTLE)
		controller_throttle = MAX_THROTTLE;

	if (controller_throttle + action_throttle_yaw > MAX_THROTTLE)
		controller_throttle = controller_throttle - (controller_throttle + action_throttle_yaw - MAX_THROTTLE);

	if (controller_throttle > MINIMUM_SATURATION){
		set_throttle(1,controller_throttle-action_throttle_yaw);
		set_throttle(2,controller_throttle+action_throttle_yaw);
		set_throttle(3,controller_throttle-action_throttle_yaw);
		set_throttle(4,controller_throttle+action_throttle_yaw);
	}
	else
	{
		set_throttle(4,0);
		set_throttle(1,0);
		set_throttle(2,0);
		set_throttle(3,0);
	}
}

/* Not tested and some  optimization needed */
void PID_roll_control(){

	int8_t action_throttle_roll;

	action_throttle_roll = (int)(((desired_roll - roll)) * KP_roll);

	int32_t controller_throttle = pulse_length_throttle - 100;

	if(action_throttle_roll > MAXIMUM_SATURATION_ROLL)
		action_throttle_roll = MAXIMUM_SATURATION_ROLL;
	else if(action_throttle_roll < -MAXIMUM_SATURATION_ROLL)
		action_throttle_roll = -MAXIMUM_SATURATION_ROLL;

	if (controller_throttle > MINIMUM_SATURATION){
		set_throttle(1,controller_throttle - action_throttle_roll);
		set_throttle(2,controller_throttle - action_throttle_roll);
		set_throttle(3,controller_throttle + action_throttle_roll);
		set_throttle(4,controller_throttle + action_throttle_roll);
	}
	else
	{
		set_throttle(4,0);
		set_throttle(1,0);
		set_throttle(2,0);
		set_throttle(3,0);
	}


}

/* Not tested and some  optimization needed */
void PID_pitch_control(){

	int8_t action_throttle_pitch;

	action_throttle_pitch = (int)(desired_pitch - pitch)*KP_pitch;
	int32_t controller_throttle = pulse_length_throttle - 100;

	if(action_throttle_pitch > MAXIMUM_SATURATION_PITCH)
		action_throttle_pitch = MAXIMUM_SATURATION_PITCH;
	else if(action_throttle_pitch < -MAXIMUM_SATURATION_PITCH)
		action_throttle_pitch = -MAXIMUM_SATURATION_PITCH;

	if (controller_throttle > MINIMUM_SATURATION){
		set_throttle(4,controller_throttle + action_throttle_pitch);
		set_throttle(1,controller_throttle + action_throttle_pitch);
		set_throttle(2,controller_throttle - action_throttle_pitch);
		set_throttle(3,controller_throttle - action_throttle_pitch);
	}
	else
	{
		set_throttle(4,0);
		set_throttle(1,0);
		set_throttle(2,0);
		set_throttle(3,0);
	}
}

/* Not tested and some  optimization needed */
void PID_stabilization_control(){

	int8_t action_throttle_yaw;
	int8_t action_throttle_roll;
	int8_t action_throttle_pitch;

	int32_t controller_throttle = pulse_length_throttle - THROTTLE_CONSTANT;
	desired_yaw = (float)(pulse_length_yaw - YAW_CONSTANT)*4;

	action_throttle_yaw = (int)(((desired_yaw - gyroscope_data_avg[2])) * KP_yaw);
	if(action_throttle_yaw > MAXIMUM_SATURATION_YAW)
		action_throttle_yaw = MAXIMUM_SATURATION_YAW;
	else if(action_throttle_yaw < -MAXIMUM_SATURATION_YAW)
		action_throttle_yaw = -MAXIMUM_SATURATION_YAW;

	action_throttle_pitch = (int8_t)(desired_pitch - pitch)*KP_pitch;
	if(action_throttle_pitch > MAXIMUM_SATURATION_PITCH)
		action_throttle_pitch = MAXIMUM_SATURATION_PITCH;
	else if(action_throttle_pitch < -MAXIMUM_SATURATION_PITCH)
		action_throttle_pitch = -MAXIMUM_SATURATION_PITCH;

	action_throttle_roll = (int8_t)(desired_roll - roll)*KP_roll;
	if(action_throttle_roll > MAXIMUM_SATURATION_ROLL)
		action_throttle_roll = MAXIMUM_SATURATION_ROLL;
	else if(action_throttle_roll < -MAXIMUM_SATURATION_ROLL)
		action_throttle_roll = -MAXIMUM_SATURATION_ROLL;

	if(controller_throttle > MAX_THROTTLE)
		controller_throttle = MAX_THROTTLE;

	int16_t action_throttle_PID = action_throttle_pitch + action_throttle_roll+action_throttle_yaw;

	if (controller_throttle + action_throttle_PID > MAX_THROTTLE)
		controller_throttle = controller_throttle - (controller_throttle + action_throttle_PID - MAX_THROTTLE);

	if (controller_throttle > MINIMUM_SATURATION){
		set_throttle(4,controller_throttle + action_throttle_pitch + action_throttle_roll+action_throttle_yaw);
		set_throttle(1,controller_throttle + action_throttle_pitch - action_throttle_roll-action_throttle_yaw);
		set_throttle(2,controller_throttle - action_throttle_pitch - action_throttle_roll+action_throttle_yaw);
		set_throttle(3,controller_throttle - action_throttle_pitch + action_throttle_roll-action_throttle_yaw);
	}
	else
	{
		set_throttle(4,0);
		set_throttle(1,0);
		set_throttle(2,0);
		set_throttle(3,0);
	}

}

