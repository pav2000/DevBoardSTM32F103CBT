#include "main.h"
#include <stdio.h>

#define lowByte(a)        *((unsigned char *)&a)
#define highByte(a)       *(((unsigned char *)&a) + 1)

extern void scan_i2c(void);
extern void test_VL53L0x(void);
extern void test_max30102(void);
extern void test_hmc5883l(void);
extern void test_mcp4725(void);
extern void test_at24c128(void);


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
										  "2.Test vl53l01  (0x29)",
								          "3.Test max30102 (0x57)",
                                          "4.Test HMC5883L (0x1e)",
										  "5.Test MCP4725A (0x60)",
										  "6.Test AT24Cxxx (0x50)",
							              "7.Exit main menu" };

const char codeHex[]={"0123456789abcdef"};
char _bufTemp[64];

// Стартовый экран
void start_screen(void){
 uint8_t i;
 ST7735_FillScreen(ST7735_BLACK);
 ST7735_FillRectangle(0, 0, 160-1, 12, ST7735_BLUE);
 ST7735_DrawString(0, 1, "Hardware", Font_7x10, ST7735_YELLOW, ST7735_BLUE);
 ST7735_DrawString(60, 1, HARDWARE, Font_7x10, ST7735_YELLOW, ST7735_BLUE);
 ST7735_DrawString(94, 1, "soft", Font_7x10, ST7735_YELLOW, ST7735_BLUE);
 ST7735_DrawString(124, 1, VERSION, Font_7x10, ST7735_YELLOW, ST7735_BLUE);
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
   		case 4: test_mcp4725();screen_i2c(menu_i2c); break;
   		case 5: test_at24c128();screen_i2c(menu_i2c); break;
     	case 6: myMenu=mMain;  TIM1->ARR = (NUM_MENU_MAIN-1)*2+1;return; break; // Выход
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

// uint16_t в текстовую строку вида 0xf50e
char* uint16ToHex(uint16_t f)
{
	_bufTemp[0]='0';
	_bufTemp[1]='x';
	_bufTemp[2]=codeHex[0x0f & (unsigned char)(highByte(f)>>4)];
	_bufTemp[3]=codeHex[0x0f & (unsigned char)(highByte(f))];
	_bufTemp[4]=codeHex[0x0f & (unsigned char)(lowByte(f)>>4)];
	_bufTemp[5]=codeHex[0x0f & (unsigned char)(lowByte(f))];
	_bufTemp[6]=0; // Конец строки
	return _bufTemp;
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

// Работа с железом чипа (определение чипа) =========================================================
// Определение значения DBGMCU_IDCODE
/*Table: DBGMCU_IDCODES (https://mecrisp-stellaris-folkdoc.sourceforge.io/bluepill-diagnostics-v1.6.html)
*================= ============== ============== ============== ================= ============== ==================== =====
*Device            CPU-ID         RevID          Flash (KB)     2nd 64KB Hidden ? Ram (KB)       Manufacturer         *
*================= ============== ============== ============== ================= ============== ==================== =====
*STM32F103C8T6     0x410          0x2003         2x 64          YES               20             STMicroelectronics   Y
*STM32F103RBT6     0x410 ?        ?              128            N/A               20             STMicroelectronics   Y
*CKS32F103C8T6     0x410          0x2003         64             N/A               20             China Key System     N
*APM32F103CBT6     0x410          0x2003         ?              ?                 ?              Apex Micro           N
*GD32F130C8        0x410          0x1303         64             ?                 8              Gigadevice           ?
*GD32F150C8        0x410          0x1303         64             ?                 8              Gigadevice           ?
*STM32F103VET6     0x414          0x1001         512            N/A               64             STMicroelectronics   Y
*GD32F103VE        0x414          0x1309         512            N/A               64             Gigadevice           ?
*GD32F103VK        0x430          0x1309         3072           N/A               96             Gigadevice           ?
*GD32F103C8T6      0x436 ?        0x1038 ?       ?              ?                 ?              Gigadevice           ?
*================= ============== ============== ============== ================= ============== ==================== =====
*/
uint32_t get_DBGMCU_IDCODE(void){
	return ((DBGMCU->IDCODE));
}

// Определение значения производителя чипа через JDEC Codes
// https://www.blaatschaap.be/some-side-notes/
// https://mecrisp-stellaris-folkdoc.sourceforge.io/bluepill-diagnostics-v1.6.html
char* get_Manufacturer(void){
	int * pid = (int*)(0xE00FFFE0);
//	bool used = pid[2] & 8;
	uint8_t identity_code = ((pid[1] & 0xf0) >> 4) | ((pid[2] & 0x7) << 4);
	uint8_t continuation_code = (*(int*)(0xE00FFFD0));
	sprintf(_bufTemp, "0x%02x 0x%02x",identity_code,continuation_code);
	if ((identity_code==0x20)&&(continuation_code==0x00)) strcat(_bufTemp," (STM)"); else
    if ((identity_code==0xC8)&&(continuation_code==0x07)) strcat(_bufTemp," (GigaDevice)"); else
    if ((identity_code==0x51)&&(continuation_code==0x07)) strcat(_bufTemp," (GigaDevice (Beijing))"); else
    if ((identity_code==0x55)&&(continuation_code==0x05)) strcat(_bufTemp," (HK32)"); else
    if ((identity_code==0x3b)&&(continuation_code==0x04)) strcat(_bufTemp," (CS32 or APM32)"); else
   	strcat(_bufTemp,"Unknow");
    return _bufTemp;

}

// Чтение ROMTABLE
// https://github.com/a-v-s/ucdev/tree/master/demos/cortex_romtable/stm32f1
#include "arm_cpuid/arm_cpuid.h"
char* parse_romtable(void) {

	intptr_t ROMTABLE = (intptr_t) (0xE00FF000);
	romtable_id_t *rid = (romtable_id_t*) (ROMTABLE | 0xFD0);
	romtable_pid_t romtable_pid = extract_romtable_pid(rid);

	char *prob = "Unknown";

	if (romtable_pid.jep106_used) {
		if (romtable_pid.identity_code == 32
				&& romtable_pid.continuation_code == 0) {
			prob = "STM32";
		}
		if (romtable_pid.identity_code == 81
				&& romtable_pid.continuation_code == 7) {
			prob = "GD32";
		}
		if (romtable_pid.identity_code == 59
				&& romtable_pid.continuation_code == 4) {
			// APM or CS
			cortex_m_romtable_t *rom = (cortex_m_romtable_t*) (ROMTABLE);
			if (rom->etm & 1) {
				prob = "CS32";
			} else {
				prob = "APM32";
			}
		}
	} else {
		// JEP106 not used. Legacy ASCII values are used. This should not be used
		// on new products. And this note was written in the ADI v5 specs.
		// The Only value I've been able to find is 0x41 for ARM.

		// The identity/contiuation code are not filled acoording the JEP106
		// According to speds, this is the legacy idenitification where
		// the Identity Code contains an ASCII value. On the HK32 we read
		// JEP106 = false / Identity = 0x55 / Continuation = 5
		// 0x55 corresponds with 'U'. This looks like this could be an ASCII Identifier.
		// However, if ASCII IDs are used, the expected Continuation would be 0, as
		// this field is "reserved, read as zero" when legacy ASCII IDs are used.

		// Even though these values are violating the specs, we can use
		// JEP106 = false, ID = 0x55, Cont = 5 to detect HK32.

		if (romtable_pid.identity_code == 0x55
				&& romtable_pid.continuation_code == 5) prob = "HK32";
	}

	sprintf(_bufTemp, "%s %s V:%1d CONT:0x%02x ID:0x%02x PART:0x%04x REV:0x%02x ",(char*) prob,
				(char*)cpuid(), romtable_pid.jep106_used, romtable_pid.continuation_code,
				romtable_pid.identity_code, romtable_pid.partno,
				romtable_pid.revision);
	return _bufTemp;

}

char* get_coreID(void) {
	intptr_t ROMTABLE = (intptr_t) (0xE00FF000);
	romtable_id_t *rid = (romtable_id_t*) (ROMTABLE | 0xFD0);
	romtable_pid_t romtable_pid = extract_romtable_pid(rid);

	char *prob = "Unknown";

	if (romtable_pid.jep106_used) {
		if (romtable_pid.identity_code == 32
				&& romtable_pid.continuation_code == 0) {
			prob = "STM32";
		}
		if (romtable_pid.identity_code == 81
				&& romtable_pid.continuation_code == 7) {
			prob = "GD32";
		}
		if (romtable_pid.identity_code == 59
				&& romtable_pid.continuation_code == 4) {
			// APM or CS
			cortex_m_romtable_t *rom = (cortex_m_romtable_t*) (ROMTABLE);
			if (rom->etm & 1) {
				prob = "CS32";
			} else {
				prob = "APM32";
			}
		}
	} else {
		// JEP106 not used. Legacy ASCII values are used. This should not be used
		// on new products. And this note was written in the ADI v5 specs.
		// The Only value I've been able to find is 0x41 for ARM.

		// The identity/contiuation code are not filled acoording the JEP106
		// According to speds, this is the legacy idenitification where
		// the Identity Code contains an ASCII value. On the HK32 we read
		// JEP106 = false / Identity = 0x55 / Continuation = 5
		// 0x55 corresponds with 'U'. This looks like this could be an ASCII Identifier.
		// However, if ASCII IDs are used, the expected Continuation would be 0, as
		// this field is "reserved, read as zero" when legacy ASCII IDs are used.

		// Even though these values are violating the specs, we can use
		// JEP106 = false, ID = 0x55, Cont = 5 to detect HK32.

		if (romtable_pid.identity_code == 0x55
				&& romtable_pid.continuation_code == 5) prob = "HK32";
	}
return prob;
}

char* get_romtable(void) {
	intptr_t ROMTABLE = (intptr_t) (0xE00FF000);
	romtable_id_t *rid = (romtable_id_t*) (ROMTABLE | 0xFD0);
	romtable_pid_t romtable_pid = extract_romtable_pid(rid);

	sprintf(_bufTemp, "V:%1d CONT:0x%02x ID:0x%02x PART:0x%04x REV:0x%02x ",
				romtable_pid.jep106_used, romtable_pid.continuation_code,
				romtable_pid.identity_code, romtable_pid.partno,
				romtable_pid.revision);
	return _bufTemp;

}

