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
volatile motors_t motors;
phase_t phase[3] = {INCREASING_ACCELERATION, INCREASING_ACCELERATION, INCREASING_ACCELERATION};
uint16_t counter_period;

typedef void (*excute_phase_t)(motors_t motors);
static void run_motor(motors_t motors);
typedef struct
{
	float phase_0 ;
	float phase_1 ;
	float phase_2 ;
	float phase_3 ;
	float phase_4 ;
	float phase_5 ;
	float phase_6 ;
} max_fre_t;

max_fre_t max_fre [3];
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

void stepper_motor_set_param(motors_t motors, uint16_t set_fre)
{
	param[motors].set_fre = set_fre;
	param[motors].one_phase_time = (float)sqrtf(set_fre/MAX_ACC_DE_DIFFERIENT);
	param[motors].max_acc_decel = param[motors].one_phase_time * MAX_ACC_DE_DIFFERIENT;
}

static void run_motor (motors_t motors)
{
	switch(motors)
			{
			case MOTOR_X:
				counter_period = (uint16_t)1000.0f/param[motors].current_fre;
				MOTOR_X_TIMER.Instance -> ARR = counter_period;
				__HAL_TIM_SET_COMPARE(&MOTOR_X_TIMER, MOTOR_X_CHANEL, counter_period/2);
				break;
			case MOTOR_Y:
				counter_period = (uint16_t)(1000.0f/param[motors].current_fre);
				MOTOR_Y_TIMER.Instance -> ARR = counter_period;
				__HAL_TIM_SET_COMPARE(&MOTOR_Y_TIMER, MOTOR_Y_CHANEL, counter_period/2);
				break;
			case MOTOR_Z:
				counter_period = (uint16_t)(1000.0f/param[motors].current_fre);
				MOTOR_Z_TIMER.Instance -> ARR = counter_period;
				__HAL_TIM_SET_COMPARE(&MOTOR_Z_TIMER, MOTOR_Z_CHANEL, counter_period/2);
				break;
			}

}
void excute_phase_0(motors_t motors)
{
	param[motors].acc_decel_defferient = MAX_ACC_DE_DIFFERIENT;

	if(param[motors].cnt_time < param[motors].one_phase_time)
	{
		//phase = INCREASING_ACCELERATION;
		param[motors].current_acc_decel = MAX_ACC_DE_DIFFERIENT * param[motors].cnt_time;
		param[motors].current_fre =(uint16_t) 0.5 * MAX_ACC_DE_DIFFERIENT * pow(param[motors].cnt_time,2);
		run_motor(motors);
	}
	else
	{
		phase[motors] = UNIFORM_ACCELERATION;
		param[motors].cnt_time = 0;
		max_fre[motors].phase_0 = 0.5 * MAX_ACC_DE_DIFFERIENT * pow(param[motors].one_phase_time,2);
	}
}

void excute_phase_1(motors_t motors)

{
	param[motors].acc_decel_defferient = 0;
	if(param[motors].cnt_time < param[motors].one_phase_time)
	{
		//phase = INCREASING_ACCELERATION;
		param[motors].current_acc_decel = param[motors].max_acc_decel;
		param[motors].current_fre =(uint16_t) (max_fre[motors].phase_0 + param[motors].max_acc_decel * param[motors].cnt_time);
		run_motor(motors);
	}
	else
	{
		phase[motors] = DECREASING_ACCELERATION;
		param[motors].cnt_time=0;
		max_fre[motors].phase_1 = max_fre[motors].phase_0 + param[motors].max_acc_decel * param[motors].one_phase_time;
	}
}

void excute_phase_2(motors_t motors)
{
	param[motors].acc_decel_defferient = -MAX_ACC_DE_DIFFERIENT;
	if(param[motors].cnt_time < param[motors].one_phase_time)
	{
		param[motors].current_acc_decel = param[motors].max_acc_decel - param[motors].max_acc_decel * param[motors].cnt_time;
		param[motors].current_fre =(uint16_t)max_fre[motors].phase_1 + param[motors].max_acc_decel * param[motors].cnt_time - 0.5 * MAX_ACC_DE_DIFFERIENT * pow(param[motors].cnt_time,2);
		run_motor(motors);
	}
	else
	{
		phase[motors] = UNIFORM_VELOCITY;
		param[motors].cnt_time=0;
		max_fre[motors].phase_2 = max_fre[motors].phase_1 + param[motors].max_acc_decel * param[motors].one_phase_time - 0.5 * MAX_ACC_DE_DIFFERIENT * pow(param[motors].one_phase_time,2) ;
	}
}

void excute_phase_3(motors_t motors)
{
	param[motors].acc_decel_defferient = 0;
	if(param[motors].cnt_time < 0xffff)
	{
		param[motors].current_acc_decel = 0;
		param[motors].current_fre =(uint16_t) max_fre[motors].phase_2;
		run_motor(motors);
	}
	else
	{
		phase[motors] = INCREASING_DECELERATION;
		param[motors].cnt_time = 0;
		max_fre[motors].phase_3 = max_fre[motors].phase_2;
	}
}

void excute_phase_4(motors_t motors)
{
	param[motors].acc_decel_defferient = -MAX_ACC_DE_DIFFERIENT;
	if(param[motors].cnt_time < param[motors].one_phase_time)
	{
		param[motors].current_acc_decel = -MAX_ACC_DE_DIFFERIENT * param[motors].cnt_time;
		param[motors].current_fre =(uint16_t)max_fre[motors].phase_3 - 0.5 * param[motors].acc_decel_defferient * pow(param[motors].cnt_time,2);
		run_motor(motors);
	}
	else
	{
		phase[motors] = UNIFORM_DECELERATION;
		param[motors].cnt_time = 0;
		max_fre[motors].phase_4 = max_fre[motors].phase_3 - 0.5 * param[motors].acc_decel_defferient * pow(param[motors].one_phase_time,2);
	}
}

void excute_phase_5(motors_t motors)
{
	param[motors].acc_decel_defferient = 0;
	if(param[motors].cnt_time < param[motors].one_phase_time)
	{
		param[motors].current_acc_decel = -param[motors].max_acc_decel;
		param[motors].current_fre =(uint16_t) max_fre[motors].phase_4 - param[motors].max_acc_decel * param[motors].cnt_time;
		run_motor(motors);
	}
	else
	{
		phase[motors] = DECREASING_DECELERATION;
		param[motors].cnt_time = 0;
		max_fre[motors].phase_5 = max_fre[motors].phase_4 - param[motors].max_acc_decel * param[motors].one_phase_time;
	}
}

void excute_phase_6(motors_t motors)
{
	param[motors].acc_decel_defferient = MAX_ACC_DE_DIFFERIENT;
	if(param[motors].cnt_time < param[motors].one_phase_time)
	{
		param[motors].current_acc_decel = -param[motors].max_acc_decel + param[motors].max_acc_decel * param[motors].cnt_time;
		param[motors].current_fre =(uint16_t) max_fre[motors].phase_5 - param[motors].max_acc_decel * param[motors].cnt_time + 0.5 * MAX_ACC_DE_DIFFERIENT * pow(param[motors].cnt_time,2);
		run_motor(motors);
	}
	else
	{
		phase[motors] = INCREASING_ACCELERATION;
		param[motors].cnt_time = 0;

	}
}

excute_phase_t excute_phase[7] = {excute_phase_0, excute_phase_1, excute_phase_2, excute_phase_3, excute_phase_4, excute_phase_5, excute_phase_6};
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == MOTOR_X_TIMER.Instance)
    {
    	motors = MOTOR_X;
    	param[MOTOR_X].cnt_time++;
    	excute_phase[phase[MOTOR_X]](MOTOR_X);
    }

    if (htim->Instance == MOTOR_Y_TIMER.Instance)
    {
    	motors = MOTOR_Y;
    	param[MOTOR_Y].cnt_time++;
    	excute_phase[phase[MOTOR_Y]](MOTOR_Y);
    }

    if (htim->Instance == MOTOR_Z_TIMER.Instance)
    {
    	motors = MOTOR_Z;
    	param[MOTOR_Z].cnt_time++;
    	excute_phase[phase[MOTOR_Z]](MOTOR_Z);
    }
}
