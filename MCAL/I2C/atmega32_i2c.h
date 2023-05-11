
/*
 * atmega32_i2c.h
 *
 *  Created on: Apr 11, 2023
 *      Author: moham
 */

#ifndef MCAL_I2C_ATMEGA32_I2C_H_
#define MCAL_I2C_ATMEGA32_I2C_H_


#include "atmega32.h"


/***********Private macros*************/
#define STATUS_MASK              0b11111000


/***********Actions' IDs***************/

#define START_ACK                0x08 /* start has been sent */
#define REP_START_ACK            0x10 /* repeated start */
#define SLAVE_ADDR_AND_WR_ACK     0x18 /* Master transmit ( slave address + Write request ) ACK */
#define SLAVE_ADDR_AND_RD_ACK     0x40 /* Master transmit ( slave address + Read request ) ACK */
#define MSTR_WR_BYTE_ACK         0x28 /* Master transmit data ACK */
#define MSTR_RD_BYTE_WITH_ACK    0x50 /* Master received data with ACK */
#define MSTR_RD_BYTE_WITH_NACK   0x58 /* Master received data with not ACK */
#define SLAVE_ADDR_RCVD_RD_REQ    0xA8 /* means that slave address is received with read request */
#define SLAVE_ADDR_RCVD_WR_REQ    0x60 /* means that slave address is received with write request */
#define SLAVE_DATA_RECEIVED      0x80 /* means that a byte is received */
#define SLAVE_BYTE_TRANSMITTED   0xB8 /* means that the written byte is transmitted */

#define I2C_Ready		0
#define Busy_In_Tx		1
#define Busy_In_Rx		2


#define I2C_Mode_Sync		1
#define I2C_Mode_Async		0

#define I2C_Ack_Disable		0
#define I2C_Ack_Enable		1

#define I2C_SCL_SPEED400K	1	//Set the TWBR Bits as 2  and PRESCALER Bits as 0
#define I2C_SCL_SPEED200K	2   //set the TWBR Bits as 12 and PRESCALER Bits as 0
#define I2C_SCL_SPEED100K	3	//Set the TWBR Bits as 32 and PRESCALER Bits as 0

#define I2C_GeneralCall_Enable		1
#define I2C_GeneralCall_Disable		0

typedef struct
{
	uint8_t I2C_Address;
	uint8_t I2C_Mode;
	uint8_t I2C_Prescaler;
	uint8_t I2C_Ack;
	uint8_t GenralCall_Status;
}I2C_Config_t;

typedef struct
{
	I2C_Config_t 	pI2C_Config;
	uint8_t 		*pTxBuffer; /* !< To store the app. Tx buffer address > */
	uint8_t 		*pRxBuffer;	/* !< To store the app. Rx buffer address > */
	uint32_t 		TxLen;		/* !< To store Tx len > */
	uint32_t 		RxLen;		/* !< To store Tx len > */
	uint8_t 		TxRxState;	/* !< To store Communication state > */
	uint8_t 		DevAddr;	/* !< To store slave/device address > */
    uint32_t        RxSize;		/* !< To store Rx size  > */
    uint8_t         Sr;			/* !< To store repeated start value  > */
}I2C_Handle_t;

void I2C_Init(I2C_Handle_t *pI2C_Handle);
void I2C_MasterSendData(uint8_t *pTxBuffer,uint8_t Length,uint8_t Address,uint8_t RS);
void I2C_MasterRecieveData(uint8_t *pRxBuffer,uint8_t Length, uint8_t Address,uint8_t RS);

void I2C_GenerateStartCond(void);
uint8_t I2C_MasterSendDataIT(I2C_Handle_t *pI2C_Handle,uint8_t *pRxBuffer,uint8_t Length , uint8_t Address,uint8_t Sr);
uint8_t I2C_MasterRecieveDataIT(I2C_Handle_t *pI2C_Handle,uint8_t *pRxBuffer,uint8_t Length, uint8_t Address,uint8_t Sr);

void I2C_ISRHandler(I2C_Handle_t *pI2C_Handle);
#endif /* MCAL_I2C_ATMEGA32_I2C_H_ */
