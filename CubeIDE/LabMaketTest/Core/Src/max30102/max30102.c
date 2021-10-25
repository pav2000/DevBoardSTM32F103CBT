#include "stm32f1xx_hal.h"
#include "max30102/max30102.h"
#include <math.h>

//#define MAX30102_ADDR_WRITE 0xae
//#define MAX30102_ADDR_READ 0xaf

#define RES_INTERRUPT_STATUS_1 0x00
#define RES_INTERRUPT_STATUS_2 0x01
#define RES_INTERRUPT_ENABLE_1 0x02
#define RES_INTERRUPT_ENABLE_2 0x03
#define RES_FIFO_WRITE_POINTER 0x04
#define RES_OVERFLOW_COUNTER 0x05
#define RES_FIFO_READ_POINTER 0x06
#define RES_FIFO_DATA_REGISTER 0x07
#define RES_FIFO_CONFIGURATION 0x08
#define RES_MODE_CONFIGURATION 0x09
#define RES_SPO2_CONFIGURATION 0x0a
#define RES_LED_PLUSE_AMPLITUDE_1 0x0c
#define RES_LED_PLUSE_AMPLITUDE_2 0x0d
#define RES_PROXIMITY_MODE_LED_PLUSE_AMPLITUDE 0x10
#define RES_MULTI_LED_MODE_CONTROL_1 0x11
#define RES_MULTI_LED_MODE_CONTROL_2 0x12
#define RES_DIE_TEMP_INTEGER 0x1f
#define RES_DIE_TEMP_FRACTION 0x20
#define RES_DIE_TEMPERATURE_CONFIG 0x21
#define RES_PROXIMITY_INTERRUPT_THRESHOLD 0x30
#define RES_REVISION_ID 0xfe
#define RES_PART_ID 0xff

extern I2C_HandleTypeDef hi2c1;
extern void maxim_heart_rate_and_oxygen_saturation(SAMPLE *s,//uint32_t *pun_ir_buffer,
		int32_t n_ir_buffer_length,
		//uint32_t *pun_red_buffer,
		int32_t *pn_spo2,
		int8_t *pch_spo2_valid,
		int32_t *pn_heart_rate,
		int8_t *pch_hr_valid);

void max30102_init()
{
    uint8_t data = 0;
    /*reset*/
    data = 0x40;
    HAL_I2C_Mem_Write(&hi2c1, MAX30102_ADDR_WRITE, RES_MODE_CONFIGURATION, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
    do
    {
        HAL_I2C_Mem_Read(&hi2c1, MAX30102_ADDR_READ, RES_MODE_CONFIGURATION, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
    } while (data & 0x40);
    //Новые данные прерваны
    data = 0x40;
    HAL_I2C_Mem_Write(&hi2c1, MAX30102_ADDR_WRITE, RES_INTERRUPT_ENABLE_1, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
    //    // Прерывание после преобразования температуры
    //    data = 0x02;
    //    HAL_I2C_Mem_Write(&hi2c1, MAX30102_ADDR_WRITE, RES_INTERRUPT_ENABLE_2, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
    //    /*25个采样未读中断*/
    //    data = 0x7;
    //    /* 快满中断*/
    //    data = 0x80;
    //    HAL_I2C_Mem_Write(&hi2c1, MAX30102_ADDR_WRITE, RES_INTERRUPT_ENABLE_1, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
    //    HAL_I2C_Mem_Write(&hi2c1, MAX30102_ADDR_WRITE, RES_FIFO_CONFIGURATION, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
    /*16384量程 50Hz 18位adc分辨率*/
    data = 0x63;//0x67;//0x63;
    HAL_I2C_Mem_Write(&hi2c1, MAX30102_ADDR_WRITE, RES_SPO2_CONFIGURATION, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
    /*灯的亮度*/
    data = 0x6A; // 21.4mA //0x47; // 14.2mA //
    HAL_I2C_Mem_Write(&hi2c1, MAX30102_ADDR_WRITE, RES_LED_PLUSE_AMPLITUDE_1, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
    HAL_I2C_Mem_Write(&hi2c1, MAX30102_ADDR_WRITE, RES_LED_PLUSE_AMPLITUDE_2, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
    // not use  in 30102
//    HAL_I2C_Mem_Write(&hi2c1, MAX30102_ADDR_WRITE, RES_PROXIMITY_MODE_LED_PLUSE_AMPLITUDE, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
    /*FIFO clear*/
    data = 0;
    HAL_I2C_Mem_Write(&hi2c1, MAX30102_ADDR_WRITE, RES_FIFO_WRITE_POINTER, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
    HAL_I2C_Mem_Write(&hi2c1, MAX30102_ADDR_WRITE, RES_OVERFLOW_COUNTER, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
    // not use  in 30102
//    HAL_I2C_Mem_Write(&hi2c1, MAX30102_ADDR_WRITE, RES_FIFO_READ_POINTER, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
    //data = 0x12;
    //HAL_I2C_Mem_Write(&hi2c1, MAX30102_ADDR_WRITE, RES_MULTI_LED_MODE_CONTROL_1, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
    //data = 0;
    //HAL_I2C_Mem_Write(&hi2c1, MAX30102_ADDR_WRITE, RES_MULTI_LED_MODE_CONTROL_2, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);

    /*interrupt status clear*/
    max30102_getStatus();
    // /*转换温度*/
    // data = 1;
    // HAL_I2C_Mem_Write(&hi2c1, MAX30102_ADDR_WRITE, RES_DIE_TEMPERATURE_CONFIG, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
    /*SPO2 Mode*/
    data = 0x03;
    HAL_I2C_Mem_Write(&hi2c1, MAX30102_ADDR_WRITE, RES_MODE_CONFIGURATION, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
}

uint8_t max30102_getUnreadSampleCount()
{
    uint8_t wr = 0, rd = 0;
    HAL_I2C_Mem_Read(&hi2c1, MAX30102_ADDR_READ, RES_FIFO_WRITE_POINTER, I2C_MEMADD_SIZE_8BIT, &wr, 1, 10);
    HAL_I2C_Mem_Read(&hi2c1, MAX30102_ADDR_READ, RES_FIFO_READ_POINTER, I2C_MEMADD_SIZE_8BIT, &rd, 1, 10);
    if ((wr - rd) < 0)
        return wr - rd + 32;
    else
        return wr - rd;
}

void max30102_getFIFO(SAMPLE *data, uint8_t sampleCount)
{
    uint8_t dataTemp[sampleCount*6];//5 * 6];
    //if (sampleCount > 5)
    //    sampleCount = 5;
    HAL_I2C_Mem_Read(&hi2c1, MAX30102_ADDR_READ, RES_FIFO_DATA_REGISTER,
                     I2C_MEMADD_SIZE_8BIT, dataTemp,
                     6 * sampleCount, 25);
    uint8_t i;
    for (i = 0; i < sampleCount; i++)
    {
        data[i].red = (((uint32_t)dataTemp[i * 6]) << 16 | ((uint32_t)dataTemp[i * 6 + 1]) << 8 | dataTemp[i * 6 + 2]) & 0x3ffff;
        data[i].iRed = (((uint32_t)dataTemp[i * 6 + 3]) << 16 | ((uint32_t)dataTemp[i * 6 + 4]) << 8 | dataTemp[i * 6 + 5]) & 0x3ffff;
    }
}

uint8_t max30102_getStatus()
{
    uint8_t data = 0, dataTemp = 0;
    HAL_I2C_Mem_Read(&hi2c1, MAX30102_ADDR_READ, RES_INTERRUPT_STATUS_1, I2C_MEMADD_SIZE_8BIT, &dataTemp, 1, 10);
    data = dataTemp;
    HAL_I2C_Mem_Read(&hi2c1, MAX30102_ADDR_READ, RES_INTERRUPT_STATUS_2, I2C_MEMADD_SIZE_8BIT, &dataTemp, 1, 10);
    return data | dataTemp;
}

// 用这个要读状态确定是否转换完
// float max30102_getTemperature()
// {
//     int8_t dataTemp = 0;
//     float data;
//     HAL_I2C_Mem_Read(&hi2c1, MAX30102_ADDR_READ, RES_DIE_TEMP_INTEGER, I2C_MEMADD_SIZE_8BIT, (uint8_t *)(&dataTemp), 1, 10);
//     data = dataTemp;
//     HAL_I2C_Mem_Read(&hi2c1, MAX30102_ADDR_READ, RES_DIE_TEMP_FRACTION, I2C_MEMADD_SIZE_8BIT, (uint8_t *)(&dataTemp), 1, 10);
//     data += dataTemp * 0.0625;
//     // 启动下一次转换
//     dataTemp = 1;
//     HAL_I2C_Mem_Write(&hi2c1, MAX30102_ADDR_WRITE, RES_DIE_TEMPERATURE_CONFIG, I2C_MEMADD_SIZE_8BIT, (uint8_t *)(&dataTemp), 1, 10);
//     return data;
// }

void max30102_OFF()
{
    uint8_t data = 0;
    HAL_I2C_Mem_Read(&hi2c1, MAX30102_ADDR_READ, RES_MODE_CONFIGURATION, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
    data |= 0x80;
    HAL_I2C_Mem_Write(&hi2c1, MAX30102_ADDR_WRITE, RES_MODE_CONFIGURATION, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
}

void max30102_ON()
{
    uint8_t data = 0;
    HAL_I2C_Mem_Read(&hi2c1, MAX30102_ADDR_READ, RES_MODE_CONFIGURATION, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
    data &= ~(0x80);
    HAL_I2C_Mem_Write(&hi2c1, MAX30102_ADDR_WRITE, RES_MODE_CONFIGURATION, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
}

/******************************计算******************************/

#define BUFF_SIZE 100//200
#define BUFF_SIZE_half BUFF_SIZE/2
SAMPLE sampleBuff[BUFF_SIZE];

//#define IRBUF 1500
//uint32_t	buff_ir[IRBUF];
//uint32_t	cntr_ir=0;

uint8_t heartRate = 0;
uint8_t spo2 = 0;
#define SPO2_AVER 13
uint8_t spo2_aver[2+SPO2_AVER] = {0,2,0,0,0,0,0,0,0};	// для усреднения сатурации 0 - усредненный 1 - счетчик
uint8_t heartRate_aver[2+SPO2_AVER] = {0,2,0,0,0,0,0,0,0};

int8_t validSPO2;
int8_t validHeartRate;

uint16_t redAC = 0;
uint32_t redDC = 0;
uint16_t iRedAC = 0;
uint32_t iRedDC = 0;

uint16_t redAC_prev = 0;
uint32_t redDC_prev = 0;
uint16_t iRedAC_prev = 0;
uint32_t iRedDC_prev = 0;

#define FILTER_LEVEL 8 /*滤波等级*/
void filter(SAMPLE *s)
{
    uint8_t i;
    uint32_t red = 0;
    uint32_t ired = 0;
    for (i = 0; i < FILTER_LEVEL - 1; i++)
    {
        red += sampleBuff[i].red;
        ired += sampleBuff[i].iRed;
    }
    s->red = (red + s->red) / FILTER_LEVEL;
    s->iRed = (ired + s->iRed) / FILTER_LEVEL;
}

void buffInsert(SAMPLE s)
{
    uint8_t i;
    for (i = BUFF_SIZE - 1; i > 0; i--)
    {
        sampleBuff[i].red = sampleBuff[i - 1].red;
        sampleBuff[i].iRed = sampleBuff[i - 1].iRed;
    }
    if(s.red > s.iRed) {	// Уровень с ик диода выше чем с красного
    	sampleBuff[0].red = s.iRed;
    	sampleBuff[0].iRed = s.red;
    } else {
    	sampleBuff[0].red = s.red;
    	sampleBuff[0].iRed = s.iRed;
    }

}

void calAcDc(uint16_t *rac, uint32_t *rdc, uint16_t *iac, uint32_t *idc)
{
    uint32_t rMax = sampleBuff[0].red;
    uint32_t rMin = sampleBuff[0].red;
    uint32_t iMax = sampleBuff[0].iRed;
    uint32_t iMin = sampleBuff[0].iRed;

    uint8_t i;
    for (i = 0; i < BUFF_SIZE; i++)
    {
        if (sampleBuff[i].red > rMax)
            rMax = sampleBuff[i].red;
        if (sampleBuff[i].red < rMin)
            rMin = sampleBuff[i].red;
        if (sampleBuff[i].iRed > iMax)
            iMax = sampleBuff[i].iRed;
        if (sampleBuff[i].iRed < iMin)
            iMin = sampleBuff[i].iRed;
    }
    *rac = rMax - rMin;
    *rdc = (rMax + rMin) / 2;
    *iac = iMax - iMin;
    *idc = (iMax + iMin) / 2;
}

uint32_t rMax = 2;
uint32_t rMin = 1;
uint32_t iMax = 2;
uint32_t iMin = 1;

uint32_t maxFindSteep=0;
uint32_t minFindSteep=0;

int16_t eachSampleDiff = 0;
uint8_t newpulse=0;

uint32_t	lenghtWave[2] = {0,0};

#define MIDDLEBUUF BUFF_SIZE
uint32_t middle_ir[1 + 1 + 1 + MIDDLEBUUF]; // 0 средняя 1 сумма 2 счетчик 3-302 значения
//uint64_t	middelingBuf=0;
void calcAcDcFIFO(SAMPLE s, uint16_t *rac, uint32_t *rdc, uint16_t *iac, uint32_t *idc) {
	// Уровень с ик диода выше чем с красного
    uint8_t i;
	for (i = BUFF_SIZE - 1; i > 0; i--) {
		sampleBuff[i].red = sampleBuff[i - 1].red;
		sampleBuff[i].iRed = sampleBuff[i - 1].iRed;
		middle_ir[3 + i] = middle_ir[3 + i - 1];
	}
	if(s.red > s.iRed) {	// Уровень с ик диода выше чем с красного
		sampleBuff[0].red = s.iRed;
		sampleBuff[0].iRed = s.red;
		middle_ir[3] = s.red;
	} else {
		sampleBuff[0].red = s.red;
		sampleBuff[0].iRed = s.iRed;
		middle_ir[3] = s.iRed;
	}

	{	// фильтрация
		uint32_t acc32 = (middle_ir[3] + middle_ir[4] + middle_ir[5]) / 3;// + middle_ir[6] + middle_ir[7]) / 5;
		middle_ir[3] = acc32;

		// среднеарифметическое значение в middle_ir[0]
		uint64_t acc64=0;
		for(uint32_t k=3; k<(MIDDLEBUUF+3); k++)
			acc64 += ((uint64_t)middle_ir[k]*(uint64_t)middle_ir[k]);
		acc64 = acc64/MIDDLEBUUF;
		middle_ir[0] = ( ((uint32_t)sqrt(acc64)));
	}

	if( (middle_ir[0] < middle_ir[3+BUFF_SIZE_half]) && (middle_ir[0] > middle_ir[3+BUFF_SIZE_half+2]) ) { // рост максимума
		maxFindSteep = 0;
		minFindSteep = 0;

    	iMax = 0;
    	iMin = 0xffffffff;
    	rMax = 0;
    	rMin = 0xffffffff;

		uint32_t max_num = 0;
		for(uint32_t k=0; k<BUFF_SIZE_half; k++) {// поиск максимума
			if( (middle_ir[0] > middle_ir[3+BUFF_SIZE_half-k]) && (middle_ir[0] < middle_ir[3+BUFF_SIZE_half+2-k]) )
				break; // уход в минус относительно средней
			if (middle_ir[3+BUFF_SIZE_half-k] > iMax){
				iMax = middle_ir[3+BUFF_SIZE_half-k];
				max_num = BUFF_SIZE_half-k;
			}
			maxFindSteep++;
		}
		for(uint32_t k=(max_num-10); k < (max_num+10); k++) {// уточнение максимума
			if (sampleBuff[k].iRed > iMax){
				iMax = sampleBuff[k].iRed;
				rMax = sampleBuff[k].red;
			}
		}
		uint32_t min_num=0;
		for(uint32_t k=0; k<(BUFF_SIZE_half-2); k++) {// поиск минимума
			if( (middle_ir[0] > middle_ir[3+BUFF_SIZE_half+k]) && (middle_ir[0] < middle_ir[3+BUFF_SIZE_half+2+k]) )
				break;
			if (middle_ir[3+BUFF_SIZE_half+k] < iMin){
				iMin = middle_ir[3+BUFF_SIZE_half+k];
				min_num = BUFF_SIZE_half+k;
			}
			minFindSteep++;
		}
		for(uint32_t k=(min_num-10); k < (min_num+10); k++) {// уточнение минимума
			if (sampleBuff[k].iRed < iMin){
				iMin = sampleBuff[k].iRed;
				rMin = sampleBuff[k].red;
			}
		}

		lenghtWave[0] = (maxFindSteep);
		lenghtWave[1] = (minFindSteep);
		newpulse = 1;
	}

    *rac = rMax - rMin;
    *rdc = (rMax + rMin) / 2;
    *iac = iMax - iMin;
    *idc = (iMax + iMin) / 2;
}



void max30102_cal()
{
    uint8_t unreadSampleCount = max30102_getUnreadSampleCount();

    unreadSampleCount = 1;
    SAMPLE sampleBuffTemp[unreadSampleCount];
    max30102_getFIFO(sampleBuffTemp, unreadSampleCount);
    static uint8_t eachBeatSampleCount = 0;    //Сколько образцов прошло это сердцебиение
    static uint8_t lastTenBeatSampleCount[10]; //过去十次心跳每一次的样本数
    static uint32_t last_iRed = 0;             //上一次红外的值，过滤后的
    uint8_t i, ii;
    uint32_t spo2_averrange;
    for (i = 0; i < unreadSampleCount; i++)
    {
        if (sampleBuffTemp[i].iRed < 40000) //Нет пальцев, нет расчета, пропустить
        {
        	heartRate = 0;
            spo2 = 0;
            eachSampleDiff = 0;
            newpulse = 0;
            continue;
        }
        //buffInsert(sampleBuffTemp[i]);
        //calAcDc(&redAC, &redDC, &iRedAC, &iRedDC);
        calcAcDcFIFO(sampleBuffTemp[i], &redAC, &redDC, &iRedAC, &iRedDC);
        filter(&sampleBuffTemp[i]);

        //计算spo2
        if((redAC == redAC_prev)&&(redDC == redDC_prev)&&(iRedAC == iRedAC_prev)&&(iRedDC == iRedDC_prev)){

        }else {
			float R = (((float)(redAC)) / ((float)(redDC))) / (((float)(iRedAC)) / ((float)(iRedDC)));
			if (R >= 0.36 && R < 0.66)
				spo2 = (uint8_t)(107 - 20 * R);
			else if (R >= 0.66 && R < 1)
				spo2 = (uint8_t)(129.64 - 54 * R);

			/*spo2_averrange = 0;
			spo2_aver[spo2_aver[1]] = spo2;
			if((spo2_aver[1] + 1) >= (2+SPO2_AVER))
				spo2_aver[1] = 2;
			else
				spo2_aver[1]++;
			for(ii=2; ii<(2+SPO2_AVER); ii++)
				spo2_averrange += spo2_aver[ii];
			spo2_aver[0] = spo2_averrange / SPO2_AVER;

			//частота сердечных сокращений,30-250ppm  count:200-12
			spo2_averrange = 0;
			heartRate_aver[heartRate_aver[1]] = 3000/(lenghtWave[0] + lenghtWave[1]);
			if((heartRate_aver[1] + 1) >= (2+SPO2_AVER))
				heartRate_aver[1] = 2;
			else
				heartRate_aver[1]++;
			for(ii=2; ii<(2+SPO2_AVER); ii++)
				spo2_averrange += heartRate_aver[ii];
			heartRate_aver[0] = spo2_averrange / SPO2_AVER;*/

			heartRate = 3000/(lenghtWave[0] + lenghtWave[1]);

			redAC_prev = redAC;
			redDC_prev = redDC;
			iRedAC_prev = iRedAC;
			iRedDC_prev = iRedDC;
        }


        eachSampleDiff = middle_ir[3 + BUFF_SIZE_half] - iMin;

    }

}

uint8_t max30102_getHeartRate() { return heartRate; }
uint8_t max30102_getSpO2() { return spo2; }
int16_t max30102_getDiff() { return eachSampleDiff; }
uint16_t max30102_getMaxMin() { return (uint16_t)(iMax - iMin); }

uint8_t max30102_getPulse() { return newpulse; }

