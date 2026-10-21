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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern ADC_HandleTypeDef hadc1;
extern I2C_HandleTypeDef hi2c2;
extern SPI_HandleTypeDef hspi2;
extern TIM_HandleTypeDef htim2;
extern UART_HandleTypeDef huart1;
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
#define Target_set_Pin GPIO_PIN_2
#define Target_set_GPIO_Port GPIOA
#define Temp_Sensor_Pin GPIO_PIN_3
#define Temp_Sensor_GPIO_Port GPIOA
#define Start_Button_Pin GPIO_PIN_12
#define Start_Button_GPIO_Port GPIOB
#define Start_Button_EXTI_IRQn EXTI15_10_IRQn
#define FND_SCK_Pin GPIO_PIN_13
#define FND_SCK_GPIO_Port GPIOB
#define FND_RCK_Pin GPIO_PIN_14
#define FND_RCK_GPIO_Port GPIOB
#define FND_MOSI_Pin GPIO_PIN_15
#define FND_MOSI_GPIO_Port GPIOB
#define Relay_Ctrl_Pin GPIO_PIN_5
#define Relay_Ctrl_GPIO_Port GPIOB
#define Led_1_Pin GPIO_PIN_6
#define Led_1_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
