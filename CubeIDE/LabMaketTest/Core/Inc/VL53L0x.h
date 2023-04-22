#ifndef VL53L0X_h
#define VL53L0X_h

#include "main.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define ADDRESS_DEFAULT 0b0101001

#define SYSRANGE_START                              0x00

#define SYSTEM_THRESH_HIGH                          0x0C
#define SYSTEM_THRESH_LOW                           0x0E

#define SYSTEM_SEQUENCE_CONFIG                      0x01
#define SYSTEM_RANGE_CONFIG                         0x09
#define SYSTEM_INTERMEASUREMENT_PERIOD              0x04

#define SYSTEM_INTERRUPT_CONFIG_GPIO                0x0A

#define GPIO_HV_MUX_ACTIVE_HIGH                     0x84

#define SYSTEM_INTERRUPT_CLEAR                      0x0B

#define RESULT_INTERRUPT_STATUS                     0x13
#define RESULT_RANGE_STATUS                         0x14

#define RESULT_CORE_AMBIENT_WINDOW_EVENTS_RTN       0xBC
#define RESULT_CORE_RANGING_TOTAL_EVENTS_RTN        0xC0
#define RESULT_CORE_AMBIENT_WINDOW_EVENTS_REF       0xD0
#define RESULT_CORE_RANGING_TOTAL_EVENTS_REF        0xD4
#define RESULT_PEAK_SIGNAL_RATE_REF                 0xB6

#define ALGO_PART_TO_PART_RANGE_OFFSET_MM           0x28

#define I2C_SLAVE_DEVICE_ADDRESS                    0x8A

#define MSRC_CONFIG_CONTROL                         0x60

#define PRE_RANGE_CONFIG_MIN_SNR                    0x27
#define PRE_RANGE_CONFIG_VALID_PHASE_LOW            0x56
#define PRE_RANGE_CONFIG_VALID_PHASE_HIGH           0x57
#define PRE_RANGE_MIN_COUNT_RATE_RTN_LIMIT          0x64

#define FINAL_RANGE_CONFIG_MIN_SNR                  0x67
#define FINAL_RANGE_CONFIG_VALID_PHASE_LOW          0x47
#define FINAL_RANGE_CONFIG_VALID_PHASE_HIGH         0x48
#define FINAL_RANGE_CONFIG_MIN_COUNT_RATE_RTN_LIMIT 0x44

#define PRE_RANGE_CONFIG_SIGMA_THRESH_HI            0x61
#define PRE_RANGE_CONFIG_SIGMA_THRESH_LO            0x62

#define PRE_RANGE_CONFIG_VCSEL_PERIOD               0x50
#define PRE_RANGE_CONFIG_TIMEOUT_MACROP_HI          0x51
#define PRE_RANGE_CONFIG_TIMEOUT_MACROP_LO          0x52

#define SYSTEM_HISTOGRAM_BIN                        0x81
#define HISTOGRAM_CONFIG_INITIAL_PHASE_SELECT       0x33
#define HISTOGRAM_CONFIG_READOUT_CTRL               0x55

#define FINAL_RANGE_CONFIG_VCSEL_PERIOD             0x70
#define FINAL_RANGE_CONFIG_TIMEOUT_MACROP_HI        0x71
#define FINAL_RANGE_CONFIG_TIMEOUT_MACROP_LO        0x72
#define CROSSTALK_COMPENSATION_PEAK_RATE_MCPS       0x20

#define MSRC_CONFIG_TIMEOUT_MACROP                  0x46

#define SOFT_RESET_GO2_SOFT_RESET_N                 0xBF
#define IDENTIFICATION_MODEL_ID                     0xC0
#define IDENTIFICATION_REVISION_ID                  0xC2

#define OSC_CALIBRATE_VAL                           0xF8

#define GLOBAL_CONFIG_VCSEL_WIDTH                   0x32
#define GLOBAL_CONFIG_SPAD_ENABLES_REF_0            0xB0
#define GLOBAL_CONFIG_SPAD_ENABLES_REF_1            0xB1
#define GLOBAL_CONFIG_SPAD_ENABLES_REF_2            0xB2
#define GLOBAL_CONFIG_SPAD_ENABLES_REF_3            0xB3
#define GLOBAL_CONFIG_SPAD_ENABLES_REF_4            0xB4
#define GLOBAL_CONFIG_SPAD_ENABLES_REF_5            0xB5

#define GLOBAL_CONFIG_REF_EN_START_SELECT           0xB6
#define DYNAMIC_SPAD_NUM_REQUESTED_REF_SPAD         0x4E
#define DYNAMIC_SPAD_REF_EN_START_OFFSET            0x4F
#define POWER_MANAGEMENT_GO1_POWER_FORCE            0x80

#define VHV_CONFIG_PAD_SCL_SDA__EXTSUP_HV           0x89

#define ALGO_PHASECAL_LIM                           0x30
#define ALGO_PHASECAL_CONFIG_TIMEOUT                0x30

typedef struct __VL53L0X
{
    uint8_t address;
    uint32_t io_timeout;
    bool did_timeout;
    uint32_t timeout_start_ms;

    uint8_t stop_variable; // read by init and used when starting measurement; is StopVariable field of VL53L0X_DevData_t structure in API
    uint32_t measurement_timing_budget_us;
    uint8_t last_status; // status of last I2C transmission

} VL53L0X;

	typedef enum { VcselPeriodPreRange, VcselPeriodFinalRange } vcselPeriodType;

	typedef struct __SequenceStepEnables
    {
      bool tcc, msrc, dss, pre_range, final_range;
    } SequenceStepEnables;

    typedef struct __SequenceStepTimeouts
    {
      uint16_t pre_range_vcsel_period_pclks, final_range_vcsel_period_pclks;

      uint16_t msrc_dss_tcc_mclks, pre_range_mclks, final_range_mclks;
      uint32_t msrc_dss_tcc_us,    pre_range_us,    final_range_us;
    } SequenceStepTimeouts;

	void setup_VL53L0X(VL53L0X * lidar);
	void setAddress(VL53L0X * lidar,uint8_t new_addr);
    uint8_t getAddress(VL53L0X * lidar);
    uint8_t getWhoAmI(VL53L0X * lidar);

    bool init(VL53L0X * lidar, bool io_2v8); // bool io_2v8 = true

    void writeReg(VL53L0X * lidar,uint8_t reg, uint8_t value);
    void writeReg16Bit(VL53L0X * lidar,uint8_t reg, uint16_t value);
    void writeReg32Bit(VL53L0X * lidar,uint8_t reg, uint32_t value);
    uint8_t readReg(VL53L0X * lidar,uint8_t reg);
    uint16_t readReg16Bit(VL53L0X * lidar,uint8_t reg);
    uint32_t readReg32Bit(VL53L0X * lidar,uint8_t reg);

    void writeMulti(VL53L0X * lidar,uint8_t reg, uint8_t * src, uint8_t count);
    void readMulti(VL53L0X * lidar,uint8_t reg, uint8_t * dst, uint8_t count);

    bool setSignalRateLimit(VL53L0X * lidar,float limit_Mcps);
    float getSignalRateLimit(VL53L0X * lidar);

    bool setMeasurementTimingBudget(VL53L0X * lidar,uint32_t budget_us);
    uint32_t getMeasurementTimingBudget(VL53L0X * lidar);

    bool setVcselPulsePeriod(VL53L0X * lidar,vcselPeriodType type, uint8_t period_pclks);
    uint8_t getVcselPulsePeriod(VL53L0X * lidar,vcselPeriodType type);

    void startContinuous(VL53L0X * lidar, uint32_t period_ms); ////////////////////////////uint32_t period_ms = 0
    void stopContinuous(VL53L0X * lidar);
    uint16_t readRangeContinuousMillimeters(VL53L0X * lidar);
    uint16_t readRangeSingleMillimeters(VL53L0X * lidar);

    void setTimeout(VL53L0X * lidar,uint16_t timeout);
    uint16_t getTimeout(VL53L0X * lidar);
    bool timeoutOccurred(VL53L0X * lidar);
	bool getSpadInfo(VL53L0X * lidar,uint8_t * count, bool * type_is_aperture);

	void getSequenceStepEnables(VL53L0X * lidar,SequenceStepEnables * enables);
	void getSequenceStepTimeouts(VL53L0X * lidar,SequenceStepEnables const * enables, SequenceStepTimeouts * timeouts);

	bool performSingleRefCalibration(VL53L0X * lidar,uint8_t vhv_init_byte);

	uint16_t decodeTimeout(uint16_t value);
	uint16_t encodeTimeout(uint16_t timeout_mclks);
	uint32_t timeoutMclksToMicroseconds(uint16_t timeout_period_mclks, uint8_t vcsel_period_pclks);
	uint32_t timeoutMicrosecondsToMclks(uint32_t timeout_period_us, uint8_t vcsel_period_pclks);

	void startTimeout(VL53L0X * lidar);
	bool checkTimeoutExpired(VL53L0X * lidar);
	uint8_t decodeVcselPeriod(uint8_t reg_val);
	uint8_t encodeVcselPeriod(uint8_t period_pclks);
	uint32_t calcMacroPeriod(uint8_t vcsel_period_pclks);


// ==============================
	/*
	 * I2CDev.h через HAL
	 *
	 *  Created on: 5 Apr 2018
	 *      Author: Adam
	 */

	#include "stm32f1xx_hal.h"
	#include <stdbool.h>
	#include <stdint.h>
	#include <stdlib.h>

	#define I2CTIMEOUT 0x100

	static int8_t readBit(I2C_HandleTypeDef *handler, uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t *data, uint16_t timeout);
	static int8_t readBits(I2C_HandleTypeDef *handler, uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data, uint16_t timeout);
	static int8_t readByte(I2C_HandleTypeDef *handler, uint8_t devAddr, uint8_t regAddr, uint8_t *data, uint16_t timeout);
	static int8_t readWord(I2C_HandleTypeDef *handler, uint8_t devAddr, uint8_t regAddr, uint16_t *data, uint16_t timeout);
	static int8_t readBytes(I2C_HandleTypeDef *handler, uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data, uint16_t timeout);
	//static int8_t readWords(I2C_HandleTypeDef *handler, uint8_t devAddr, uint8_t regAddr, uint8_t length, uint16_t *data, uint16_t timeout);
	static int8_t read32Bits(I2C_HandleTypeDef *handler, uint8_t devAddr, uint8_t regAddr, uint32_t *data, uint16_t timeout);

	static bool writeBit(I2C_HandleTypeDef *handler, uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t data);
	static bool writeBits(I2C_HandleTypeDef *handler, uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data);
	static bool writeByte(I2C_HandleTypeDef *handler, uint8_t devAddr, uint8_t regAddr, uint8_t data);
	static bool writeWord(I2C_HandleTypeDef *handler, uint8_t devAddr, uint8_t regAddr, uint16_t data);
	static bool writeBytes(I2C_HandleTypeDef *handler, uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data);
	static bool write32Bits(I2C_HandleTypeDef *handler, uint8_t devAddr, uint8_t regAddr, uint32_t data);
	//static bool writeWords(I2C_HandleTypeDef *handler, uint8_t devAddr, uint8_t regAddr, uint8_t length, uint16_t *data);


	int8_t readBit(I2C_HandleTypeDef *handler, uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t *data, uint16_t timeout) {
	    uint8_t b;
	    uint8_t count = readByte(handler, devAddr, regAddr, &b, timeout);
	    *data = b & (1 << bitNum);
	    return count;
	}

	int8_t readBits(I2C_HandleTypeDef *handler, uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data, uint16_t timeout) {
	    // 01101001 read byte
	    // 76543210 bit numbers
	    //    yyy   args: bitStart=4, length=3
	    //    010   masked
	    //   -> 010 shifted
	    uint8_t count,b;
	    if ((count = readByte(handler, devAddr, regAddr, &b, timeout)) != 0) {
	        uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
	        b &= mask;
	        b >>= (bitStart - length + 1);
	        *data = b;
	    }
	    return count;
	}

	int8_t readByte(I2C_HandleTypeDef *handler, uint8_t devAddr, uint8_t regAddr, uint8_t *data, uint16_t timeout) {
	    return readBytes(handler, devAddr, regAddr, 1, data, timeout);
	}

	int8_t readBytes(I2C_HandleTypeDef *handler, uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data, uint16_t timeout){
		int8_t count = length;
		HAL_I2C_Mem_Read(handler, devAddr<<1, (uint16_t)regAddr, 1, data, (uint16_t)length, (uint32_t)timeout);
		return count;
	}

	int8_t readWord(I2C_HandleTypeDef *handler, uint8_t devAddr, uint8_t regAddr, uint16_t *data, uint16_t timeout) {
		int8_t count = 2;
		uint8_t temp[4];
		readBytes(handler, devAddr, regAddr, 2, (uint8_t*)temp, timeout);
		*data = (((uint16_t)temp[0])<<8) | (temp[1]);
		return count;
	}

	int8_t read32Bits(I2C_HandleTypeDef *handler, uint8_t devAddr, uint8_t regAddr, uint32_t *data, uint16_t timeout){
		int8_t count = 4;
		uint8_t temp[8];
		readBytes(handler,devAddr,regAddr,4,(uint8_t*)temp,timeout);
		*data =  ((uint32_t)temp[0])<<24;
		*data |= ((uint32_t)temp[1])<<16;
		*data |= ((uint32_t)temp[2])<<8;
		*data |= ((uint32_t)temp[3]);
		return count;
	}

	bool writeBit(I2C_HandleTypeDef *handler, uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t data) {
	    uint8_t b;
	    readByte(handler, devAddr, regAddr, &b,100);
	    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
	    return writeByte(handler, devAddr, regAddr, b);
	}

	bool writeBits(I2C_HandleTypeDef *handler, uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data) {
	    //      010 value to write
	    // 76543210 bit numbers
	    //    yyy   args: bitStart=4, length=3
	    // 00011100 mask byte
	    // 10101111 original value (sample)
	    // 10100011 original & ~mask
	    // 10101011 masked | value
	    uint8_t b;
	    if (readByte(handler, devAddr, regAddr, &b,100) != 0) {
	        uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
	        data <<= (bitStart - length + 1); // shift data into correct position
	        data &= mask; // zero all non-important bits in data
	        b &= ~(mask); // zero all important bits in existing byte
	        b |= data; // combine data with existing byte
	        return writeByte(handler, devAddr, regAddr, b);
	    } else {
	        return false;
	    }
	}

	bool writeByte(I2C_HandleTypeDef *handler, uint8_t devAddr, uint8_t regAddr, uint8_t data) {
	    return writeBytes(handler, devAddr, regAddr, 1, &data);
	}

	bool writeBytes(I2C_HandleTypeDef *handler, uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data){
		uint8_t status = 0;
		HAL_I2C_Mem_Write(handler, devAddr<<1, (uint16_t)regAddr,1,data, (uint16_t)length, (uint32_t)100);
		return status == 0;
	}
	bool writeWord(I2C_HandleTypeDef *handler, uint8_t devAddr, uint8_t regAddr, uint16_t data){
		uint8_t towrite[2];
		towrite[0]=(uint8_t)data>>8 & 0xFF; //MSB
		towrite[1]=(uint8_t)data & 0xFF; //LSB
		writeBytes(handler, devAddr, regAddr, 2,(uint8_t*)towrite);
		return true;
	}
	bool write32Bits(I2C_HandleTypeDef *handler, uint8_t devAddr, uint8_t regAddr, uint32_t data){
		uint8_t towrite[4];
		towrite[0]=(uint8_t)data>>24 & 0xFF; //MSB
		towrite[1]=(uint8_t)data>>16 & 0xFF;
		towrite[2]=(uint8_t)data>>8 & 0xFF;
		towrite[3]=(uint8_t)data & 0xFF;
		writeBytes(handler, devAddr, regAddr, 4,(uint8_t*)towrite);
		return true;
	}


#endif


