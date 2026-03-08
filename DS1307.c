/*------------------------------------------------------------------------------------------------
 *	Version: 1.0
 *  Build:  20260304
 *	Copyright (c) 2025 by Nam Pham
 *	All rights reserved.
 *	
 *	This library provides basic DS1307 functionality using an STM32. 
 *  For more details, see the IC datasheet.
------------------------------------------------------------------------------------------------*/

#include "DS1307.h"

DS1307_TypeDef *DS1307Sys;

/*------------------------------------------------------------------------------------------------
	+ Function: BCD_TO_DEC
	+ Description: Convert BCD to decimal
------------------------------------------------------------------------------------------------*/
uint8_t BCD_TO_DEC(uint8_t bin)
{
	return (((bin>>4)*10)+(bin&0x0F));
}

/*------------------------------------------------------------------------------------------------
	+ Function: DEC_TO_BCD
	+ Description: Convert decimal to BCD
------------------------------------------------------------------------------------------------*/
uint8_t DEC_TO_BCD(uint8_t dec)
{
	return (((dec/10)<<4) | (dec%10));
}

/*------------------------------------------------------------------------------------------------
	+ Function: DS1307_INIT
	+ Description: Initialization parameters for the DS1307
	+ PARA IN: port hi2c, structDS1307
	+ PARA OUT: none
------------------------------------------------------------------------------------------------*/

HAL_StatusTypeDef DS1307_INIT(DS1307_TypeDef *DS1307,I2C_HandleTypeDef *hi2c)
{
	HAL_StatusTypeDef stt=HAL_ERROR;
	DS1307->I2C_Port=hi2c;
	DS1307Sys=DS1307;
	DS1307->RxOk=false;
	DS1307->RxErr=false;
	
	HAL_I2C_DeInit(DS1307->I2C_Port);
	DS1307->I2C_Port->Init.ClockSpeed = 100000;
  DS1307->I2C_Port->Init.DutyCycle = I2C_DUTYCYCLE_2;
  DS1307->I2C_Port->Init.OwnAddress1 = 0;
  DS1307->I2C_Port->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  DS1307->I2C_Port->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  DS1307->I2C_Port->Init.OwnAddress2 = 0;
  DS1307->I2C_Port->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  DS1307->I2C_Port->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	HAL_I2C_Init(DS1307->I2C_Port);
	
	stt=HAL_I2C_IsDeviceReady(DS1307->I2C_Port,DS1307_ADDR,2,1000); // Polling ACK
	return stt;
}

/*------------------------------------------------------------------------------------------------
	+ Function: DS1307_WRITE_DATA
	+ Description: Write data to DS1307. This Function will polling ack after send data
	+ PARA IN: struct DS1307, hour, min, sec, dayOfWeek, day, mon, year
	+ PARA OUT: status send I2C
------------------------------------------------------------------------------------------------*/

uint8_t DS1307_WRITE_RTC(DS1307_TypeDef *DS1307,uint8_t hour, uint8_t min, uint8_t sec,uint8_t dayOfWeek, uint8_t day, uint8_t mon, uint16_t year)
{
	uint8_t stt=0;
	
	memset(DS1307->dataTx,0,DS1307_LEN_TX);
	DS1307->dataTx[INDEX_SECOND]=DEC_TO_BCD(sec);
	DS1307->dataTx[INDEX_MINUTE]=DEC_TO_BCD(min);
	DS1307->dataTx[INDEX_HOUR]=DEC_TO_BCD(hour);
	DS1307->dataTx[INDEX_DAY_OF_WEEK]=DEC_TO_BCD(dayOfWeek);
	DS1307->dataTx[INDEX_DAY]=DEC_TO_BCD(day);
	DS1307->dataTx[INDEX_MONTH]=DEC_TO_BCD(mon);
	DS1307->dataTx[INDEX_YEAR]=DEC_TO_BCD(year%100);

	if (HAL_I2C_GetState(DS1307->I2C_Port) == HAL_I2C_STATE_READY)
	{
    stt=HAL_I2C_Mem_Write(DS1307->I2C_Port,DS1307_ADDR,REG_SECOND_ADDR,I2C_MEMADD_SIZE_8BIT,DS1307->dataTx,sizeof(DS1307->dataTx),100);
	}
	else
	{
		return HAL_I2C_GetState(DS1307->I2C_Port);
	}
	return stt;
}

/*------------------------------------------------------------------------------------------------
	+ Function: DS1307_READ_DATA
	+ Description: Read data from DS1307. This Function will polling ack after send data
	+ PARA IN: struct DS1307, buff_rx
	+ PARA OUT: status send I2C
------------------------------------------------------------------------------------------------*/

HAL_StatusTypeDef DS1307_READ_RAW(DS1307_TypeDef *DS1307)
{
    if (HAL_I2C_GetState(DS1307->I2C_Port) != HAL_I2C_STATE_READY)
        return HAL_BUSY;
		
    return HAL_I2C_Mem_Read(DS1307->I2C_Port,DS1307_ADDR,REG_SECOND_ADDR,I2C_MEMADD_SIZE_8BIT,DS1307->dataRx,7,100);
}

void DS1307_PARSE_RTC(DS1307_TypeDef *DS1307)
{
  DS1307->second    = BCD_TO_DEC(DS1307->dataRx[INDEX_SECOND] & 0x7F);
  DS1307->minute    = BCD_TO_DEC(DS1307->dataRx[INDEX_MINUTE]);
  DS1307->hour      = BCD_TO_DEC(DS1307->dataRx[INDEX_HOUR] & 0x3F);
  DS1307->dayOfWeek = BCD_TO_DEC(DS1307->dataRx[INDEX_DAY_OF_WEEK]);
  DS1307->day       = BCD_TO_DEC(DS1307->dataRx[INDEX_DAY]);
  DS1307->month     = BCD_TO_DEC(DS1307->dataRx[INDEX_MONTH]);
  DS1307->year      = BCD_TO_DEC(DS1307->dataRx[INDEX_YEAR]) + 2000;
}

void DS1307_READ_RTC(DS1307_TypeDef *DS1307)
{
	if(DS1307_READ_RAW(DS1307)==HAL_OK)
	{
		DS1307_PARSE_RTC(DS1307);
	}
}