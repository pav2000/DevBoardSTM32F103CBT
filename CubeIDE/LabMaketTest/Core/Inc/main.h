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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdlib.h>
#include "st7735.h"
#include "fonts.h"
#include "ds18b20.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
#define VERSION  "1.33"     // Версия тестовой программы
#define STR_H           10  // Высота строки
#define NUM_MENU_MAIN   9   // Число пунктов основного меню
#define NUM_MENU_I2C    5   // Число пунктов i2c меню
typedef enum {
  mMain,
  mI2C
  } mMenu;  // Текущее меню

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
#define LED3_Pin GPIO_PIN_13
#define LED3_GPIO_Port GPIOC
#define SPI_CS2_nrf_Pin GPIO_PIN_4
#define SPI_CS2_nrf_GPIO_Port GPIOA
#define LED2_Pin GPIO_PIN_1
#define LED2_GPIO_Port GPIOB
#define LED1_CE_NRF_Pin GPIO_PIN_10
#define LED1_CE_NRF_GPIO_Port GPIOB
#define ONEWIRE_Pin GPIO_PIN_11
#define ONEWIRE_GPIO_Port GPIOB
#define STEP1_Pin GPIO_PIN_12
#define STEP1_GPIO_Port GPIOB
#define TFT_CS_Pin GPIO_PIN_13
#define TFT_CS_GPIO_Port GPIOB
#define TFT_DC_Pin GPIO_PIN_14
#define TFT_DC_GPIO_Port GPIOB
#define STEP4_Pin GPIO_PIN_15
#define STEP4_GPIO_Port GPIOB
#define ENC_B_Pin GPIO_PIN_8
#define ENC_B_GPIO_Port GPIOA
#define ENC_A_Pin GPIO_PIN_9
#define ENC_A_GPIO_Port GPIOA
#define SD_CS_Pin GPIO_PIN_10
#define SD_CS_GPIO_Port GPIOA
#define TFT_RST_Pin GPIO_PIN_15
#define TFT_RST_GPIO_Port GPIOA
#define INT_MAX30102_Pin GPIO_PIN_3
#define INT_MAX30102_GPIO_Port GPIOB
#define BUZZER_Pin GPIO_PIN_4
#define BUZZER_GPIO_Port GPIOB
#define ENC_BTN_Pin GPIO_PIN_5
#define ENC_BTN_GPIO_Port GPIOB
#define STEP2_Pin GPIO_PIN_8
#define STEP2_GPIO_Port GPIOB
#define STEP3_Pin GPIO_PIN_9
#define STEP3_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
