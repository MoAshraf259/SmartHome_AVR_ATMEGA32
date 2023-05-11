
/*
 * EEPROM.h
 *
 *  Created on: May 7, 2023
 *      Author: moham
 */

#ifndef HAL_EEPROM_EEPROM_H_
#define HAL_EEPROM_EEPROM_H_

#include "atmega32.h"

#define EEPROM_ADDRESS	(0b1010000)
#define EEPROM_A2_ADDRESS	1

#define EEPROM_PAGE1		0b00
#define EEPROM_PAGE2		0b01
#define EEPROM_PAGE3		0b10
#define EEPROM_PAGE4		0b11

void EEPROM_SendToData(uint8_t data,uint16_t ByteNumber);
void EEPROM_ReceiveFromData(uint8_t *data,uint16_t ByteNumber,uint8_t Length);
void I2C_GenerateStartCond(void);
void EEPROM_SendToDataIT(I2C_Handle_t *pI2C_Handle,uint8_t data,uint8_t ByteNumber,uint8_t PageNumber);
void EEPROM_ReceiveFromDataIT(I2C_Handle_t *pI2C_Handle,uint8_t *data,uint8_t ByteNumber,uint8_t PageNumber);
#endif /* HAL_EEPROM_EEPROM_H_ */
