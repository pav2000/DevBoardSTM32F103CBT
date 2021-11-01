/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "testimg.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define VERSION  "1.29"   // Версия тестовой программы
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

RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi1;
DMA_HandleTypeDef hdma_spi1_tx;

/* USER CODE BEGIN PV */
RTC_TimeTypeDef RtcTime;
RTC_DateTypeDef RtcDate;

uint8_t r = 0;
char buf[64];
int EncVal=0;
const char *menu_main_text[NUM_MENU_MAIN]={"1.Test TFT st7735",
	                        	 "2.Test analog sensor",
								 "3.Test key & leds",
                                 "4.Test RTC stm32",
							     "5.Test ds18b20",
                                 "6.Test I2C bus >>",
                                 "7.Test motor&ACS70331",
                                 "8.Test SD card",
								 "9.Test nrf24l01"	};

uint8_t menu_main = 0;
uint8_t menu_i2c = 0;
mMenu myMenu;   // текущее мению
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_SPI1_Init(void);
static void MX_ADC1_Init(void);
static void MX_RTC_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint32_t CalculateDayNumber(uint8_t Date, uint8_t Month, uint8_t Year)
{
	// Format:
	// DD.MM.YY
	uint32_t _Year = Year + 20;
	Month = (Month + 9) % 12;
	_Year = _Year - (Month / 10);
	return ((365 * _Year) + (_Year / 4) - (_Year / 100) + (_Year / 400) + (((Month * 306) + 5) / 10) + (Date - 1));
}

void CalculateDateFromDayNumber(uint32_t DayNumber, uint8_t *Date, uint8_t *Month, uint8_t *Year)
{
	uint32_t _Date, _Month, _Year;
	_Year = ((10000 * DayNumber) + 14780) / 3652425;
	int32_t ddd = DayNumber - ((365 * _Year) + (_Year / 4) - (_Year / 100) + (_Year / 400));
	if (ddd < 0)
	{
		_Year -= 1;
		ddd = DayNumber - ((365 * _Year) + (_Year / 4) - (_Year / 100) + (_Year / 400));
	}
	int32_t mi = ((100 * ddd) + 52) / 3060;
	_Month = (mi + 2) % 12 + 1;
	_Year = _Year + (mi + 2)/12;
	_Date = ddd - ((mi * 306) + 5)/10 + 1;
	*Date = _Date;
	*Month = _Month;
	*Year = _Year - 20;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
uint8_t old_menu = menu_main;  // Старая позиция меню
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
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_ADC1_Init();
  MX_RTC_Init();
  MX_I2C1_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */
  beep(200);

  ST7735_Init();          // Не забываем в кубе настроить DMA
  ST7735_Backlight_On(); // Включить подсветку дисплея
  ST7735_SetRotation(3);
  ST7735_FillScreen(ST7735_WHITE);
//  test_hmc5883l();
//  test_nrf24();
  ST7735_DrawImage(0, 10, 160, 58, (const unsigned char*) stm32logo); // вывести заставку
  ST7735_DrawString(0, 108, "Hardware version: 1.3", Font_7x10, ST7735_RED, ST7735_WHITE);
  ST7735_DrawString(0, 118, "Test prog version:", Font_7x10, ST7735_RED, ST7735_WHITE);
  ST7735_DrawString(130, 118, VERSION, Font_7x10, ST7735_RED, ST7735_WHITE);
  HAL_Delay(3000);
  HAL_ADCEx_Calibration_Start(&hadc1);  // Калибровка ацп
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
 // HAL_GPIO_WritePin(GPIOB, LED2_Pin, GPIO_PIN_SET);    // Установить светодиод 2 в 1
  HAL_GPIO_WritePin(GPIOB, LED1_Pin, GPIO_PIN_RESET);    // Установить светодиод 1 в 0
 // HAL_GPIO_WritePin(GPIOA, TFT_LED_Pin, GPIO_PIN_SET); // Включить подсветку дисплея

  HAL_ADCEx_Calibration_Start(&hadc1);   // Калибровка первого ADC

  start_screen();
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
 // myMenu=mMain;    // текущее меню - главное
  char testDataToSend[]="Hello world!\n";
  while (1)
  {
	  HAL_GPIO_TogglePin(GPIOB, LED1_Pin); // Инвертирование состояния выхода.
	  HAL_Delay(50);                       // Пауза 50 миллисекунд.

	//  CDC_Transmit_FS(testDataToSend, strlen(testDataToSend));

     if(old_menu!=menu_main) // Надо перерисовать пункт меню
     {
 	  itoa(menu_main+1,buf,16);
 	  HAL_NVIC_DisableIRQ(EXTI9_5_IRQn) ;
   	  ST7735_DrawString(150, 1, buf, Font_7x10, ST7735_RED, ST7735_BLUE); // текущий атом меню
    	// Старое меню восстанавливаем
   	  ST7735_FillRectangle(0, 2+(old_menu+1)*STR_H, 160-1,STR_H, ST7735_BLACK);
      ST7735_DrawString(0, 2+(old_menu+1)*STR_H,menu_main_text[old_menu], Font_7x10, ST7735_WHITE, ST7735_BLACK);
    	// Новое рисуем
   	  ST7735_FillRectangle(0, 2+(menu_main+1)*STR_H, 160-1,STR_H, ST7735_WHITE);
      ST7735_DrawString(0, 2+(menu_main+1)*STR_H,menu_main_text[menu_main], Font_7x10, ST7735_BLUE, ST7735_WHITE);
      old_menu=menu_main;
      HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
      beep_enc();

     }
     // Нажата кнопка
     if (HAL_GPIO_ReadPin(GPIOB, ENC_BTN_Pin) == 1) {
    	 beep_enc();
    	 switch (menu_main){
    		case 0: test_TFT();start_screen(); break;
    		case 1: test_ADC();start_screen(); break;
    		case 2: test_keyADC_LEDS();start_screen(); break;
      		case 3: test_RTC();start_screen(); break;
      		case 4: test_ds18b20();start_screen(); break;
      		case 5: menu_i2c_screen();start_screen(); break;
      		case 6: test_Stepper();start_screen(); break;
    		case 7: test_SD();start_screen(); break;
    		case 8: test_nrf24();start_screen(); break;

      		default: break;

    	}
     }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_ADC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
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
  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
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
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef DateToUpdate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_NONE;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */
   RTC_DateTypeDef BackupDate;

     HAL_PWR_EnableBkUpAccess();
      /* Enable BKP CLK enable for backup registers */
      __HAL_RCC_BKP_CLK_ENABLE();
      /* RTC clock enable */
      __HAL_RCC_RTC_ENABLE();

   RtcDate.Date = (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR2) >> 8);
   RtcDate.Month = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR2);
   RtcDate.Year = (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR3) >> 8);
   RtcDate.WeekDay = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR3);

   HAL_RTC_GetTime(&hrtc, &RtcTime, RTC_FORMAT_BIN); // There is also an internal date update based on HW RTC time elapsed!!
   HAL_RTC_GetDate(&hrtc, &BackupDate, RTC_FORMAT_BIN); // Days elapsed since MCU power down

   uint32_t BackupDateDays = CalculateDayNumber(BackupDate.Date, BackupDate.Month, BackupDate.Year);
   uint32_t RtcDateDays = CalculateDayNumber(RtcDate.Date, RtcDate.Month, RtcDate.Year);

   RtcDateDays += (BackupDateDays - CalculateDayNumber(1, 1, 0));

   CalculateDateFromDayNumber(RtcDateDays, &RtcDate.Date, &RtcDate.Month, &RtcDate.Year);

   HAL_RTC_SetDate(&hrtc, &RtcDate, RTC_FORMAT_BIN);

   //
   //	You have to do not let Init code to reinitialize Time and Date in RTC.
   //	It's "a bug" in HAL widely described and complain on forums.
   //	That causes every MCU restart the time and date will be same as you configured in CubeMX.
   //	All you have to do is just return before init time/date in this user section.
   //

   return;  // Для того что бы дата кажды раз не выставлялась
  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0;
  sTime.Minutes = 0;
  sTime.Seconds = 0;

  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  DateToUpdate.WeekDay = RTC_WEEKDAY_MONDAY;
  DateToUpdate.Month = RTC_MONTH_JANUARY;
  DateToUpdate.Date = 1;
  DateToUpdate.Year = 21;

  if (HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, TFT_CS_Pin|SPI_CS2_nrf_Pin|SD_CS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED2_Pin|LED1_Pin|STEP1_Pin|STEP2_Pin
                          |STEP3_Pin|STEP4_Pin|BUZZER_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, TFT_LED_Pin|TFT_DC_Pin|TFT_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED3_Pin */
  GPIO_InitStruct.Pin = LED3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED3_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : TFT_CS_Pin SD_CS_Pin */
  GPIO_InitStruct.Pin = TFT_CS_Pin|SD_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : SPI_CS2_nrf_Pin */
  GPIO_InitStruct.Pin = SPI_CS2_nrf_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(SPI_CS2_nrf_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED2_Pin LED1_Pin STEP1_Pin STEP2_Pin
                           STEP3_Pin STEP4_Pin BUZZER_Pin */
  GPIO_InitStruct.Pin = LED2_Pin|LED1_Pin|STEP1_Pin|STEP2_Pin
                          |STEP3_Pin|STEP4_Pin|BUZZER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : ONEWIRE_Pin */
  GPIO_InitStruct.Pin = ONEWIRE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ONEWIRE_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : TFT_LED_Pin TFT_RST_Pin */
  GPIO_InitStruct.Pin = TFT_LED_Pin|TFT_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : TFT_DC_Pin */
  GPIO_InitStruct.Pin = TFT_DC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(TFT_DC_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : INT_MAX30102_Pin ENC_BTN_Pin ENC_A_Pin */
  GPIO_InitStruct.Pin = INT_MAX30102_Pin|ENC_BTN_Pin|ENC_A_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : ENC_B_Pin */
  GPIO_InitStruct.Pin = ENC_B_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(ENC_B_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}

/* USER CODE BEGIN 4 */
// Прерывание от энкодера
static uint32_t tick_encoder=0;
#define ENC_BOUNCE   10  // Дребезг энкодера
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

  if(GPIO_Pin == ENC_B_Pin) {  // Нужное прерываение от нужной ножки
      HAL_GPIO_TogglePin(GPIOB, LED2_Pin); // Инвертирование состояния выхода.
      if((HAL_GetTick()-tick_encoder>ENC_BOUNCE)&&(HAL_GPIO_ReadPin(GPIOB, ENC_B_Pin) == 0)) {// С прошедшего прерываения прошло более 20 мс - это не дребезг
      tick_encoder=HAL_GetTick();
        switch (myMenu){
   		case mMain:// {
   		    if ((HAL_GPIO_ReadPin(GPIOB, ENC_A_Pin) == 1)&&(HAL_GPIO_ReadPin(GPIOB, ENC_A_Pin) == 1)) { // Кто-то крутит ручку по часовой стрелке
   		    	HAL_Delay(1);   if (HAL_GPIO_ReadPin(GPIOB, ENC_A_Pin) == 1)
   		              if(menu_main <  NUM_MENU_MAIN-1) menu_main++; }
   		      else if ((HAL_GPIO_ReadPin(GPIOB, ENC_A_Pin) == 0)&&(HAL_GPIO_ReadPin(GPIOB, ENC_A_Pin) == 0)) { // Ручку крутят против часовой стрелки
   		    	HAL_Delay(1);   if (HAL_GPIO_ReadPin(GPIOB, ENC_A_Pin) == 0)
   		                  if(menu_main > 0) menu_main--; }

   			break;

   		case mI2C: // {
   		    if ((HAL_GPIO_ReadPin(GPIOB, ENC_A_Pin) == 1)&&(HAL_GPIO_ReadPin(GPIOB, ENC_A_Pin) == 1)) { // Кто-то крутит ручку по часовой стрелке
 		    	HAL_Delay(1);   if (HAL_GPIO_ReadPin(GPIOB, ENC_A_Pin) == 1)
     		    	if(menu_i2c <  NUM_MENU_I2C-1) menu_i2c++; }
   		      else if ((HAL_GPIO_ReadPin(GPIOB, ENC_A_Pin) == 0)&&(HAL_GPIO_ReadPin(GPIOB, ENC_A_Pin) == 0)) { // Ручку крутят против часовой стрелки
   		    	HAL_Delay(1);   if (HAL_GPIO_ReadPin(GPIOB, ENC_A_Pin) == 1)
       	    	  if(menu_i2c > 0) menu_i2c--; }
     		break;

      }

  } else{
   __NOP();
  } }
  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
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
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
