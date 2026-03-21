/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define FW_LPWM_Pin GPIO_PIN_5
#define FW_LPWM_GPIO_Port GPIOE
#define FW_RPWM_Pin GPIO_PIN_6
#define FW_RPWM_GPIO_Port GPIOE
#define ENCO4_A_Pin GPIO_PIN_0
#define ENCO4_A_GPIO_Port GPIOA
#define ENCO4_B_Pin GPIO_PIN_1
#define ENCO4_B_GPIO_Port GPIOA
#define ENCO2_A_Pin GPIO_PIN_5
#define ENCO2_A_GPIO_Port GPIOA
#define LW_LPWM_Pin GPIO_PIN_6
#define LW_LPWM_GPIO_Port GPIOA
#define LW_RPWM_Pin GPIO_PIN_7
#define LW_RPWM_GPIO_Port GPIOA
#define RW_LPWM_Pin GPIO_PIN_0
#define RW_LPWM_GPIO_Port GPIOB
#define RW_RPWM_Pin GPIO_PIN_1
#define RW_RPWM_GPIO_Port GPIOB
#define ENCO1_A_Pin GPIO_PIN_9
#define ENCO1_A_GPIO_Port GPIOE
#define ENCO1_B_Pin GPIO_PIN_11
#define ENCO1_B_GPIO_Port GPIOE
#define ENCO3_A_Pin GPIO_PIN_12
#define ENCO3_A_GPIO_Port GPIOD
#define ENCO3_B_Pin GPIO_PIN_13
#define ENCO3_B_GPIO_Port GPIOD
#define ENCO2_B_Pin GPIO_PIN_3
#define ENCO2_B_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
