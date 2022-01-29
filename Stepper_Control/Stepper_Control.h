/*
 * Stepper_Control.h
 *
 *  Created on: Jan 25, 2022
 *      Author: nguye
 */

#ifndef STEPPER_CONTROL_H_
#define STEPPER_CONTROL_H_

#include <stdint.h>

#define DEFAULT_MAX_FRE 400// kHZ => 7500 rpm in mode 16x microstep
#define DEFAULT_MIN_FRE 1// kHZ
#define MAX_ACC_DE_DIFFERIENT 0.02 // kHZ/(10ms)^2

typedef enum
{
	MOTOR_X,
	MOTOR_Y,
	MOTOR_Z
} motors_t;
typedef enum
{
	STOP = 0,
	CLOCK_WISE = 1,
	COUNTER_CLOCKWISE = 2,
	IDLE = 3
} status_t;

typedef enum
{
	POSITION = 0,// Generate exact the number of pulse PWM to run stepper motor
	EVE_TICK = 1// Click button to start or stop motor
} mode_t;

typedef enum
{
	INCREASING_ACCELERATION = 0,
	UNIFORM_ACCELERATION = 1,
	DECREASING_ACCELERATION = 2,
	UNIFORM_VELOCITY = 3, // acceleration = 0;
	INCREASING_DECELERATION = 4,
	UNIFORM_DECELERATION = 5,
	DECREASING_DECELERATION = 6
} phase_t;
typedef struct
{
	uint16_t set_fre;
	float max_acc_decel;
	float one_phase_time;// /10ms
	float acc_decel_defferient;
	volatile uint16_t cnt_time;
	volatile uint32_t cnt_round;
	float current_fre;
	float current_acc_decel;
} param_t;

typedef struct
{
	mode_t mode;
	status_t status;
	//param_t param;
} control_t;

//typedef void (*excute_stage)(phase_t phase, float acc_decel_defferient, uint16_t set_fre,  );
void stepper_motor_init(motors_t motors);

void stepper_motor_deinit(motors_t motors);

void stepper_motor_set_param(motors_t motors, uint16_t set_fre);

void excute_phase_0(motors_t motors);
void excute_phase_1(motors_t motors);
void excute_phase_2(motors_t motors);
void excute_phase_3(motors_t motors);
void excute_phase_4(motors_t motors);
void excute_phase_5(motors_t motors);
void excute_phase_6(motors_t motors);

#endif /* STEPPER_CONTROL_H_ */
