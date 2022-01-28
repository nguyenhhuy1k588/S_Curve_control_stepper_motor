/*
 * Stepper_Control.c
 *
 *  Created on: Jan 25, 2022
 *      Author: nguye
 */

#include "Stepper_Control.h"
#include "main.h"
#include <math.h>
#include "tim.h"

param_t  param[3];
motors_t motors;
phase_t phase[3] = {INCREASING_ACCELERATION, INCREASING_ACCELERATION, INCREASING_ACCELERATION };
void stepper_motor_init(motors_t motors)
{
	switch(motors)
	{
	case MOTOR_X:
		HAL_GPIO_WritePin(MOTOR_X_RST_GPIO_Port, MOTOR_X_RST_Pin, GPIO_PIN_SET);
		__HAL_TIM_SET_COMPARE(&MOTOR_X_TIMER, MOTOR_X_CHANEL, 0);
		HAL_TIM_PWM_Start(&MOTOR_X_TIMER, MOTOR_X_CHANEL);
		break;
	case MOTOR_Y:
		HAL_GPIO_WritePin(MOTOR_Y_RST_GPIO_Port, MOTOR_Y_RST_Pin, GPIO_PIN_SET);
		__HAL_TIM_SET_COMPARE(&MOTOR_X_TIMER, MOTOR_X_CHANEL, 0);
		HAL_TIM_PWM_Start(&MOTOR_X_TIMER, MOTOR_X_CHANEL);
		break;
	case MOTOR_Z:
		HAL_GPIO_WritePin(MOTOR_Y_RST_GPIO_Port, MOTOR_Y_RST_Pin, GPIO_PIN_SET);
		__HAL_TIM_SET_COMPARE(&MOTOR_X_TIMER, MOTOR_X_CHANEL, 0);
		HAL_TIM_PWM_Start(&MOTOR_X_TIMER, MOTOR_X_CHANEL);
		break;
	}
}
void stepper_motor_deinit(motors_t motors)
{
	switch(motors)
	{
	case MOTOR_X:
		HAL_GPIO_WritePin(MOTOR_X_RST_GPIO_Port, MOTOR_X_RST_Pin, GPIO_PIN_RESET);
		break;
	case MOTOR_Y:
		HAL_GPIO_WritePin(MOTOR_Y_RST_GPIO_Port, MOTOR_Y_RST_Pin, GPIO_PIN_RESET);
		break;
	case MOTOR_Z:
		HAL_GPIO_WritePin(MOTOR_Y_RST_GPIO_Port, MOTOR_Y_RST_Pin, GPIO_PIN_RESET);
	}
}

void stepper_motor_set_param(motors_t motors, uint16_t set_fre, uint16_t max_acc_decel)
{
	param[motors].set_fre = set_fre;
	param[motors].one_phase_time = (float)sqrtf(set_fre/MAX_ACC_DE_DIFFERIENT);
	param[motors].max_acc_decel = param[motors].one_phase_time * MAX_ACC_DE_DIFFERIENT;
}

void excute_phase_0(motors_t motor, uint16_t set_fre)
{
	param[motors].acc_decel_defferient = MAX_ACC_DE_DIFFERIENT;

	if(param[motors].cnt_time < param[motors].one_phase_time)
	{
		//phase = INCREASING_ACCELERATION;
		param[motors].current_acc_decel = param[motors].acc_decel_defferient * param[motors].cnt_time;
		param[motors].current_fre =(uint16_t) 0.5 * param[motors].acc_decel_defferient * pow(cnt_time,2);
		switch(motor)
		{
		case MOTOR_X:
			uint16_t counter_period = (uint16_t)(1000.0f/param[motors].current_fre);
			MOTOR_X_TIMER.Instance -> ARR = counter_period;
			__HAL_TIM_SET_COMPARE(&MOTOR_X_TIMER, MOTOR_X_CHANEL, counter_period/2);
			break;
		case MOTOR_Y:
			uint16_t counter_period = (uint16_t)(1000.0f/param[motors].current_fre);
			MOTOR_Y_TIMER.Instance -> ARR = counter_period;
			__HAL_TIM_SET_COMPARE(&MOTOR_Y_TIMER, MOTOR_Y_CHANEL, counter_period/2);
			break;
		case MOTOR_Z:
			uint16_t counter_period = (uint16_t)(1000.0f/param[motors].current_fre);
			MOTOR_Z_TIMER.Instance -> ARR = counter_period;
			__HAL_TIM_SET_COMPARE(&MOTOR_Z_TIMER, MOTOR_Z_CHANEL, counter_period/2);
			break;
		}
	}
	else
	{
		phase[motor] = UNIFORM_ACCELERATION;
		param[motors].cnt_time = 0;
	}
}

void excute_phase_1(float acc_decel_defferient, uint16_t one_phase_time, uint16_t cnt_time, uint16_t set_fre)
{
	param[motors].acc_decel_defferient = 0;
	if(cnt_time < one_phase_time)
	{
		phase = INCREASING_ACCELERATION;
		param[motors].current_acc_decel = param[motors].acc_decel_defferient * param[motors].cnt_time;
		param[motors].current_fre =(uint16_t) 0.5 * param[motors].acc_decel_defferient * pow(cnt_time,2);
		MOTOR_X_TIMER -> ARR = (uint32_t)(1000.0f/param[motors].current_fre);
		__HAL_TIM_SET_COMPARE(&MOTOR)
	}
	else
	{
		phase = DECREASING_ACCELERATION;
		cnt_time=0;
	}
}

void excute_phase_2(float acc_decel_defferient, uint16_t one_phase_time, uint16_t cnt_time, uint16_t set_fre)
{
	param[motors].acc_decel_defferient = MAX_ACC_DE_DIFFERIENT;
	if(cnt_time < one_phase_time)
	{
		phase = INCREASING_ACCELERATION;
		param[motors].current_acc_decel = param[motors].acc_decel_defferient * param[motors].cnt_time;
		param[motors].current_fre =(uint16_t) 0.5 * param[motors].acc_decel_defferient * pow(cnt_time,2);
		MOTOR_X_TIMER -> ARR = (uint32_t)(1000.0f/param[motors].current_fre);
		__HAL_TIM_SET_COMPARE(&MOTOR)
	}
	else
	{
		phase = UNIFORM_ACCELERATION;
	}
}

void excute_phase_3(float acc_decel_defferient, uint16_t one_phase_time, uint16_t cnt_time, uint16_t set_fre)
{
	param[motors].acc_decel_defferient = MAX_ACC_DE_DIFFERIENT;
	if(cnt_time < one_phase_time)
	{
		phase = INCREASING_ACCELERATION;
		param[motors].current_acc_decel = param[motors].acc_decel_defferient * param[motors].cnt_time;
		param[motors].current_fre =(uint16_t) 0.5 * param[motors].acc_decel_defferient * pow(cnt_time,2);
		MOTOR_X_TIMER -> ARR = (uint32_t)(1000.0f/param[motors].current_fre);
		__HAL_TIM_SET_COMPARE(&MOTOR)
	}
	else
	{
		phase = UNIFORM_ACCELERATION;
	}
}

void excute_phase_4(float acc_decel_defferient, uint16_t one_phase_time, uint16_t cnt_time, uint16_t set_fre)
{
	param[motors].acc_decel_defferient = MAX_ACC_DE_DIFFERIENT;
	if(cnt_time < one_phase_time)
	{
		phase = INCREASING_ACCELERATION;
		param[motors].current_acc_decel = param[motors].acc_decel_defferient * param[motors].cnt_time;
		param[motors].current_fre =(uint16_t) 0.5 * param[motors].acc_decel_defferient * pow(cnt_time,2);
		MOTOR_X_TIMER -> ARR = (uint32_t)(1000.0f/param[motors].current_fre);
		__HAL_TIM_SET_COMPARE(&MOTOR)
	}
	else
	{
		phase = UNIFORM_ACCELERATION;
	}
}

void excute_phase_5(float acc_decel_defferient, uint16_t one_phase_time, uint16_t cnt_time, uint16_t set_fre)
{
	param[motors].acc_decel_defferient = MAX_ACC_DE_DIFFERIENT;
	if(cnt_time < one_phase_time)
	{
		phase = INCREASING_ACCELERATION;
		param[motors].current_acc_decel = param[motors].acc_decel_defferient * param[motors].cnt_time;
		param[motors].current_fre =(uint16_t) 0.5 * param[motors].acc_decel_defferient * pow(cnt_time,2);
		MOTOR_X_TIMER -> ARR = (uint32_t)(1000.0f/param[motors].current_fre);
		__HAL_TIM_SET_COMPARE(&MOTOR)
	}
	else
	{
		phase = UNIFORM_ACCELERATION;
	}
}

void excute_phase_6(float acc_decel_defferient, uint16_t one_phase_time, uint16_t cnt_time, uint16_t set_fre)
{
	param[motors].acc_decel_defferient = MAX_ACC_DE_DIFFERIENT;
	if(cnt_time < one_phase_time)
	{
		phase = INCREASING_ACCELERATION;
		param[motors].current_acc_decel = param[motors].acc_decel_defferient * param[motors].cnt_time;
		param[motors].current_fre =(uint16_t) 0.5 * param[motors].acc_decel_defferient * pow(cnt_time,2);
		MOTOR_X_TIMER -> ARR = (uint32_t)(1000.0f/param[motors].current_fre);
		__HAL_TIM_SET_COMPARE(&MOTOR)
	}
	else
	{
		phase = UNIFORM_ACCELERATION;
	}
}












