/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "i2c-lcd.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */

//Communication msgs for ESP8266
uint8_t initMsg[] = "init\n";
uint8_t errorMsg[] = "error\n";
uint8_t startMsg[] = "start\n";
uint8_t doneMsg[] = "done\n";
uint8_t ofMsg[] = "overflow\n";

//Test msgs
uint8_t welcomeMessage[] = "Thank you for choosing XMixer Max!\n";
uint8_t selectMessage[] = "Please select the drinks with their ratios.\n";
uint8_t updateMessage[] = "Updating drink information...\n";
uint8_t receiveMessage[] = "Receiving message...\n";
uint8_t vol1Msg[] = "The volume of drink 1 is: ";
uint8_t vol2Msg[] = "The volume of drink 2 is: ";
uint8_t vol3Msg[] = "The volume of drink 3 is: ";
uint8_t vol4Msg[] = "The volume of drink 4 is: ";
uint8_t mlMsg[] = " ml.\n";
uint8_t reply[] = "I received: ";

//Variables
uint8_t pData;
uint8_t buffer[100];
uint8_t numBuffer[10];
int numBufIdx = 0;
int i = 0;
int LF = 0;
int CurIdx;
int status = STATUS_INIT;
int timeout = 0; //2 usages
int overflow = 0;
int tim; //working time passed to TIM2 handler
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_TIM2_Init(void);
static void MX_ADC1_Init(void);
/* USER CODE BEGIN PFP */
void Drink_Transfer(int sol, int time);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	int Vol1;
	int Vol2;
	int Vol3;
	int Vol4;
	int VolDel;
	int Time1 = 0;
	int Time2 = 0;
	int Time3 = 0;
	int Time4 = 0;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_TIM2_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */

  // System starts. Initialize LCD and UAR, turn a green LED to show the status. Wait for Internet connection.
  // Send msg to ESP first and then waiting for reply to check the connection
  // Software is responsible for verifying bidirectional connection with the server
  lcd_init();
  lcd_send_cmd(0x01);
  lcd_send_cmd(0x02);
  lcd_send_string("plThank you for");
  lcd_send_cmd(0xc0);
  lcd_send_string("using XMixer Max");
  HAL_GPIO_WritePin(LED_Power_GPIO_Port, LED_Power_Pin, GPIO_PIN_SET);
  HAL_Delay(5000);
  lcd_send_cmd(0x01);
  lcd_send_cmd(0x02);
  lcd_send_string("plTrying to con");
  lcd_send_cmd(0xc0);
  lcd_send_string("nect to Internet");
  HAL_UART_Receive_IT(&huart2, &pData, 1);
  HAL_UART_Transmit(&huart2, initMsg, sizeof(initMsg), 100);
  do {
	  if ((LF == 1) && (timeout == 0)) {
		  //Keep trying as long as 30 seconds have not passed since last restart.
		  LF = 0;
		  lcd_send_cmd(0x01);
		  lcd_send_cmd(0x02);
		  HAL_UART_Receive_IT(&huart2, &pData, 1);
		  if (buffer == "success\n") {
			  //Connection established. Start working.
			  status = STATUS_IDLE;
			  lcd_send_string("plPlease select");
			  lcd_send_cmd(0xc0);
			  lcd_send_string("drinks!");
		  } else if (buffer == "fail\n") {
			  //Connection failed. Reconnecting.
			  status = STATUS_ERROR;
			  lcd_send_string("plNo WiFi found");
			  lcd_send_cmd(0xc0);
			  lcd_send_string("Reconnecting...");
		  }
	  } else if (timeout == 1) {
		  //Have Waited for 30 seconds. Update LCD if still no connection, and turn a red LED PD1 on to indicate error. Then wait for button input to start over.
		  if (HAL_GPIO_ReadPin(Button_Restart_GPIO_Port, Button_Restart_Pin) == 1) {
			  //Button pressed. Start over.
			  timeout = 0;
			  status = STATUS_INIT;
			  HAL_GPIO_WritePin(LED_Error_GPIO_Port, LED_Error_Pin, GPIO_PIN_RESET);
			  lcd_send_cmd(0x01);
			  lcd_send_cmd(0x02);
			  lcd_send_string("plReconnecting...");
		  } else {
			  //Timeout
			  HAL_GPIO_WritePin(LED_Error_GPIO_Port, LED_Error_Pin, GPIO_PIN_SET);
			  lcd_send_cmd(0x01);
			  lcd_send_cmd(0x02);
			  if (status == STATUS_INIT) {
				  lcd_send_string("plWiFi module");
				  lcd_send_cmd(0xc0);
				  lcd_send_string("malfunctioning");
			  } else if (status == STATUS_ERROR) {
				  lcd_send_string("plPlease check");
				  lcd_send_cmd(0xc0);
				  lcd_send_string("WiFi signal");
			  }
		  }
	  }
   } while (status != STATUS_IDLE);

  timeout = 0;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  //Check if any information is received.
	  if (LF == 1) {
		  LF = 0;
		  if ((buffer[0] == 49) || (buffer[0] == 10)) { // /n or 1
			  //Order received, parse information
			  CurIdx = 1;
			  numBufIdx = 0;
			  VolDel = 0;
			  while(buffer[CurIdx] != 100) { // d
				  if (buffer[CurIdx] == 44) { // ,
					  CurIdx += 1;
		  	  	  	  while (buffer[CurIdx] != 46) { // .
		  	  	  		  numBuffer[numBufIdx] = buffer[CurIdx];
		  	  	  		  numBufIdx += 1;
		  	  	  		  CurIdx += 1;
		  	  	  	  }
		  	  	  	  numBuffer[numBufIdx + 1] = 0;
		  	  	  	  if (VolDel == 0) {
		  	  	  		  Vol1 = atoi(numBuffer);
		  	  	  		  //HAL_UART_Transmit(&huart3, vol1Msg, sizeof(vol1Msg), 100);
		  	  	  	  } else if (VolDel == 1) {
		  	  	  		  Vol2 = atoi(numBuffer);
		  	  	  		  //HAL_UART_Transmit(&huart3, vol2Msg, sizeof(vol2Msg), 100);
		  	  	  	  } else if (VolDel == 2) {
		  	  	  		  Vol3 = atoi(numBuffer);
		  	  	  		  //HAL_UART_Transmit(&huart3, vol3Msg, sizeof(vol3Msg), 100);
		  	  	  	  } else if (VolDel == 3) {
		  	  	  		  Vol4 = atoi(numBuffer);
		  	  	  		  //HAL_UART_Transmit(&huart3, vol4Msg, sizeof(vol4Msg), 100);
		  	  	  	  }
		  	  	  	  VolDel += 1;
		  	  	  	  numBufIdx = 0;
		  	  	  	  //HAL_UART_Transmit(&huart3, numBuffer, sizeof(numBuffer), 100);
		  	  	  	  //HAL_UART_Transmit(&huart3, mlMsg, sizeof(mlMsg), 100);
			  	  }
			  CurIdx += 1;
	  	  	  }
			  lcd_send_cmd(0x01);
			  lcd_send_cmd(0x02);
			  lcd_send_string("plOrder received");
			  lcd_send_cmd(0xc0);
			  lcd_send_string("Mixing");
			  status = STATUS_MIXING;
			  HAL_UART_Transmit(&huart2, startMsg, sizeof(startMsg), 100);
		  }
		  //For now, assume no other information would be received, and the process would not be interrupted.
		  else {
			  //Indicate information is not recognizable
			  HAL_UART_Transmit(&huart2, errorMsg, sizeof(errorMsg), 100);
		  }
		  i = 0;
		  HAL_UART_Receive_IT(&huart2, &pData, 1);
	  }

	  if (status == STATUS_MIXING) {
		  //Start mixing.
		  //Need to calculate the time first, fill them in Time1-4
		  HAL_GPIO_WritePin(LED_Working_GPIO_Port, LED_Working_Pin, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(Pump_idk_GPIO_Port, Pump_idk_Pin, GPIO_PIN_SET); //Turn on the relay
		  HAL_GPIO_WritePin(Pump_Power_GPIO_Port, Pump_Power_Pin, GPIO_PIN_SET); //Turn on the pump, as PC10 in the pump test

		  Drink_Transfer(1, Time1);
		  if (!overflow) {
			  Drink_Transfer(2, Time2);
			  if (!overflow) {
				  Drink_Transfer(3, Time3);
				  if (!overflow) {
					  Drink_Transfer(4, Time4);
				  }
			  }
		  }
		  Drink_Transfer(5, 3000); //Air

		  HAL_GPIO_WritePin(Pump_Power_GPIO_Port, Pump_Power_Pin, GPIO_PIN_RESET); //Turn off the pump
		  HAL_GPIO_WritePin(Pump_idk_GPIO_Port, Pump_idk_Pin, GPIO_PIN_RESET); //Turn off the relay
		  status = STATUS_IDLE;

		  //Update LED, LCD and the web site
		  HAL_GPIO_WritePin(LED_Working_GPIO_Port, LED_Working_Pin, GPIO_PIN_RESET);
		  if (!overflow) {
			  lcd_send_cmd(0x01);
			  lcd_send_cmd(0x02);
			  lcd_send_string("plOrder");
			  lcd_send_cmd(0xc0);
			  lcd_send_string("completed!");
			  HAL_UART_Transmit(&huart2, doneMsg, sizeof(doneMsg), 100);
		  } else {
			  lcd_send_cmd(0x01);
			  lcd_send_cmd(0x02);
			  lcd_send_string("plAlmost full!");
			  lcd_send_cmd(0xc0);
			  lcd_send_string("Stop mixing!");
			  HAL_UART_Transmit(&huart2, ofMsg, sizeof(ofMsg), 100);
			  overflow = 0;
		  }
	  }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /**Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 41999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 199999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Pump_idk_GPIO_Port, Pump_idk_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED_Power_Pin|LED_Error_Pin|LED_Working_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, Pump_Power_Pin|Solenoid_1_Pin|Solenoid_2_Pin|Solenoid_3_Pin 
                          |Solenoid_4_Pin|Solenoid_Air_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : Pump_idk_Pin */
  GPIO_InitStruct.Pin = Pump_idk_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Pump_idk_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_Power_Pin LED_Error_Pin LED_Working_Pin */
  GPIO_InitStruct.Pin = LED_Power_Pin|LED_Error_Pin|LED_Working_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : Button_Reset_Pin Button_Restart_Pin */
  GPIO_InitStruct.Pin = Button_Reset_Pin|Button_Restart_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : Pump_Power_Pin Solenoid_1_Pin Solenoid_2_Pin Solenoid_3_Pin 
                           Solenoid_4_Pin Solenoid_Air_Pin */
  GPIO_InitStruct.Pin = Pump_Power_Pin|Solenoid_1_Pin|Solenoid_2_Pin|Solenoid_3_Pin 
                          |Solenoid_4_Pin|Solenoid_Air_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void Drink_Transfer(int sol, int time) {
	GPIO_TypeDef *port;
	uint16_t pin;
	switch (sol) {
	case 1:
		port = Solenoid_1_GPIO_Port;
		pin = Solenoid_1_Pin;
		break;
	case 2:
		port = Solenoid_2_GPIO_Port;
		pin = Solenoid_2_Pin;
		break;
	case 3:
		port = Solenoid_3_GPIO_Port;
		pin = Solenoid_3_Pin;
		break;
	case 4:
		port = Solenoid_4_GPIO_Port;
		pin = Solenoid_4_Pin;
		break;
	case 5:
		port = Solenoid_Air_GPIO_Port;
		pin = Solenoid_Air_Pin;
		break;
	default:
		port = Solenoid_Air_GPIO_Port;
		pin = Solenoid_Air_Pin;
		break;
	}

	tim = time;
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET); //Open solenoid
	status = STATUS_MIXING;
	while (!timeout) {
		if (overflow) {
			break;
		}
	}
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET); //Close solenoid
	status = STATUS_IDLE;
	HAL_Delay(1500); //Wait for a little bit: 1.5s
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (pData == '\r') {
		i--;
		LF = 1;
	} else {
		buffer[i] = pData;
		i++;
		HAL_UART_Receive_IT(&huart2, &pData, 1);
	}

}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
