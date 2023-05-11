
/*
 * EEPROM.c
 *
 *  Created on: May 7, 2023
 *      Author: moham
 */
#include "EEPROM.h"
#include "LCD.h"
void EEPROM_SendToData(uint8_t data,uint16_t ByteNumber)
{
	uint8_t arr[2];

	arr[0]=(uint8_t)ByteNumber;
	arr[1]=data;

	uint8_t address = ((EEPROM_ADDRESS)|(EEPROM_A2_ADDRESS<<2)|(uint8_t)(ByteNumber>>8));

	I2C_MasterSendData(arr, sizeof(arr), address,0);
	_delay_ms(20);
}

void EEPROM_ReceiveFromData(uint8_t *data,uint16_t ByteNumber,uint8_t Length)
{
	uint8_t address = ((EEPROM_ADDRESS)|(EEPROM_A2_ADDRESS<<2)|(uint8_t)(ByteNumber>>8));
	uint8_t temp=(uint8_t)ByteNumber;
	I2C_MasterSendData(&temp, 1, address,1);
	_delay_ms(10);
	I2C_MasterRecieveData(data, Length, address,1);

}

void EEPROM_SendToDataIT(I2C_Handle_t *pI2C_Handle,uint8_t data,uint8_t ByteNumber,uint8_t PageNumber)
{
	uint8_t arr[2]={ByteNumber,data};
	uint8_t address=EEPROM_ADDRESS | (PageNumber);
	(pI2C_Handle->TxRxState)=4;
	while(I2C_MasterSendDataIT(pI2C_Handle,arr, sizeof(arr), address,0)!=I2C_Ready);

	_delay_ms(10);
}
void EEPROM_ReceiveFromDataIT(I2C_Handle_t *pI2C_Handle,uint8_t *data,uint8_t ByteNumber,uint8_t PageNumber)
{
	uint8_t address=EEPROM_ADDRESS | (PageNumber);
	(pI2C_Handle->TxRxState)=4;
	while(I2C_MasterSendDataIT(pI2C_Handle,&ByteNumber, 1, address,1)!=I2C_Ready);
	(pI2C_Handle->TxRxState)=4;
	while(I2C_MasterRecieveDataIT(pI2C_Handle,data, 1, address,1)!=I2C_Ready);
}
