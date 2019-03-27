/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_Power_Pin GPIO_PIN_4
#define LED_Power_GPIO_Port GPIOA
#define LED_Error_Pin GPIO_PIN_5
#define LED_Error_GPIO_Port GPIOA
#define LED_Working_Pin GPIO_PIN_6
#define LED_Working_GPIO_Port GPIOA
#define Boot1_Pin GPIO_PIN_2
#define Boot1_GPIO_Port GPIOB
#define Button_Stop_Pin GPIO_PIN_10
#define Button_Stop_GPIO_Port GPIOC
#define Button_Restart_Pin GPIO_PIN_11
#define Button_Restart_GPIO_Port GPIOC
#define Relay_Power_Pin GPIO_PIN_0
#define Relay_Power_GPIO_Port GPIOD
#define Pump_Power_Pin GPIO_PIN_1
#define Pump_Power_GPIO_Port GPIOD
#define Solenoid_1_Pin GPIO_PIN_2
#define Solenoid_1_GPIO_Port GPIOD
#define Solenoid_2_Pin GPIO_PIN_3
#define Solenoid_2_GPIO_Port GPIOD
#define Solenoid_3_Pin GPIO_PIN_4
#define Solenoid_3_GPIO_Port GPIOD
#define Solenoid_4_Pin GPIO_PIN_5
#define Solenoid_4_GPIO_Port GPIOD
#define Solenoid_Air_Pin GPIO_PIN_6
#define Solenoid_Air_GPIO_Port GPIOD
/* USER CODE BEGIN Private defines */

#define STATUS_INIT 0
#define STATUS_IDLE 1
#define STATUS_MIXING 2
#define STATUS_READY 3
#define STATUS_ERROR 4

#define SPEED 0.0075

#define WEIGHT 5000

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
