/*------------------------------------------------------------------------------------------------
 *	Version: 1.0
 *  Build:  20260304
 *	Copyright (c) 2025 by Nam Pham
 *	All rights reserved.
 *	
 *	This library provides basic DS1307 functionality using an STM32. 
 *  For more details, see the IC datasheet.
------------------------------------------------------------------------------------------------*/
#ifndef __DS1307_H__
#define __DS1307_H__

#include "main.h" 
#include "stdbool.h"
#include "string.h"

#define DS1307_LEN_RX 7
#define DS1307_LEN_TX 7

// DS1307 I2C address
#define DS1307_ADDR							(0x68<<1)

// Address register DS1307
#define REG_SECOND_ADDR					0x00 
#define REG_MINUTE_ADDR					0x01 
#define REG_HOUR_ADDR						0x02 
#define REG_DAY_OF_WEEK_ADDR		0x03 
#define REG_DAY_ADDR						0x04 
#define REG_MONTH_ADDR					0x05 
#define REG_YEAR_ADDR						0x06
#define REG_CONTROL_ADDR				0x07


#define INDEX_SECOND				0 
#define INDEX_MINUTE				1 
#define INDEX_HOUR					2 
#define INDEX_DAY_OF_WEEK		3 
#define INDEX_DAY						4 
#define INDEX_MONTH					5 
#define INDEX_YEAR					6

#define DS1307_NOT_FOUND		-1

typedef struct
{
	I2C_HandleTypeDef *I2C_Port;
	uint8_t day;
	uint8_t month;
	uint16_t year;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	uint8_t dayOfWeek;
	bool RxOk;
	bool RxErr;
	uint8_t dataRx[DS1307_LEN_RX];
	uint8_t dataTx[DS1307_LEN_TX];
}DS1307_TypeDef;

uint8_t BCD_TO_DEC(uint8_t bin);
uint8_t DEC_TO_BCD(uint8_t dec);
HAL_StatusTypeDef DS1307_READ_RAW(DS1307_TypeDef *DS1307);
void DS1307_PARSE_RTC(DS1307_TypeDef *DS1307);
HAL_StatusTypeDef DS1307_INIT(DS1307_TypeDef *DS1307,I2C_HandleTypeDef *hi2c);
void DS1307_READ_RTC(DS1307_TypeDef *DS1307);
uint8_t DS1307_WRITE_RTC(DS1307_TypeDef *DS1307,uint8_t hour, uint8_t min, uint8_t sec,uint8_t dayOfWeek, uint8_t day, uint8_t mon, uint16_t year);


#endif
