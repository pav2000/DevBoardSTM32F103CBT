#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "main.h"

//#include "testimg.h"
#include "VL53L0x.h"
#include "hmc5883l.h"
#include "max30102/max30102.h"
#include "fatfs.h"
#include "nrf24.h"
#include "w25qxx/w25qxx.h"
#include "user_at24cxx.h"
#include "MCP4725.h"


void test_TFT(void);
void test_ADC(void);
void test_keyADC_LEDS(void);
void test_RTC(void);
void test_ds18b20(void);
void scan_i2c(void);
void test_VL53L0x(void);
void test_max30102(void);
void test_hmc5883l(void);
void test_SD(void);
void test_nrf24(void);
void test_Stepper(void);
void test_SPI_flash(void);
void test_mcp4725(void);
void test_at24c128(void);
void test_fake_stm32(void);

extern void setDateTime(void);
extern char * my_ftoa(double f, char * buf, int precision);
extern void beep_enc(void);
extern void beep(uint16_t t);

extern I2C_HandleTypeDef hi2c1;
extern uint32_t ADC_Result(ADC_HandleTypeDef *hadc, uint32_t ch);

// Тест экрана
void test_TFT(void)
{

 ST7735_FillScreen(ST7735_BLACK);

 for(int x = 0; x < ST7735_GetWidth(); x++)
 {
   ST7735_DrawPixel(x, 0, ST7735_WHITE);
   ST7735_DrawPixel(x, ST7735_GetHeight() - 1, ST7735_WHITE);
 }

 for(int y = 0; y < ST7735_GetHeight(); y++)
 {
   ST7735_DrawPixel(0, y, ST7735_WHITE);
   ST7735_DrawPixel(ST7735_GetWidth() - 1, y, ST7735_WHITE);
 }

 ST7735_DrawLine(0, 0, ST7735_GetWidth(), ST7735_GetHeight(), ST7735_WHITE);
 ST7735_DrawLine(ST7735_GetWidth(), 0, 0, ST7735_GetHeight(), ST7735_WHITE);

 HAL_Delay(2000);

 ST7735_FillScreen(ST7735_BLACK);

 for (int i = 0; i < ST7735_GetHeight(); i += 4)
 {
  ST7735_DrawFastHLine(0, i, ST7735_GetWidth() - 1, ST7735_WHITE);
 }

 for (int i = 0; i < ST7735_GetWidth(); i += 4)
 {
  ST7735_DrawFastVLine(i, 0, ST7735_GetHeight() - 1, ST7735_WHITE);
 }

 HAL_Delay(2000);

 // Check fonts
 ST7735_FillScreen(ST7735_BLACK);
 ST7735_DrawString(0, 0, "Font_7x10, red on black, lorem ipsum dolor sit amet", Font_7x10, ST7735_RED, ST7735_BLACK);
 ST7735_DrawString(0, 3*10, "Font_7x10, green, lorem ipsum", Font_7x10, ST7735_GREEN, ST7735_BLACK);
 ST7735_DrawString(0, 3*10+3*18, "Font_16x26", Font_16x26, ST7735_BLUE, ST7735_BLACK);
 HAL_Delay(2000);

 // Check colors
 ST7735_FillScreen(ST7735_BLACK);
 ST7735_DrawString(0, 0, "BLACK", Font_7x10, ST7735_WHITE, ST7735_BLACK);
 HAL_Delay(500);

 ST7735_FillScreen(ST7735_BLUE);
 ST7735_DrawString(0, 0, "BLUE", Font_7x10, ST7735_BLACK, ST7735_BLUE);
 HAL_Delay(500);

 ST7735_FillScreen(ST7735_RED);
 ST7735_DrawString(0, 0, "RED", Font_7x10, ST7735_BLACK, ST7735_RED);
 HAL_Delay(500);

 ST7735_FillScreen(ST7735_GREEN);
 ST7735_DrawString(0, 0, "GREEN", Font_7x10, ST7735_BLACK, ST7735_GREEN);
 HAL_Delay(500);

 ST7735_FillScreen(ST7735_CYAN);
 ST7735_DrawString(0, 0, "CYAN", Font_7x10, ST7735_BLACK, ST7735_CYAN);
 HAL_Delay(500);

 ST7735_FillScreen(ST7735_MAGENTA);
 ST7735_DrawString(0, 0, "MAGENTA", Font_7x10, ST7735_BLACK, ST7735_MAGENTA);
 HAL_Delay(500);

 ST7735_FillScreen(ST7735_YELLOW);
 ST7735_DrawString(0, 0, "YELLOW", Font_7x10, ST7735_BLACK, ST7735_YELLOW);
 HAL_Delay(500);

 ST7735_FillScreen(ST7735_WHITE);
 ST7735_DrawString(0, 0, "WHITE", Font_7x10, ST7735_BLACK, ST7735_WHITE);
 HAL_Delay(500);

 // Draw circles
 ST7735_FillScreen(ST7735_BLACK);
 for (int i = 0; i < ST7735_GetHeight() / 2; i += 2)
 {
  ST7735_DrawCircle(ST7735_GetWidth() / 2, ST7735_GetHeight() / 2, i, ST7735_YELLOW);
 }
 HAL_Delay(1000);

 ST7735_FillScreen(ST7735_BLACK);
 ST7735_FillTriangle(0, 0, ST7735_GetWidth() / 2, ST7735_GetHeight(), ST7735_GetWidth(), 0, ST7735_RED);
 HAL_Delay(1000);

 // Проверка регулирования яркости дисплея с помощью аппаратного таймера IM 2 chanal 2
 ST7735_FillScreen(ST7735_WHITE);
 ST7735_DrawString(8, 50, "BACKLIGHT", Font_16x26, ST7735_BLACK, ST7735_WHITE);
 ST7735_DrawString(12, 80, "Uses TIM 2 chanal 2", Font_7x10, ST7735_BLACK, ST7735_WHITE);

 for (int i=0;i<100;i++){ST7735_Backlight(i);HAL_Delay(20); }
 for (int i=0;i<100;i++){ST7735_Backlight(100-i);HAL_Delay(50); }
 ST7735_Backlight(100);
// ST7735_FillScreen(ST7735_BLACK);
// ST7735_DrawImage(16, 0, 128, 128, (uint16_t*) test_img_128x128);
// ST7735_DrawImage(0, 0, 160, 128, (const unsigned char*) gImage_icon03);


// HAL_Delay(2000);
}

// Тест аналоговых датчиков
extern ADC_HandleTypeDef hadc1;
extern char buf[32];
void test_ADC(void)
{
uint16_t adc8=0,adc0=0;
uint32_t volt=0;
uint8_t i;

 ST7735_FillScreen(ST7735_BLACK);
 ST7735_FillRectangle(0, 0, 160-1, 12, ST7735_BLUE);
 ST7735_DrawString(0, 1, "ADC test", Font_7x10, ST7735_YELLOW, ST7735_BLUE);
 ST7735_DrawString(0, 2*STR_H, "Photo resistor R28", Font_7x10, ST7735_YELLOW, ST7735_BLACK);
 ST7735_DrawString(0, 3*STR_H, "ADC1 CH2 counts", Font_7x10, ST7735_WHITE, ST7735_BLACK);
 ST7735_DrawString(0, 4*STR_H, "ADC1 CH2 (mV)", Font_7x10, ST7735_WHITE, ST7735_BLACK);

 ST7735_DrawString(0, 6*STR_H, "Resistor R31", Font_7x10, ST7735_YELLOW, ST7735_BLACK);
 ST7735_DrawString(0, 7*STR_H, "ADC1 CH8 counts", Font_7x10, ST7735_WHITE, ST7735_BLACK);
 ST7735_DrawString(0, 8*STR_H, "ADC1 CH8 (mV)", Font_7x10, ST7735_WHITE, ST7735_BLACK);


 ST7735_DrawString(0, 118, "Exit - press encoder", Font_7x10, ST7735_YELLOW, ST7735_BLACK);
 while (1)
  {
	 if (HAL_GPIO_ReadPin(ENC_BTN_GPIO_Port, ENC_BTN_Pin) == 1) break;  // выход по кнопке энкодера
	 // Чтение ADC
	 adc0=0;
     for(i=0;i<10;i++)
	    adc0 = adc0 + ADC_Result(&hadc1, 2); // читаем полученное значение в переменную adc
     adc0=adc0/10;
	 itoa(adc0,buf,10);
	 ST7735_FillRectangle(110, 3*STR_H, 49, STR_H, ST7735_BLACK);
	 ST7735_DrawString(110, 3*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);
	 volt=(adc0*3300)/4096;
	 itoa(volt,buf,10);
	 ST7735_FillRectangle(110, 4*STR_H, 49, STR_H, ST7735_BLACK);
	 ST7735_DrawString(110, 4*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);

//	 adc8=0;
//	     for(i=0;i<10;i++)
	//	    adc8 = adc8 + ADC_Result(&hadc1, 8); // читаем полученное значение в переменную adc
//	     adc8=adc8/10;
	 adc8 = ADC_Result(&hadc1, 8); // читаем полученное значение в переменную adc
	 itoa(adc8,buf,10);
	 ST7735_FillRectangle(110, 7*STR_H, 49, STR_H, ST7735_BLACK);
	 ST7735_DrawString(110, 7*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);
	 volt=(adc8*3300)/4096;
	 itoa(volt,buf,10);
	 ST7735_FillRectangle(110, 8*STR_H, 49, STR_H, ST7735_BLACK);
	 ST7735_DrawString(110, 8*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);
	 HAL_Delay(50);

  }

}

// Тест аналоговых кнопок  и светодиодов
void test_keyADC_LEDS(void){
uint16_t adc2=0;
uint8_t i,n=20, led=0;
	 ST7735_FillScreen(ST7735_BLACK);
	 ST7735_FillRectangle(0, 0, 160-1, 12, ST7735_BLUE);
	 ST7735_DrawString(0, 1, "Test key ADC & Leds", Font_7x10, ST7735_YELLOW, ST7735_BLUE);
	 ST7735_DrawString(0, 2*STR_H, "Key ADC (ADC1 CH0)", Font_7x10, ST7735_YELLOW, ST7735_BLACK);
	 ST7735_DrawString(0, 3*STR_H, "ADC1 CH2 counts", Font_7x10, ST7735_WHITE, ST7735_BLACK);
	 ST7735_DrawString(0, 4*STR_H, "Button pressed:", Font_7x10, ST7735_WHITE, ST7735_BLACK);

	 ST7735_DrawString(0, 6*STR_H, "LEDS", Font_7x10, ST7735_YELLOW, ST7735_BLACK);
	 ST7735_DrawString(0, 7*STR_H, "LED1 (PB10)", Font_7x10, ST7735_WHITE, ST7735_BLACK);
	 ST7735_DrawString(0, 8*STR_H, "LED2 (PB1)", Font_7x10, ST7735_WHITE, ST7735_BLACK);
	 ST7735_DrawString(0, 9*STR_H, "LED3 (PC13)", Font_7x10, ST7735_WHITE, ST7735_BLACK);


	 ST7735_DrawString(0, 118, "Exit - press encoder", Font_7x10, ST7735_YELLOW, ST7735_BLACK);
	 while (1)
	  {
		 if (HAL_GPIO_ReadPin(ENC_BTN_GPIO_Port, ENC_BTN_Pin) == 1){
			 HAL_GPIO_WritePin(LED1_CE_NRF_GPIO_Port, LED1_CE_NRF_Pin,GPIO_PIN_SET);HAL_GPIO_WritePin(GPIOB,LED2_Pin,GPIO_PIN_SET);HAL_GPIO_WritePin(GPIOC,LED3_Pin,GPIO_PIN_SET);// Погасить все светодиоды
			 break;  // выход по кнопке энкодера
		 }
		 // Чтение ADC
		 adc2=0;
	     for(i=0;i<10;i++)
		    adc2 = adc2 + ADC_Result(&hadc1, 0); // читаем полученное значение в переменную adc
	     adc2=adc2/10;
		 itoa(adc2,buf,10);
		 ST7735_FillRectangle(110, 3*STR_H, 49, STR_H, ST7735_BLACK);
		 ST7735_DrawString(110, 3*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);
		 if (adc2<200) strcpy(buf,"none");
		 else if ((adc2>=1300)&&(adc2<1400)) strcpy(buf,"Esc");
		 else if ((adc2>2000)&&(adc2<2100)) strcpy(buf," +");
		 else if ((adc2>2400)&&(adc2<2500)) strcpy(buf,"Ok");
		 else if ((adc2>3000)&&(adc2<3200)) strcpy(buf," -");
		 else strcpy(buf,"error");
		 ST7735_FillRectangle(110, 4*STR_H, 49, STR_H, ST7735_BLACK);
		 ST7735_DrawString(110, 4*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);
		 HAL_Delay(100);
		 n++;
		 if(n>10){ // Пора светодиоды зажигать
		 n=0;
		 switch (led){
		 case 0: {HAL_GPIO_WritePin(LED1_CE_NRF_GPIO_Port, LED1_CE_NRF_Pin,GPIO_PIN_RESET);HAL_GPIO_WritePin(GPIOB,LED2_Pin,GPIO_PIN_SET);HAL_GPIO_WritePin(GPIOC,LED3_Pin,GPIO_PIN_SET);
		         ST7735_FillRectangle(80,7*STR_H-1,20,10,ST7735_BLUE);ST7735_FillRectangle(80,8*STR_H-1,20,10,ST7735_BLACK);ST7735_FillRectangle(80,9*STR_H-1,20,10,ST7735_BLACK);
		         led=1;break;}
		 case 1: {HAL_GPIO_WritePin(LED1_CE_NRF_GPIO_Port, LED1_CE_NRF_Pin,GPIO_PIN_SET);HAL_GPIO_WritePin(GPIOB,LED2_Pin,GPIO_PIN_RESET);HAL_GPIO_WritePin(GPIOC,LED3_Pin,GPIO_PIN_SET);
                 ST7735_FillRectangle(80,7*STR_H-1,20,10,ST7735_BLACK);ST7735_FillRectangle(80,8*STR_H-1,20,10,ST7735_RED);ST7735_FillRectangle(80,9*STR_H-1,20,10,ST7735_BLACK);
         		 led=2;break;}
		 case 2: {HAL_GPIO_WritePin(LED1_CE_NRF_GPIO_Port, LED1_CE_NRF_Pin,GPIO_PIN_SET);HAL_GPIO_WritePin(GPIOB,LED2_Pin,GPIO_PIN_SET);HAL_GPIO_WritePin(GPIOC,LED3_Pin,GPIO_PIN_RESET);
                 ST7735_FillRectangle(80,7*STR_H-1,20,10,ST7735_BLACK);ST7735_FillRectangle(80,8*STR_H-1,20,10,ST7735_BLACK);ST7735_FillRectangle(80,9*STR_H-1,20,10,ST7735_RED);
                 led=0;break;}

		 }
		 }
	//     HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
	  }

}

// Тест встроенных часов
// кажется что то не вопрядке с датой
char *_time_=__TIME__;
char *_date_=__DATE__;

extern RTC_HandleTypeDef hrtc;
void test_RTC(void){
RTC_TimeTypeDef time;
RTC_DateTypeDef date;
HAL_StatusTypeDef res;
char tmp[16];
uint16_t adc2=0;

	 ST7735_FillScreen(ST7735_BLACK);
	 ST7735_FillRectangle(0, 0, 160-1, 12, ST7735_BLUE);
	 ST7735_DrawString(0, 1, "Test RTC stm32", Font_7x10, ST7735_YELLOW, ST7735_BLUE);
	 ST7735_DrawString(0, STR_H*2, "__TIME__ ", Font_7x10, ST7735_WHITE, ST7735_BLACK);
	 ST7735_DrawString(60, STR_H*2, _time_, Font_7x10, ST7735_WHITE, ST7735_BLACK);
	 ST7735_DrawString(0, STR_H*3, "__DATE__", Font_7x10, ST7735_WHITE, ST7735_BLACK);
	 ST7735_DrawString(60, STR_H*3, _date_, Font_7x10, ST7735_WHITE, ST7735_BLACK);

	 ST7735_DrawString(0, STR_H*5, "RTC time", Font_7x10, ST7735_WHITE, ST7735_BLACK);
	 ST7735_DrawString(0, STR_H*6, "RTC date", Font_7x10, ST7735_WHITE, ST7735_BLACK);
	 ST7735_DrawString(0, STR_H*7, "WeekDay:", Font_7x10, ST7735_WHITE, ST7735_BLACK);

	 ST7735_DrawString(0, 109, "Set RTC - press Ok key", Font_7x10, ST7735_YELLOW, ST7735_BLACK);
	 ST7735_DrawString(0, 118, "Exit - press encoder", Font_7x10, ST7735_YELLOW, ST7735_BLACK);
	 while (1)
	  {
		  res = HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
		    if(res != HAL_OK) {
	    	ST7735_DrawString(60+7*0, STR_H*5, "Error GetTime", Font_7x10, ST7735_WHITE, ST7735_BLACK);
		    } else {
			    snprintf(tmp,sizeof(tmp)-1, "%02d:%02d:%02d", time.Hours,time.Minutes,time.Seconds);
			    ST7735_DrawString(60+7*0, STR_H*5, tmp, Font_7x10, ST7735_WHITE, ST7735_BLACK);
		    }

		    res = HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
		    if(res != HAL_OK) {
		    	ST7735_DrawString(60+7*0, STR_H*6, "Error GetDate", Font_7x10, ST7735_WHITE, ST7735_BLACK);
		    } else {
			    snprintf(tmp, sizeof(tmp), "%02d/%02d/%02d", date.Date,date.Month,date.Year);
			    ST7735_DrawString(60+7*0, STR_H*6, tmp, Font_7x10, ST7735_WHITE, ST7735_BLACK);

			    snprintf(tmp, sizeof(tmp), "%02d", date.WeekDay);
			    ST7735_DrawString(60+7*0, STR_H*7, tmp, Font_7x10, ST7735_WHITE, ST7735_BLACK);
		    }

			 // Чтение аналоговой кнопки
			 adc2=0;
		     for(uint8_t i=0;i<10;i++)
			    adc2 = adc2 + ADC_Result(&hadc1, 0);
		     adc2=adc2/10;
             if((adc2>2400)&&(adc2<2500)){ // Кнопка установки RTC нажата

             setDateTime();

             }

		 HAL_Delay(100);
		 if (HAL_GPIO_ReadPin(ENC_BTN_GPIO_Port, ENC_BTN_Pin) == 1) break;  // выход по кнопке энкодера

	  }
}
// тест датчика температуры
void test_ds18b20(){
	 uint8_t status,i;
	 uint8_t dt[8];
	 uint16_t raw_temper;
	 char c;
	 float temper;


	 ST7735_FillScreen(ST7735_BLACK);
	 ST7735_FillRectangle(0, 0, 160-1, 12, ST7735_BLUE);
	 ST7735_DrawString(0, 1, "Test DS18B20 gpio PB11", Font_7x10, ST7735_YELLOW, ST7735_BLUE);

     ST7735_DrawString(0, 118, "Exit - press encoder", Font_7x10, ST7735_YELLOW, ST7735_BLACK);

     // Инициализация датчика
     port_init();
 	 status = ds18b20_init(SKIP_ROM);
 	 sprintf(buf,"Init Status: %d",status);
     ST7735_DrawString(0, 1*STR_H, buf, Font_7x10, ST7735_YELLOW, ST7735_BLACK);

	 while (1)
	  {
			ds18b20_MeasureTemperCmd(SKIP_ROM, 0);
			for(i=0;i<8;i++){
			HAL_Delay(100);
			 if (HAL_GPIO_ReadPin(GPIOB, ENC_BTN_Pin) == 1)  return;  // выход по кнопке энкодера
			}
			ds18b20_ReadStratcpad(SKIP_ROM, dt, 0);
			ST7735_DrawString(0, 3*STR_H, "STRATHPAD:", Font_7x10, ST7735_YELLOW, ST7735_BLACK);
			sprintf(buf,"%02X %02X %02X %02X %02X %02X %02X %02X", dt[0], dt[1], dt[2], dt[3], dt[4], dt[5], dt[6], dt[7]);
			ST7735_DrawString(0, 4*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);
			raw_temper = ((uint16_t)dt[1]<<8)|dt[0];
			if(ds18b20_GetSign(raw_temper)) c='-';
			else c='+';
			temper = ds18b20_Convert(raw_temper);
			sprintf(buf,"Raw temp: 0x%04X", raw_temper);
			ST7735_DrawString(0, 6*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);
			sprintf(buf,"Temp (C): %c", c);
			ST7735_DrawString(0, 7*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);
			my_ftoa(temper,buf,2);
		    ST7735_DrawString(80, 7*STR_H, buf, Font_7x10, ST7735_RED, ST7735_BLACK);

		 HAL_Delay(150);
		 if (HAL_GPIO_ReadPin(ENC_BTN_GPIO_Port, ENC_BTN_Pin) == 1)  return;  // выход по кнопке энкодера

	  }
}

// Сканирование шины i2c
// Вверсии 1.5 при всех устнановленых компонентах должно находиться 5 устройств
void scan_i2c(){
	 uint8_t n=1,i,ret;

	 ST7735_FillScreen(ST7735_BLACK);
	 ST7735_FillRectangle(0, 0, 160-1, 12, ST7735_BLUE);
	 ST7735_DrawString(0, 1, "Scan I2C bus", Font_7x10, ST7735_YELLOW, ST7735_BLUE);

     ST7735_DrawString(0, 118, "Exit - press encoder", Font_7x10, ST7735_YELLOW, ST7735_BLACK);


     /*-[ I2C Bus Scanning ]-*/
       for(i=1; i<128; i++)
       {
    	   buf[0]=0;
    	   sprintf(buf, "0x%02x", i);
    	   ST7735_DrawString(130, 1, buf, Font_7x10, ST7735_WHITE, ST7735_BLUE); // текущий адрес
           ret = HAL_I2C_IsDeviceReady(&hi2c1, (uint16_t)(i<<1), 3, 5);
       	   if(ret == HAL_OK)
            {
       		  buf[0]=0;
              sprintf(buf, "Found: 0x%02x", i);
              ST7735_DrawString(0, 5+STR_H*n, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);
              beep_enc();
          	  n++;
            }
       	HAL_Delay(20);
        ST7735_DrawString(130, 1, "0x00", Font_7x10, ST7735_BLUE, ST7735_BLUE); // стереть адрес

       }
       ST7735_DrawString(0, 5+STR_H*n, "Scan done . . .", Font_7x10, ST7735_YELLOW, ST7735_BLACK);
       beep(500);

       /*--[ Scanning Done ]--*/
     while (1)
	  {
		 HAL_Delay(20);
		 if (HAL_GPIO_ReadPin(ENC_BTN_GPIO_Port, ENC_BTN_Pin) == 1)  return;  // выход по кнопке энкодера
	  }
}

// Тест датчика дальности VL53L0x
// Uncomment this line to use long range mode. This
// increases the sensitivity of the sensor and extends its
// potential range, but increases the likelihood of getting
// an inaccurate reading because of reflections from objects
// other than the intended target. It works best in dark
// conditions.
// Uncomment ONE of these two lines to get
// - higher speed at the cost of lower accuracy OR
// - higher accuracy at the cost of lower speed

//#define HIGH_SPEED
#define HIGH_ACCURACY
//#define LONG_RANGE

VL53L0X _VL53L0X;

void test_VL53L0x(void){
	 ST7735_FillScreen(ST7735_BLACK);
	 ST7735_FillRectangle(0, 0, 160-1, 12, ST7735_BLUE);
	 ST7735_DrawString(0, 1, "Test VL53L01 I2C1", Font_7x10, ST7735_YELLOW, ST7735_BLUE);

     ST7735_DrawString(0, 118, "Exit - press encoder", Font_7x10, ST7735_YELLOW, ST7735_BLACK);

	  setup_VL53L0X(&_VL53L0X); //Set up the VL53L0X struct with default address and timeout status.
	  if(!init(&_VL53L0X,true)) //attempt to initialise it with the necessary settings for normal operation. Returns 0 if fail, 1 if success.
	  {
		  sprintf(buf,"Failed to init");
		  ST7735_DrawString(0, 1*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);
	  }
	  else
	  {
		  sprintf(buf,"Successfully init");
		  ST7735_DrawString(0, 1*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);

	  }
   	    setTimeout(&_VL53L0X,500);
	    setVcselPulsePeriod(&_VL53L0X,VcselPeriodPreRange, 18);    //  VcselPeriodPreRange:  12 to 18 (initialized default: 14)
	    setVcselPulsePeriod(&_VL53L0X,VcselPeriodFinalRange, 14);  //  VcselPeriodFinalRange: 8 to 14 (initialized default: 10)
	    startContinuous(&_VL53L0X,0);

	  #if defined LONG_RANGE
	    // lower the return signal rate limit (default is 0.25 MCPS)
	     setSignalRateLimit(&_VL53L0X,0.1);
	   #endif

	  #if defined HIGH_SPEED
	     // reduce timing budget to 20 ms (default is about 33 ms)
	     setMeasurementTimingBudget(&_VL53L0X,20000);
	  #elif defined HIGH_ACCURACY
	            // increase timing budget to 200 ms
	     setMeasurementTimingBudget(&_VL53L0X,200000);
	  #endif

	     startContinuous(&_VL53L0X,0); //start the sensor in continuous back-to-back reading mode.

	     sprintf(buf,"I2C address: 0x%0x",_VL53L0X.address); // Адрес датчика
	     ST7735_DrawString(0, 2*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);

	     sprintf(buf,"ID model: 0x%0x",readReg16Bit(&_VL53L0X,IDENTIFICATION_MODEL_ID)); // модель
	     ST7735_DrawString(0, 3*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);

	     sprintf(buf,"ID revision: 0x%0x",readReg(&_VL53L0X, IDENTIFICATION_REVISION_ID)); // ревизия
	     ST7735_DrawString(0, 4*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);

	    // базовая шкала
	     ST7735_DrawString(0, 7*STR_H, "Distance: ", Font_7x10, ST7735_WHITE, ST7735_BLACK);
	     ST7735_DrawFastHLine(0,8*STR_H+6,320,ST7735_WHITE);
	     for(int i=0;i<=5;i++)   { ST7735_DrawFastVLine(i*32,8*STR_H+6,4,ST7735_WHITE);  }
	     ST7735_DrawFastVLine(159,8*STR_H+6,4,ST7735_WHITE);

	    while (1)
		  {
	    	uint16_t value = readRangeContinuousMillimeters(&_VL53L0X);
			if(value<2000){
				sprintf(buf, " %d mm     ", value);
				ST7735_DrawString(65, 7*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);
				uint x= (160*value/500);
				ST7735_FillRectangle(0, 8*STR_H+1, x, 4, ST7735_YELLOW);
				ST7735_FillRectangle(x+1, 8*STR_H+1, 159, 4, ST7735_BLACK);
			}
			else {
				sprintf(buf, "out of range");
				ST7735_DrawString(65, 7*STR_H, buf, Font_7x10, ST7735_RED, ST7735_BLACK);
				ST7735_FillRectangle(0, 8*STR_H+1, 160, 4, ST7735_YELLOW);
			}


			 if (timeoutOccurred(&_VL53L0X)) {
				sprintf(buf,"TIMEOUT, err 0x%0x", _VL53L0X.last_status); // последня ошибка i2c
				ST7735_DrawString(0, 6*STR_H, buf, Font_7x10, ST7735_RED, ST7735_BLACK);
			 }

			 if (HAL_GPIO_ReadPin(ENC_BTN_GPIO_Port, ENC_BTN_Pin) == 1)  return;  // выход по кнопке энкодера
			  HAL_Delay(100);
		  }

}
// тест датчика пульса  max30102
// Адрес 0x57
void test_max30102(void){

	  /* I2C1_EV_IRQn interrupt configuration */
	 ST7735_FillScreen(ST7735_BLACK);
	 ST7735_FillRectangle(0, 0, 160-1, 12, ST7735_BLUE);
	 ST7735_DrawString(0, 1, "Test max30102 I2C1", Font_7x10, ST7735_YELLOW, ST7735_BLUE);
	 ST7735_DrawString(0, 118, "Exit - press encoder", Font_7x10, ST7735_YELLOW, ST7735_BLACK);

	 sprintf(buf,"I2C address: 0x%0x",MAX30102_ADDR_WRITE>>1); // Адрес датчика
	 ST7735_DrawString(0, 1*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);

	 // Инициализация датчика
	 max30102_init();
	 sprintf(buf, "Max30102_Init: 0x%02x ", max30102_getStatus());
	 ST7735_DrawString(0, 2*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);

	 HAL_Delay(200);
     while (1)
	  {//INT_MAX30102
    	//  if (HAL_GPIO_ReadPin(GPIOB, INT_MAX30102_Pin) == GPIO_PIN_RESET)
    	        {
    	            HAL_GPIO_TogglePin(LED1_CE_NRF_GPIO_Port, LED1_CE_NRF_Pin); // Инвертирование состояния выхода.
    	            max30102_cal();
    	       //     uint8_t spo2 = max30102_getSpO2();
    	       //     uint8_t heartReat = max30102_getHeartRate();
    	            sprintf(buf, "HR: %d SpO2: %d  P: %d", max30102_getHeartRate(), max30102_getSpO2(),max30102_getPulse());
    	            ST7735_DrawString(0, 4*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);

    	        }

    //	 Max30102_Task();
    //	 sprintf(buf, "HR: %d SpO2: %d   ", Max30102_GetHeartRate(), Max30102_GetSpO2Value());
      //   ST7735_DrawString(0, 4*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);
 //        Max30102_InterruptCallback();
		// HAL_Delay(150);
    	 HAL_Delay(20);
		 if (HAL_GPIO_ReadPin(ENC_BTN_GPIO_Port, ENC_BTN_Pin) == 1){  return;}  // выход по кнопке энкодера
	  }

}

// тест датчика компаса HMC5883L
#define cx 125
#define cy 82
#define cr 30
#define cb 5
#define PI 3.1416
// Адрес 0x1E
void test_hmc5883l(void){
	float headingDegrees_old=360;
	// Для калибровки
	int32_t minX = 0;
	int32_t maxX = 0;
	int32_t minY = 0;
	int32_t maxY = 0;
	int16_t offX = 0;
	int16_t offY = 0;


	 ST7735_FillScreen(ST7735_BLACK);
	 ST7735_FillRectangle(0, 0, 160-1, 12, ST7735_BLUE);
	 ST7735_DrawString(0, 1, "Test HMC5883L I2C1", Font_7x10, ST7735_YELLOW, ST7735_BLUE);
//	 ST7735_DrawString(0, 108, "Esc - calibrate", Font_7x10, ST7735_YELLOW, ST7735_BLACK);
	 ST7735_DrawString(0, 118, "Exit - press encoder", Font_7x10, ST7735_YELLOW, ST7735_BLACK);

	 sprintf(buf,"I2C address: 0x%02x",HMC5883L_ADDRESS); // Адрес датчика
	 ST7735_DrawString(0, 1*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);

	 // Инициализация датчика
	 HMC5883L_setRange(HMC5883L_RANGE_0_88GA);
	 HMC5883L_setDataRate(HMC5883L_DATARATE_15HZ);
	 HMC5883L_setSamples(HMC5883L_SAMPLES_2);
	 HMC5883L_setMeasurementMode(HMC5883L_CONTINOUS);

	// sprintf(buf,"REG_ID: 0x%02x 0x%02x 0x%02x",HMC5883L_readRegister8(HMC5883L_REG_IDENT_A),HMC5883L_readRegister8(HMC5883L_REG_IDENT_B),HMC5883L_readRegister8(HMC5883L_REG_IDENT_C));
	// ST7735_DrawString(0, 6*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);

	 ST7735_DrawString(0, 4*STR_H, "Raw:", Font_7x10, ST7735_WHITE, ST7735_BLACK);
	 ST7735_DrawString(0, 5*STR_H, "Heading:", Font_7x10, ST7735_WHITE, ST7735_BLACK);
	 ST7735_DrawString(0, 6*STR_H, "Degress:", Font_7x10, ST7735_WHITE, ST7735_BLACK);

	 ST7735_DrawCircle(cx,cy,cr+2,ST7735_CYAN);
	 ST7735_DrawCircle(cx,cy,cr,ST7735_WHITE);

	 HMC5883L_setOffset(0, 0);
     while (1)
	  {
    	 Vector mag = HMC5883L_readRaw();

    	  // Determine Min / Max values
    	  if (mag.XAxis < minX) minX = mag.XAxis;
    	  if (mag.XAxis > maxX) maxX = mag.XAxis;
    	  if (mag.YAxis < minY) minY = mag.YAxis;
    	  if (mag.YAxis > maxY) maxY = mag.YAxis;

    	  // Calculate offsets
    	  offX = (maxX + minX)/2;
    	  offY = (maxY + minY)/2;

   // 	  HMC5883L_setOffset(offX, offY);
    	  sprintf(buf,"offX: %d  offY: %d",offX, offY);
    	  ST7735_DrawString(0, 3*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);

    	 Vector norm = HMC5883L_readNormalize();

    	  // Calculate heading
    	  float heading = atan2(norm.YAxis, norm.XAxis);

    	  // Set declination angle on your location and fix heading
    	  // You can find your declination on: http://magnetic-declination.com/
    	  // (+) Positive or (-) for negative
    	  // For Bytom / Poland declination angle is 4'26E (positive)
    	  // Moscow Magnetic Declination: +11° 40'
    	  // Formula: (deg + (min / 60.0)) / (180 / PI);
    	  float declinationAngle = (11.0 + (26.0 / 60.0)) / (180 / PI);
    	  heading += declinationAngle;

    	  // Correct for heading < 0deg and heading > 360deg
    	  if (heading < 0)
    	  {
    	    heading += 2 * PI;
    	  }

    	  if (heading > 2 * PI)
    	  {
    	    heading -= 2 * PI;
    	  }

    	  // Convert to degrees
    	 float headingDegrees = heading * 180/PI;

    	 sprintf(buf, "%d %d %d   ",mag.XAxis,mag.YAxis,mag.ZAxis);
         ST7735_DrawString(28, 4*STR_H, buf, Font_7x10, ST7735_YELLOW, ST7735_BLACK);

    	 itoa((int)heading,buf,10);
         ST7735_FillRectangle(60, 5*STR_H, 30, 10, ST7735_BLACK);
         ST7735_DrawString(60, 5*STR_H, buf, Font_7x10, ST7735_YELLOW, ST7735_BLACK);

         itoa((int)headingDegrees,buf,10);
         ST7735_FillRectangle(60, 6*STR_H, 30, 10, ST7735_BLACK);
         ST7735_DrawString(60, 6*STR_H, buf, Font_7x10, ST7735_YELLOW, ST7735_BLACK);

         if(headingDegrees!=headingDegrees_old){ // Рисовать стрелку на север
			 int h=headingDegrees_old;
			 ST7735_DrawLine((int)cx,(int)cy,(int)(cx+(cr-4)*cos(PI*h/180)),(int)(cy-(cr-4)*sin(PI*h/180)),ST7735_BLACK); // Стереть старую линию
			 h=headingDegrees;
			 ST7735_DrawLine((int)cx,(int)cy,(int)(cx+(cr-4)*cos(PI/180*h)),(int)(cy-(cr-4)*sin(PI/180*h)),ST7735_RED);
			 headingDegrees_old=headingDegrees;
			   }
         HAL_Delay(100);
		 if (HAL_GPIO_ReadPin(ENC_BTN_GPIO_Port, ENC_BTN_Pin) == 1){ max30102_OFF(); return;}  // выход по кнопке энкодера
	  }

}
// тест SD card
// File function return code (FRESULT )
//typedef enum {
//	FR_OK = 0,				/* (0) Succeeded */
//	FR_DISK_ERR,			/* (1) A hard error occurred in the low level disk I/O layer */
//	FR_INT_ERR,				/* (2) Assertion failed */
//	FR_NOT_READY,			/* (3) The physical drive cannot work */
//	FR_NO_FILE,				/* (4) Could not find the file */
//	FR_NO_PATH,				/* (5) Could not find the path */
//	FR_INVALID_NAME,		/* (6) The path name format is invalid */
//	FR_DENIED,				/* (7) Access denied due to prohibited access or directory full */
//	FR_EXIST,				/* (8) Access denied due to prohibited access */
//	FR_INVALID_OBJECT,		/* (9) The file/directory object is invalid */
//	FR_WRITE_PROTECTED,		/* (10) The physical drive is write protected */
//	FR_INVALID_DRIVE,		/* (11) The logical drive number is invalid */
//	FR_NOT_ENABLED,			/* (12) The volume has no work area */
//	FR_NO_FILESYSTEM,		/* (13) There is no valid FAT volume */
//	FR_MKFS_ABORTED,		/* (14) The f_mkfs() aborted due to any parameter error */
//	FR_TIMEOUT,				/* (15) Could not get a grant to access the volume within defined period */
//	FR_LOCKED,				/* (16) The operation is rejected according to the file sharing policy */
//	FR_NOT_ENOUGH_CORE,		/* (17) LFN working buffer could not be allocated */
//	FR_TOO_MANY_OPEN_FILES,	/* (18) Number of open files > _FS_SHARE */
//	FR_INVALID_PARAMETER	/* (19) Given parameter is invalid */
//} FRESULT;

void test_SD(void){

	 FATFS fs;  // file system
	 FIL fil; // File
	 FRESULT fresult;  // result
	 FATFS *pfs;
	 DWORD fre_clust;
	 uint32_t total, free_space;

	 ST7735_FillScreen(ST7735_BLACK);
	 ST7735_FillRectangle(0, 0, 160-1, 12, ST7735_BLUE);
	 ST7735_DrawString(0, 1, "Test SD card (SPI1)", Font_7x10, ST7735_YELLOW, ST7735_BLUE);

     ST7735_DrawString(0, 118, "Exit - press encoder", Font_7x10, ST7735_YELLOW, ST7735_BLACK);

//     HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_SET);
     fresult = f_mount(&fs, "/", 1);
     if(fresult==FR_OK) sprintf(buf, "Mount SD: OK"); else sprintf(buf, "Mount SD: ERROR %d",fresult);
     ST7735_DrawString(0, 2*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);

     // Check free space
     f_getfree("", &fre_clust, &pfs);
     total = (uint32_t)((pfs->n_fatent - 2) * pfs->csize * 0.5);
     free_space = (uint32_t)(fre_clust * pfs->csize * 0.5);
	 sprintf(buf, "Size SD: %lu",total);
     ST7735_DrawString(0, 3*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);
	 sprintf(buf, "Free SD: %lu",free_space);
     ST7735_DrawString(0, 4*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);

    	// Open file to write/ create a file if it doesn't exist
      fresult = f_open(&fil, "file1.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
      if(fresult==FR_OK) sprintf(buf, "Open file: OK"); else sprintf(buf, "Open file: ERROR %d",fresult);
      ST7735_DrawString(0, 5*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);

      // Writing text
      f_puts("This data is from the FILE1.txt. And it was written using ...f_puts... ", &fil);
      if(fresult==FR_OK) sprintf(buf, "Write file: OK"); else sprintf(buf, "Write file: ERROR %d",fresult);
      ST7735_DrawString(0, 6*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);

      // Close file
      fresult = f_close(&fil);
      if(fresult==FR_OK) sprintf(buf, "Close file: OK"); else sprintf(buf, "Close file: ERROR %d",fresult);
      ST7735_DrawString(0, 7*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);



     while (1)
	  {
		 HAL_Delay(20);
		 if (HAL_GPIO_ReadPin(ENC_BTN_GPIO_Port, ENC_BTN_Pin) == 1)  return;  // выход по кнопке энкодера
	  }

}
// Тест беспроводного чипа nrf24 (просто чтение различных регистров)
void test_nrf24(void){

	uint8_t buf1[20]={0};
	uint8_t dt_reg=0;

	 ST7735_FillScreen(ST7735_BLACK);
	 ST7735_FillRectangle(0, 0, 160-1, 12, ST7735_BLUE);
	 ST7735_DrawString(0, 1, "Test nrf24l01 (SPI1)", Font_7x10, ST7735_YELLOW, ST7735_BLUE);

     ST7735_DrawString(0, 118, "Exit - press encoder", Font_7x10, ST7735_YELLOW, ST7735_BLACK);
     HAL_GPIO_WritePin(LED1_CE_NRF_GPIO_Port, LED1_CE_NRF_Pin, GPIO_PIN_SET);    // Установить LED1_CE_NRF в 1

     NRF24_ini();

		HAL_Delay(100);
		dt_reg = NRF24_ReadReg(CONFIG);
		sprintf(buf,"CONFIG: 0x%02X",dt_reg);
		ST7735_DrawString(0, 1*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);

		dt_reg = NRF24_ReadReg(EN_AA);
		sprintf(buf,"EN_AA: 0x%02X",dt_reg);
		ST7735_DrawString(0, 2*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);


		dt_reg = NRF24_ReadReg(EN_RXADDR);
		sprintf(buf,"EN_RXADDR: 0x%02X",dt_reg);
		ST7735_DrawString(0, 3*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);


		dt_reg = NRF24_ReadReg(STATUS);
		sprintf(buf,"STATUS: 0x%02X",dt_reg);
		ST7735_DrawString(0, 4*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);

		dt_reg = NRF24_ReadReg(RF_SETUP);
		sprintf(buf,"RF_SETUP: 0x%02X",dt_reg);
		ST7735_DrawString(0, 5*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);

		NRF24_Read_Buf(TX_ADDR,buf1,3);
		sprintf(buf,"TX: 0x%02X, 0x%02X, 0x%02X",buf1[0],buf1[1],buf1[2]);
		ST7735_DrawString(0, 6*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);

		NRF24_Read_Buf(RX_ADDR_P1,buf1,3);
		sprintf(buf,"RX: 0x%02X, 0x%02X, 0x%02X",buf1[0],buf1[1],buf1[2]);
		ST7735_DrawString(0, 7*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);



     while (1)
	  {
		 HAL_Delay(20);
		 if (HAL_GPIO_ReadPin(ENC_BTN_GPIO_Port, ENC_BTN_Pin) == 1)  return;  // выход по кнопке энкодера
	  }

}

// Тест шагового двигателя с помощью перемнного резистора можно задавать скорость.
void test_Stepper(void){
     uint8_t i;
	 ST7735_FillScreen(ST7735_BLACK);
	 ST7735_FillRectangle(0, 0, 160-1, 12, ST7735_BLUE);
	 ST7735_DrawString(0, 1, "Test Step motor ", Font_7x10, ST7735_YELLOW, ST7735_BLUE);

     ST7735_DrawString(0, 118, "Exit - press encoder", Font_7x10, ST7735_YELLOW, ST7735_BLACK);


     while (1)
	  {
    	 int32_t adc0=0;
    	     for(i=0;i<5;i++)
    		    adc0 = adc0 + ADC_Result(&hadc1, 8); // читаем значение переменного резистора
    	     adc0=adc0/5;
    	 int32_t speed=adc0*100/4096;
    	 sprintf(buf,"Speed step ms: %d   ",(int)speed);
    	 ST7735_DrawString(0, 2*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);

    	 HAL_GPIO_WritePin(STEP1_GPIO_Port, STEP1_Pin, GPIO_PIN_SET);
    	 HAL_GPIO_WritePin(STEP2_GPIO_Port, STEP2_Pin, GPIO_PIN_RESET);
    	 HAL_GPIO_WritePin(STEP3_GPIO_Port, STEP3_Pin, GPIO_PIN_RESET);
    	 HAL_GPIO_WritePin(STEP4_GPIO_Port, STEP4_Pin, GPIO_PIN_RESET);
    	 HAL_Delay(speed);

    	 adc0=0;
    	    	 for(i=0;i<5;i++)
    	        	 adc0 = adc0 + ADC_Result(&hadc1, 3); // читаем значение датчика тока
    	        	     adc0=adc0/5;
    	        	 uint32_t volt=(adc0*3360)/4096;

       	 HAL_GPIO_WritePin(STEP1_GPIO_Port, STEP1_Pin, GPIO_PIN_RESET);
    	 HAL_GPIO_WritePin(STEP2_GPIO_Port, STEP2_Pin, GPIO_PIN_SET);
    	 HAL_GPIO_WritePin(STEP3_GPIO_Port, STEP3_Pin, GPIO_PIN_RESET);
    	 HAL_GPIO_WritePin(STEP4_GPIO_Port, STEP4_Pin, GPIO_PIN_RESET);
    	 HAL_Delay(speed);

    	 sprintf(buf,"Input mV: %d  ",(int)volt);
       	 ST7735_DrawString(0, 3*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);

    	 HAL_GPIO_WritePin(STEP1_GPIO_Port, STEP1_Pin, GPIO_PIN_RESET);
    	 HAL_GPIO_WritePin(STEP2_GPIO_Port, STEP2_Pin, GPIO_PIN_RESET);
    	 HAL_GPIO_WritePin(STEP3_GPIO_Port, STEP3_Pin, GPIO_PIN_SET);
    	 HAL_GPIO_WritePin(STEP4_GPIO_Port, STEP4_Pin, GPIO_PIN_RESET);
    	 HAL_Delay(speed);

    	 uint32_t cur=(3360/2-volt)*1000/400;
    	 sprintf(buf,"Current mA: %d  ",(int)cur);
    	 ST7735_DrawString(0, 4*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);

    	 HAL_GPIO_WritePin(STEP1_GPIO_Port, STEP1_Pin, GPIO_PIN_RESET);
    	 HAL_GPIO_WritePin(STEP2_GPIO_Port, STEP2_Pin, GPIO_PIN_RESET);
    	 HAL_GPIO_WritePin(STEP3_GPIO_Port, STEP3_Pin, GPIO_PIN_RESET);
    	 HAL_GPIO_WritePin(STEP4_GPIO_Port, STEP4_Pin, GPIO_PIN_SET);
    	 HAL_Delay(speed);

		 if (HAL_GPIO_ReadPin(ENC_BTN_GPIO_Port, ENC_BTN_Pin) == 1) {   // выход по кнопке энкодера
	     HAL_GPIO_WritePin(STEP1_GPIO_Port, STEP1_Pin, GPIO_PIN_RESET);
		 HAL_GPIO_WritePin(STEP2_GPIO_Port, STEP2_Pin, GPIO_PIN_RESET);
		 HAL_GPIO_WritePin(STEP3_GPIO_Port, STEP3_Pin, GPIO_PIN_RESET);
		 HAL_GPIO_WritePin(STEP4_GPIO_Port, STEP4_Pin, GPIO_PIN_RESET);
			 return;}
	  }

}
// Тестирование SPI flash памяти которая установлена на плате версии 1.5
// Чип w25q32 (4 мегабайт), тип (объем) памяти определяется, делается скоросной тест
const char *typeChip[10]={"W25Q10",
						  "W25Q20",
						  "W25Q40",
						  "W25Q80",
						  "W25Q16",
						  "W25Q32",
						  "W25Q64",
						  "W25Q128",
						  "W25Q256",
						  "W25Q512" };

void test_SPI_flash(void)
{
	ST7735_FillScreen(ST7735_BLACK);
	ST7735_FillRectangle(0, 0, 160-1, 12, ST7735_BLUE);
	ST7735_DrawString(0, 1, "Test SPI flash (SPI1)", Font_7x10, ST7735_YELLOW, ST7735_BLUE);

    ST7735_DrawString(0, 118, "Exit - press encoder", Font_7x10, ST7735_YELLOW, ST7735_BLACK);

    W25qxx_Init(); // Инициализация памяти и чтение информации о чипе памяти

    sprintf(buf,"Chip type: %s",typeChip[w25qxx.ID-1]);
    ST7735_DrawString(0, 1*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);


    sprintf(buf,"UniqID:%02X%02X%02X%02X%02X%02X%02X%02X",w25qxx.UniqID[0],w25qxx.UniqID[1],w25qxx.UniqID[2],w25qxx.UniqID[3],w25qxx.UniqID[4],w25qxx.UniqID[5],w25qxx.UniqID[6],w25qxx.UniqID[7]);
    ST7735_DrawString(0, 2*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);

    sprintf(buf,"Capacity kB: %d",(int)w25qxx.CapacityInKiloByte);
    ST7735_DrawString(0, 3*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);

    sprintf(buf,"Page size byte: %d",w25qxx.PageSize);
    ST7735_DrawString(0, 4*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);

    sprintf(buf,"Page count: %d",(int)w25qxx.PageCount);
    ST7735_DrawString(0, 5*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);

    sprintf(buf,"Sector size byte:%d",(int)w25qxx.SectorSize);
    ST7735_DrawString(0, 6*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);

    sprintf(buf,"Sector count: %d",(int)w25qxx.SectorCount);
    ST7735_DrawString(0, 7*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);

    sprintf(buf,"Block size byte:%d",(int)w25qxx.BlockSize);
    ST7735_DrawString(0, 8*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);

    sprintf(buf,"Block count: %d",(int)w25qxx.BlockCount);
    ST7735_DrawString(0, 9*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);

    sprintf(buf,"Status:0x%02x 0x%02x 0x%02x",w25qxx.StatusRegister1,w25qxx.StatusRegister2,w25qxx.StatusRegister3);
    ST7735_DrawString(0, 9*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);

    // Тест на скорость записи
    uint8_t bufSector[4096+1]={55};
    uint start=HAL_GetTick();
    W25qxx_WriteSector(bufSector, 0, 0, 4096);
    uint t=HAL_GetTick()-start;
    sprintf(buf,"Write 4 kB  msec: %d",t);
    ST7735_DrawString(0, 10*STR_H-1, buf, Font_7x10, ST7735_RED, ST7735_BLACK);

    // Тест на скорость чтения
    start=HAL_GetTick();
    W25qxx_ReadSector(bufSector, 0, 0, 4096);
    t=HAL_GetTick()-start;
    sprintf(buf,"Read 4 kB  msec: %d",t);
    ST7735_DrawString(0, 11*STR_H-2, buf, Font_7x10, ST7735_RED, ST7735_BLACK);


    while (1)
	  {
		 HAL_Delay(20);
		 if (HAL_GPIO_ReadPin(ENC_BTN_GPIO_Port, ENC_BTN_Pin) == 1)  return;  // выход по кнопке энкодера
	  }

}
// Тест ЦАП mcp4725, генерация пилообразного напряжения на выходе.
// Для визуального контроля (светодиод белого цвета будет плавно увеличивать яркость) не забыть поставить перемычку 1-2 в разъеме H4
void test_mcp4725(void){
	ST7735_FillScreen(ST7735_BLACK);
	ST7735_FillRectangle(0, 0, 160-1, 12, ST7735_BLUE);
	ST7735_DrawString(0, 1, "Test MCP4725 I2C1", Font_7x10, ST7735_YELLOW, ST7735_BLUE);

    ST7735_DrawString(0, 118, "Exit - press encoder", Font_7x10, ST7735_YELLOW, ST7735_BLACK);

    sprintf(buf,"I2C address: 0x%02x",MCP4725A0_ADDR_A00); // Адрес памяти
    ST7735_DrawString(0, 1*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);

	MCP4725 myMCP4725;
	myMCP4725 = MCP4725_init(&hi2c1, MCP4725A0_ADDR_A00, 3.30);
	// Check the connection:
	if(MCP4725_isConnected(&myMCP4725)){ /* Print that the DAC is connected */
		sprintf(buf,"MCP4725 is connected ");
		ST7735_DrawString(0, 2*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);
	}
	else{ /* Print that the DAC is NOT connected */
		sprintf(buf,"MCP4725 is NOT connected ");
		ST7735_DrawString(0, 2*STR_H, buf, Font_7x10, ST7735_RED, ST7735_BLACK);
	}

	MCP4725_setValue(&myMCP4725, 4095, MCP4725_FAST_MODE, MCP4725_POWER_DOWN_OFF); // погасить светодиод dac=3.3v
	uint16_t dac=0;
	sprintf(buf,"DAC value:");
	ST7735_DrawString(0, 3*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);

    while (1)
	  {
    	 MCP4725_setValue(&myMCP4725, dac, MCP4725_FAST_MODE, MCP4725_POWER_DOWN_OFF);
		 HAL_Delay(5);
		 ST7735_FillRectangle(80, 3*STR_H, 30, 10, ST7735_BLACK);
		 sprintf(buf,"%d",dac);
		 ST7735_DrawString(80, 3*STR_H, buf, Font_7x10, ST7735_RED, ST7735_BLACK);

		 if(dac>=4096) dac=0; else dac=dac+10;
		 if (HAL_GPIO_ReadPin(ENC_BTN_GPIO_Port, ENC_BTN_Pin) == 1) {
			 MCP4725_setValue(&myMCP4725, 4095, MCP4725_FAST_MODE, MCP4725_POWER_DOWN_OFF);
			 return;  // выход по кнопке энкодера
		 }
	  }
}

// Тест флаш памяти на интрефейсе I2C
// Тип чипа определять не получится, просто проверяетя запись строки и ее чтение.
// также проводится определение скорости чтения и записи
char *HAL_Return[4]={"OK","ERROR","BUSY","TIMEOUT"};  // Расшифровка кодов возврата hal i2c
void test_at24c128(void){

	ST7735_FillScreen(ST7735_BLACK);
	ST7735_FillRectangle(0, 0, 160-1, 12, ST7735_BLUE);
	ST7735_DrawString(0, 1, "Test AT24C128 I2C1", Font_7x10, ST7735_YELLOW, ST7735_BLUE);

    ST7735_DrawString(0, 118, "Exit - press encoder", Font_7x10, ST7735_YELLOW, ST7735_BLACK);

char testBuf[]="Test at24c128 flash.";
AT24_HandleTypeDef at24c128;

AT24CXX_Init(&at24c128);

sprintf(buf,"I2C address: 0x%02x",AT24_DEV_ADDR); // Адрес памяти
ST7735_DrawString(0, 1*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);

sprintf(buf,"Write chip: %s",HAL_Return[AT24CXX_Sequencial_Write(&at24c128, 0x00,(uint8_t*)testBuf, strlen(testBuf))]);
ST7735_DrawString(0, 3*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);

sprintf(buf,"> %s",testBuf);
ST7735_DrawString(0, 4*STR_H, buf, Font_7x10, ST7735_RED, ST7735_BLACK);

sprintf(buf,"Read chip: %s",HAL_Return[AT24CXX_Sequencial_Read(&at24c128, 0x00,(uint8_t*)testBuf, strlen(testBuf))]);
ST7735_DrawString(0, 5*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);

sprintf(buf,"< %s",testBuf);
ST7735_DrawString(0, 6*STR_H, buf, Font_7x10, ST7735_YELLOW, ST7735_BLACK);

// Тест на скорость записи
sprintf(buf,"Test write/read flash");
ST7735_DrawString(0, 8*STR_H-1, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);

uint8_t bufSector[256]={55};
uint start=HAL_GetTick();
AT24CXX_Sequencial_Write(&at24c128, 0x00, bufSector, 255);
uint t=HAL_GetTick()-start;
sprintf(buf,"Write 256 b msec: %d",t);
ST7735_DrawString(0, 9*STR_H-1, buf, Font_7x10, ST7735_RED, ST7735_BLACK);

// Тест на скорость чтения
start=HAL_GetTick();
AT24CXX_Sequencial_Read(&at24c128, 0x00, bufSector, (uint8_t)255);
t=HAL_GetTick()-start;
sprintf(buf,"Read 256 b msec: %d",t);
ST7735_DrawString(0, 10*STR_H-2, buf, Font_7x10, ST7735_RED, ST7735_BLACK);

    while (1)
	  {
		 HAL_Delay(20);
		 if (HAL_GPIO_ReadPin(ENC_BTN_GPIO_Port, ENC_BTN_Pin) == 1)  return;  // выход по кнопке энкодера
	  }
}

// Определение подлинности stm32
void test_fake_stm32(void){

	ST7735_FillScreen(ST7735_BLACK);
	ST7735_FillRectangle(0, 0, 160-1, 12, ST7735_BLUE);
	ST7735_DrawString(0, 1, "Test fake chip stm32", Font_7x10, ST7735_YELLOW, ST7735_BLUE);

    ST7735_DrawString(0, 118, "Exit - press encoder", Font_7x10, ST7735_YELLOW, ST7735_BLACK);


//sprintf(buf,"I2C address: 0x%02x",AT24_DEV_ADDR); // Адрес памяти
//ST7735_DrawString(0, 1*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);

// test
strcpy(buf,"Device ID:"); strcat(buf,uint16ToHex(HAL_GetDEVID()));
ST7735_DrawString(0, 1*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);
strcpy(buf,"Revision ID:"); strcat(buf,uint16ToHex(HAL_GetREVID()));
ST7735_DrawString(0, 2*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);
strcpy(buf,"JDEC:"); strcat(buf,get_Manufacturer());
ST7735_DrawString(0, 3*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);

strcpy(buf,"Manufacturer:"); strcat(buf,get_coreID());
ST7735_DrawString(0, 4*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);

strcpy(buf,"Core:"); strcat(buf,cpuid());
ST7735_DrawString(0, 5*STR_H, buf, Font_7x10, ST7735_WHITE, ST7735_BLACK);

ST7735_DrawString(0, 7*STR_H, "ROM table:", Font_7x10, ST7735_YELLOW, ST7735_BLACK);
ST7735_DrawString(0, 8*STR_H, get_romtable(), Font_7x10, ST7735_WHITE, ST7735_BLACK);


    while (1)
	  {
		 HAL_Delay(20);
		 if (HAL_GPIO_ReadPin(ENC_BTN_GPIO_Port, ENC_BTN_Pin) == 1)  return;  // выход по кнопке энкодера
	  }
}
