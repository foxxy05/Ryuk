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
#define M6_RPWM_Pin GPIO_PIN_5
#define M6_RPWM_GPIO_Port GPIOE
#define M6_LPWM_Pin GPIO_PIN_6
#define M6_LPWM_GPIO_Port GPIOE
#define M7_RPWM_Pin GPIO_PIN_6
#define M7_RPWM_GPIO_Port GPIOF
#define M7_LPWM_Pin GPIO_PIN_7
#define M7_LPWM_GPIO_Port GPIOF
#define M9_RPWM_Pin GPIO_PIN_8
#define M9_RPWM_GPIO_Port GPIOF
#define M9_LPWM_Pin GPIO_PIN_9
#define M9_LPWM_GPIO_Port GPIOF
#define M3_RPWM_Pin GPIO_PIN_0
#define M3_RPWM_GPIO_Port GPIOA
#define M3_LPWM_Pin GPIO_PIN_1
#define M3_LPWM_GPIO_Port GPIOA
#define M4_LPWM_Pin GPIO_PIN_2
#define M4_LPWM_GPIO_Port GPIOA
#define ENC3_Pin GPIO_PIN_6
#define ENC3_GPIO_Port GPIOA
#define ENC3A7_Pin GPIO_PIN_7
#define ENC3A7_GPIO_Port GPIOA
#define ENC1_Pin GPIO_PIN_9
#define ENC1_GPIO_Port GPIOE
#define ENC1E11_Pin GPIO_PIN_11
#define ENC1E11_GPIO_Port GPIOE
#define M8_RPWM_Pin GPIO_PIN_14
#define M8_RPWM_GPIO_Port GPIOB
#define M8_LPWM_Pin GPIO_PIN_15
#define M8_LPWM_GPIO_Port GPIOB
#define M2_LPWM_Pin GPIO_PIN_12
#define M2_LPWM_GPIO_Port GPIOD
#define M2_RPWM_Pin GPIO_PIN_13
#define M2_RPWM_GPIO_Port GPIOD
#define M1_LPWM_Pin GPIO_PIN_14
#define M1_LPWM_GPIO_Port GPIOD
#define M1_RPWM_Pin GPIO_PIN_15
#define M1_RPWM_GPIO_Port GPIOD
#define M4_RPWM_Pin GPIO_PIN_6
#define M4_RPWM_GPIO_Port GPIOC
#define M5_RPWM_Pin GPIO_PIN_7
#define M5_RPWM_GPIO_Port GPIOC
#define M5_LPWM_Pin GPIO_PIN_8
#define M5_LPWM_GPIO_Port GPIOC
#define ENC2_Pin GPIO_PIN_15
#define ENC2_GPIO_Port GPIOA
#define ENC2B3_Pin GPIO_PIN_3
#define ENC2B3_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
