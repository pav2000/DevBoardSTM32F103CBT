#include "main.h"
#include <stdio.h>

extern void scan_i2c(void);
extern void test_VL53L0x(void);
extern void test_max30102(void);
extern void test_hmc5883l(void);

void start_screen(void);
void screen_i2c(uint8_t menu);
void menu_i2c_screen(void);
void beep_enc(void);
void beep(uint16_t t);
void setDateTime(void);
uint32_t ADC_Result(ADC_HandleTypeDef *hadc, uint32_t ch);
char * my_ftoa(double f, char * buf, int precision);

extern TIM_HandleTypeDef htim1;
extern char *menu_main_text[];
extern uint8_t menu_main;
extern uint8_t menu_i2c;
extern mMenu myMenu;   // текущее мению
extern char buf[32];

const char *menu_i2c_text[NUM_MENU_MAIN]={"1.Scan I2C bus",
										  "2.Test vl53l01 (i2c)",
								          "3.Test max30102 (i2c)",
                                          "4.Test HMC5883L (i2c)",
							              "5.Exit main menu", };

// Стартовый экран
void start_screen(void){
 uint8_t i;
 ST7735_FillScreen(ST7735_BLACK);
 ST7735_FillRectangle(0, 0, 160-1, 12, ST7735_BLUE);
 ST7735_DrawString(0, 1, "Hardware 1.4 soft", Font_7x10, ST7735_YELLOW, ST7735_BLUE);
 ST7735_DrawString(122, 1, VERSION, Font_7x10, ST7735_YELLOW, ST7735_BLUE);
// itoa(menu_main+1,buf,16);   // Вывести текущий пункт меню
// ST7735_DrawString(150, 1, buf, Font_7x10, ST7735_RED, ST7735_BLUE);
 for(i=1;i<=NUM_MENU_MAIN;i++) ST7735_DrawString(0, 2+i*STR_H,menu_main_text[i-1], Font_7x10, ST7735_WHITE, ST7735_BLACK);  // нарисовать все пункты
 // выделить пункт меню
 ST7735_FillRectangle(0, 2+(menu_main+1)*STR_H, 160-1,STR_H, ST7735_WHITE);  // подсветить выбранный пункт
 ST7735_DrawString(0, 2+(menu_main+1)*STR_H,menu_main_text[menu_main], Font_7x10, ST7735_BLUE, ST7735_WHITE);
}

// Экран меню тестирования I2C
void screen_i2c(uint8_t menu){
 uint8_t i;
 ST7735_FillScreen(ST7735_BLACK);
 ST7735_FillRectangle(0, 0, 160-1, 12, ST7735_BLUE);
 ST7735_DrawString(0, 1, "I2C menu", Font_7x10, ST7735_YELLOW, ST7735_BLUE);
 itoa(menu+1,buf,16);   // Вывести текущий пункт меню
 ST7735_DrawString(150, 1, buf, Font_7x10, ST7735_RED, ST7735_BLUE);

 for(i=1;i<=NUM_MENU_I2C;i++) ST7735_DrawString(0, 2+i*STR_H,menu_i2c_text[i-1], Font_7x10, ST7735_WHITE, ST7735_BLACK);  // нарисовать все пункты
 // выделить пункт меню
 ST7735_FillRectangle(0, 2+(menu+1)*STR_H, 160-1,STR_H, ST7735_WHITE);  // подсветить выбранный пункт
 ST7735_DrawString(0, 2+(menu+1)*STR_H,menu_i2c_text[menu], Font_7x10, ST7735_BLUE, ST7735_WHITE);
}

// Меню тестирования i2c
void menu_i2c_screen(void){
 uint8_t old_menu = 0;           // Старая позиция меню
 myMenu=mI2C;                    // текущее меню - i2c
 TIM1->ARR = (NUM_MENU_I2C-1)*2+1; // Энкодер считает по новым пунктам i2c
 TIM1->CNT = menu_i2c = 0;       // нулевой пункт меню
 screen_i2c(menu_i2c);
  while (1)
 {
	  HAL_GPIO_TogglePin(LED1_CE_NRF_GPIO_Port, LED1_CE_NRF_Pin); // Инвертирование состояния выхода.
	  HAL_Delay(50);                       // Пауза 50 миллисекунд.
	  menu_i2c= __HAL_TIM_GET_COUNTER(&htim1)/2;  // Прочитать значение энкодера
    if(old_menu!=menu_i2c) // Надо перерисовать пункт меню
    {
	  itoa(menu_i2c+1,buf,16);
  	  ST7735_DrawString(150, 1, buf, Font_7x10, ST7735_RED, ST7735_BLUE); // текущий атом меню
   	// Старое меню восстанавливаем
  	  ST7735_FillRectangle(0, 2+(old_menu+1)*STR_H, 160-1,STR_H, ST7735_BLACK);
      ST7735_DrawString(0, 2+(old_menu+1)*STR_H,menu_i2c_text[old_menu], Font_7x10, ST7735_WHITE, ST7735_BLACK);
   	// Новое рисуем
  	  ST7735_FillRectangle(0, 2+(menu_i2c+1)*STR_H, 160-1,STR_H, ST7735_WHITE);
      ST7735_DrawString(0, 2+(menu_i2c+1)*STR_H,menu_i2c_text[menu_i2c], Font_7x10, ST7735_BLUE, ST7735_WHITE);
      old_menu=menu_i2c;
      beep_enc();
    }
    // Нажата кнопка
    if (HAL_GPIO_ReadPin(GPIOB, ENC_BTN_Pin) == 1) {
   	 beep_enc();
   	 switch (menu_i2c){
   		case 0: scan_i2c();screen_i2c(menu_i2c); break;
   		case 1: test_VL53L0x();screen_i2c(menu_i2c); break;
   		case 2: test_max30102();screen_i2c(menu_i2c); break;
   		case 3: test_hmc5883l();screen_i2c(menu_i2c); break;
     	case 4: myMenu=mMain;  TIM1->ARR = (NUM_MENU_MAIN-1)*2+1;return; break; // Выход
  		default: break;

    	}//switch (menu_i2c)
    } // if

 }
}

// Звук энкодера
void beep_enc(void)
{
	HAL_GPIO_TogglePin(GPIOB, BUZZER_Pin); // Звук переключения
	HAL_Delay(10);
	HAL_GPIO_TogglePin(GPIOB, BUZZER_Pin);
}

void beep(uint16_t t)
{
	HAL_GPIO_TogglePin(GPIOB, BUZZER_Pin); // Звук переключения
	HAL_Delay(t);
	HAL_GPIO_TogglePin(GPIOB, BUZZER_Pin);
}

// Чтение одного канала ацп
uint32_t ADC_Result(ADC_HandleTypeDef *hadc, uint32_t ch){
       ADC_ChannelConfTypeDef sConfig;
       uint32_t adcResult = 0;

       sConfig.Channel = ch;
       sConfig.Rank = ADC_REGULAR_RANK_1;
       sConfig.SamplingTime = ADC_SAMPLETIME_13CYCLES_5;
       HAL_ADC_ConfigChannel(hadc, &sConfig);
       HAL_ADC_Start(hadc);
       HAL_ADC_PollForConversion(hadc, 100);
       adcResult = HAL_ADC_GetValue(hadc);
       HAL_ADC_Stop((ADC_HandleTypeDef*)&hadc);
       return adcResult;
}


// флоат в сроку
#define MAX_PRECISION	(10)
static const double rounders[MAX_PRECISION + 1] =
{
	0.5,				// 0
	0.05,				// 1
	0.005,				// 2
	0.0005,				// 3
	0.00005,			// 4
	0.000005,			// 5
	0.0000005,			// 6
	0.00000005,			// 7
	0.000000005,		// 8
	0.0000000005,		// 9
	0.00000000005		// 10
};
char * my_ftoa(double f, char * buf, int precision)
{
	char * ptr = buf;
	char * p = ptr;
	char * p1;
	char c;
	long intPart;

	// check precision bounds
	if (precision > MAX_PRECISION)
		precision = MAX_PRECISION;

	// sign stuff
	if (f < 0)
	{
		f = -f;
		*ptr++ = '-';
	}

	if (precision < 0)  // negative precision == automatic precision guess
	{
		if (f < 1.0) precision = 6;
		else if (f < 10.0) precision = 5;
		else if (f < 100.0) precision = 4;
		else if (f < 1000.0) precision = 3;
		else if (f < 10000.0) precision = 2;
		else if (f < 100000.0) precision = 1;
		else precision = 0;
	}

	// round value according the precision
	if (precision)
		f += rounders[precision];

	// integer part...
	intPart = f;
	f -= intPart;

	if (!intPart)
		*ptr++ = '0';
	else
	{
		// save start pointer
		p = ptr;

		// convert (reverse order)
		while (intPart)
		{
			*p++ = '0' + intPart % 10;
			intPart /= 10;
		}

		// save end pos
		p1 = p;

		// reverse result
		while (p > ptr)
		{
			c = *--p;
			*p = *ptr;
			*ptr++ = c;
		}

		// restore end pos
		ptr = p1;
	}

	// decimal part
	if (precision)
	{
		// place decimal point
		*ptr++ = '.';

		// convert
		while (precision--)
		{
			f *= 10.0;
			c = f;
			*ptr++ = '0' + c;
			f -= c;
		}
	}

	// terminating zero
	*ptr = 0;

	return buf;
}
// Время ---------
extern RTC_HandleTypeDef hrtc;
extern char *_time_;
extern char *_date_;
void setDateTime(void){
   RTC_TimeTypeDef sTime;
   RTC_DateTypeDef sDate;
   char s_month[5];
   int year;
   const char month_names[] = "JanFebMarAprMayJunJulAugSepOctNovDec";
   sscanf(_date_, "%s %d %d", (uint8_t*)s_month, (int*)&sDate.Date,(int*)&year);
  // sscanf(__TIME__, "%2d %*c %2d %*c %2d", &sTime.Hours, &sTime.Minutes, &sTime.Seconds);
   sscanf(_time_, "%2d:%2d:%2d",(int*)&sTime.Hours,(int*) &sTime.Minutes,(int*) &sTime.Seconds);
   // Find where is s_month in month_names. Deduce month value.
   sDate.Month = (strstr(month_names, s_month) - month_names) / 3+1;
   sDate.Year = year-2000;
   sDate.WeekDay = RTC_WEEKDAY_MONDAY;


  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }


   if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
    {
      Error_Handler();
    }
   /*##-3- Writes a data in a RTC Backup data Register1 #######################*/

    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR2, ((sDate.Date << 8) | (sDate.Month)));
   	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR3, ((sDate.Year << 8) | (sDate.WeekDay)));

}

/*
RTC_TimeTypeDef RtcTime;
RTC_DateTypeDef RtcDate;

void BackupDateToBR(void)
{
	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR2, ((RtcDate.Date << 8) | (RtcDate.Month)));
	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR3, ((RtcDate.Year << 8) | (RtcDate.WeekDay)));
}

// Установка фиксированного времени
void SetRTC(void)
{
	RTC_TimeTypeDef sTime = {0};
	RTC_DateTypeDef DateToUpdate = {0};

	// Initialize RTC and set the Time and Date

	sTime.Hours = 23;
	sTime.Minutes = 59;
	sTime.Seconds = 56;

	if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
	{
	  Error_Handler();
	}

	DateToUpdate.WeekDay = RTC_WEEKDAY_SATURDAY;
	DateToUpdate.Month = RTC_MONTH_FEBRUARY;
	DateToUpdate.Date = 3;
	DateToUpdate.Year = 20;

	if (HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BIN) != HAL_OK)
	{
	  Error_Handler();
	}

	BackupDateToBR();
}
*/


