/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "tim.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MOTOR_Y_RST_Pin GPIO_PIN_3
#define MOTOR_Y_RST_GPIO_Port GPIOE
#define MOTOR_Y_STEP_Pin GPIO_PIN_2
#define MOTOR_Y_STEP_GPIO_Port GPIOA
#define MOTOR_Z_STEP_Pin GPIO_PIN_6
#define MOTOR_Z_STEP_GPIO_Port GPIOA
#define MOTOR_X_DIR_Pin GPIO_PIN_9
#define MOTOR_X_DIR_GPIO_Port GPIOD
#define MOTOR_X_STEP_Pin GPIO_PIN_12
#define MOTOR_X_STEP_GPIO_Port GPIOD
#define MOTOR_X_RST_Pin GPIO_PIN_13
#define MOTOR_X_RST_GPIO_Port GPIOD
#define MOTOR_Z_DIR_Pin GPIO_PIN_0
#define MOTOR_Z_DIR_GPIO_Port GPIOD
#define MOTOR_Z_RST_Pin GPIO_PIN_4
#define MOTOR_Z_RST_GPIO_Port GPIOD
#define MOTOR_Y_DIR_Pin GPIO_PIN_1
#define MOTOR_Y_DIR_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */
#define MOTOR_X_TIMER         htim4
#define MOTOR_X_CHANEL        TIM_CHANNEL_1
#define MOTOR_Y_TIMER         htim5
#define MOTOR_Y_CHANEL        TIM_CHANNEL_3
#define MOTOR_Z_TIMER         htim3
#define MOTOR_Z_CHANEL        TIM_CHANNEL_1
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
