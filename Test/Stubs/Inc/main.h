/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#include "stm32g4xx_hal.h"

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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define M1_PWM_Pin GPIO_PIN_0
#define M1_PWM_GPIO_Port GPIOA
#define M2_IN1_Pin GPIO_PIN_1
#define M2_IN1_GPIO_Port GPIOA
#define M2_IN2_Pin GPIO_PIN_2
#define M2_IN2_GPIO_Port GPIOA
#define M2_PWM_Pin GPIO_PIN_3
#define M2_PWM_GPIO_Port GPIOA
#define LED_STATUS_Pin GPIO_PIN_0
#define LED_STATUS_GPIO_Port GPIOB
#define M1_IN1_Pin GPIO_PIN_8
#define M1_IN1_GPIO_Port GPIOA
#define M1_IN2_Pin GPIO_PIN_15
#define M1_IN2_GPIO_Port GPIOA
#define M2_ENCB_Pin GPIO_PIN_4
#define M2_ENCB_GPIO_Port GPIOB
#define M2_ENCA_Pin GPIO_PIN_5
#define M2_ENCA_GPIO_Port GPIOB
#define M1_ENCB_Pin GPIO_PIN_6
#define M1_ENCB_GPIO_Port GPIOB
#define M1_ENCA_Pin GPIO_PIN_7
#define M1_ENCA_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
