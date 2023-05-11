/*
 * atmega32_i2c.c
 *
 *  Created on: Apr 11, 2023
 *      Author: moham
 */


#include "atmega32_i2c.h"
#include "LCD.h"

uint8_t msg1[]="START ACK ";
uint8_t msg2[]="ADDRESS SENT";
//Helper Functions for the .c File of the I2C
static void I2C_ExcutePhaseAddress(I2C_Handle_t *pI2C_Handle);
static void I2C_HandleTXEMaster(I2C_Handle_t *pI2C_Handle);
static void I2C_HandleRXEMaster(I2C_Handle_t *pI2C_Handle);

static void I2C_DisablingACK(void);
static void I2C_EnableACK(void);

static void I2C_EnablePeripheral(void);
static void I2C_GenerateStopCond(void);

static void I2C_ClearStartCond(void);
static void I2C_ClearTWINTFlag(void);

static void I2C_DisablePeripheral(void);




void I2C_Init(I2C_Handle_t *pI2C_Handle)
{


	LCD_Init();
	//Enabling or Disabling the interrupt for the I2C Depending on the macro given by the user in main app
	if(pI2C_Handle->pI2C_Config.I2C_Mode==I2C_Mode_Sync)
	{
		/*Do nothing*/
	}
	else if(pI2C_Handle->pI2C_Config.I2C_Mode==I2C_Mode_Async)
	{
	EnableGlobalInterrupt();
	TWCR |=(1<< I2C_TWCR_TWIE);
	}

	//Enabling or Disabling the ACK bit
	TWCR |= pI2C_Handle->pI2C_Config.I2C_Ack << I2C_TWCR_TWEA;

	//Setting it`s own address so if another master wants to communicate with it as a slave
	I2C->TWAR = (pI2C_Handle->pI2C_Config.I2C_Address << 1);

	//Enabling or disabling the GENERAL call respond from this target
	I2C->TWAR |= (pI2C_Handle->pI2C_Config.GenralCall_Status<<I2C_TWAR_TWGCE);

	if(pI2C_Handle->pI2C_Config.I2C_Prescaler == I2C_SCL_SPEED400K)
	{
		I2C->TWBR = 2;
		I2C->TWSR &= ~(3<<I2C_TWSR_TWPS0);
	}
	else if(pI2C_Handle->pI2C_Config.I2C_Prescaler == I2C_SCL_SPEED200K)
	{
		I2C->TWBR = 12;
		I2C->TWSR &= ~(3<<I2C_TWSR_TWPS0);
	}
	else if(pI2C_Handle->pI2C_Config.I2C_Prescaler == I2C_SCL_SPEED100K)
	{
		I2C->TWBR = 32;
		I2C->TWSR &= ~(3<<I2C_TWSR_TWPS0);
	}


	//Enabling the I2C  Peripheral
	I2C_EnablePeripheral();
}


void I2C_MasterSendData(uint8_t *pTxBuffer,uint8_t Length,uint8_t Address,uint8_t RS)
{

	//Generate start condition to start the communication
	I2C_GenerateStartCond();

	I2C_ClearTWINTFlag();


	while(!((TWCR>>I2C_TWCR_TWINT)&0x1));

	while ((I2C->TWSR & STATUS_MASK )!=(START_ACK));

	//Sending the slave address
//LCD_SendNumber((Address<<1)& ~(0x1));
	I2C->TWDR =(Address<<1)& ~(0x1);


	I2C_ClearStartCond();

	//Clear the flag after checking if the slave address is received
	I2C_ClearTWINTFlag();

	//waiting for the slave to respond with ACK and the Address is right
	while ((I2C->TWSR & STATUS_MASK )!=(SLAVE_ADDR_AND_WR_ACK));



	while(Length!=0)
	{
		//Sending the data to the TWDR Register for the slave to get
		I2C->TWDR = *pTxBuffer;

		I2C_ClearTWINTFlag();

		while(!((TWCR>>I2C_TWCR_TWINT)&0x1));

		while ((I2C->TWSR & STATUS_MASK )!=(MSTR_WR_BYTE_ACK));

		pTxBuffer++;
		Length--;
	}
	if(RS==0)
	{
	I2C_GenerateStopCond();
	}

}
void I2C_MasterRecieveData(uint8_t *pRxBuffer,uint8_t Length, uint8_t Address,uint8_t RS)
{


	//Generate start condition to start the communication
	I2C_GenerateStartCond();

	I2C_ClearTWINTFlag();


	while(!((TWCR>>I2C_TWCR_TWINT)&0x1));

	if(RS==0)
	{
	while ((I2C->TWSR & STATUS_MASK )!=(START_ACK));
	}
	else
	{
	while ((I2C->TWSR & STATUS_MASK )!=(REP_START_ACK));
	}
	//Sending the slave address

	I2C->TWDR =(Address<<1) |(0x1);

	_delay_ms(10);

	I2C_ClearStartCond();

	I2C_ClearTWINTFlag();

	while(!((TWCR>>I2C_TWCR_TWINT)&(0x1)));

	//waiting for the slave to respond with ACK and the Address is right
	while ((I2C->TWSR & STATUS_MASK )!=(SLAVE_ADDR_AND_RD_ACK));

	I2C_ClearTWINTFlag();

	if(Length==1)
	{

		I2C_DisablingACK();
		_delay_ms(10);
		//Receiving the data required
		*pRxBuffer = I2C->TWDR;
		I2C_ClearTWINTFlag();

		while(!((TWCR>>I2C_TWCR_TWINT)&0x1));
		// wait for the master to respond that it received the byte with NACK
		while ((I2C->TWSR & STATUS_MASK )!=(MSTR_RD_BYTE_WITH_NACK));

	}
	else if(Length>=2)
	{

		while(Length!=0)
		{

			if(Length ==1)
				{
				I2C_DisablingACK();
				}

			_delay_ms(10);
			//Receiving the data required
			*pRxBuffer = I2C->TWDR;
			I2C_ClearTWINTFlag();

			while(!((TWCR>>I2C_TWCR_TWINT)&0x1));

			// wait for the master to respond that it received the byte with ACK or NACK
			if(Length <=1)
			{
				while ((I2C->TWSR & STATUS_MASK )!=(MSTR_RD_BYTE_WITH_NACK));
			}
			else{
				while ((I2C->TWSR & STATUS_MASK )!=(MSTR_RD_BYTE_WITH_ACK));
			}


			pRxBuffer++;
			Length--;
			}
	}

	I2C_GenerateStopCond();
	I2C_EnableACK();
}

uint8_t I2C_MasterSendDataIT(I2C_Handle_t *pI2C_Handle,uint8_t *pTxBuffer,uint8_t Length,uint8_t Address,uint8_t Sr)
{
	//Checking if the program is free to use I2C or not
	uint8_t busystate=pI2C_Handle->TxRxState;
	if(busystate != Busy_In_Rx && busystate != Busy_In_Tx)
	{
	pI2C_Handle->TxRxState=Busy_In_Tx;//Reserving the i2c for TX
	pI2C_Handle->pTxBuffer=pTxBuffer;
	pI2C_Handle->TxLen=Length;
	pI2C_Handle->DevAddr=Address;
	pI2C_Handle->Sr=Sr;

	I2C_GenerateStartCond();
	I2C_ClearTWINTFlag();
	}

	return busystate;//if this function in main returned I2C_Ready then it reached LINE 220
}

uint8_t I2C_MasterRecieveDataIT(I2C_Handle_t *pI2C_Handle,uint8_t *pRxBuffer,uint8_t Length , uint8_t Address,uint8_t Sr)
{
	//Checking if the program is free to use I2C or not
	uint8_t busystate=pI2C_Handle->TxRxState;
	if(busystate != Busy_In_Rx && busystate!= Busy_In_Tx)
	{
		pI2C_Handle->DevAddr=Address;
		pI2C_Handle->RxLen=Length;
		pI2C_Handle->pRxBuffer=pRxBuffer;
		pI2C_Handle->TxRxState=Busy_In_Rx;

		I2C_EnablePeripheral();

		I2C_GenerateStartCond();
		I2C_ClearTWINTFlag();

	}

	return busystate;//if this function in main returned I2C_Ready then it reached LINE 235
}



void I2C_ISRHandler(I2C_Handle_t *pI2C_Handle)
{

	switch (I2C->TWSR & STATUS_MASK)
	{
	case START_ACK:
		I2C_ExcutePhaseAddress(pI2C_Handle);
		I2C_ClearTWINTFlag();
		break;

	case SLAVE_ADDR_AND_WR_ACK:

		I2C_HandleTXEMaster(pI2C_Handle);
		I2C_ClearTWINTFlag();
		break;

	case MSTR_WR_BYTE_ACK:

		I2C_HandleTXEMaster(pI2C_Handle);
		I2C_ClearTWINTFlag();
		break;

	case REP_START_ACK:
		I2C_ExcutePhaseAddress(pI2C_Handle);
		I2C_ClearTWINTFlag();
		break;


	case SLAVE_ADDR_AND_RD_ACK:
		I2C_HandleRXEMaster(pI2C_Handle);
		I2C_ClearTWINTFlag();
		break;

	case MSTR_RD_BYTE_WITH_ACK:
		I2C_HandleRXEMaster(pI2C_Handle);
		I2C_ClearTWINTFlag();
		break;


	case MSTR_RD_BYTE_WITH_NACK:
		I2C_ClearTWINTFlag();
		break;

	case SLAVE_ADDR_RCVD_RD_REQ:
		I2C_ClearTWINTFlag();
		break;

	case SLAVE_ADDR_RCVD_WR_REQ:
		I2C_ClearTWINTFlag();
		break;

	case SLAVE_DATA_RECEIVED:
		I2C_ClearTWINTFlag();
		break;

	case SLAVE_BYTE_TRANSMITTED:
		I2C_ClearTWINTFlag();
		break;

	default :
		while(1);
		break;

	}
}

static void I2C_ExcutePhaseAddress(I2C_Handle_t *pI2C_Handle)
{
	if(pI2C_Handle->TxRxState==Busy_In_Rx)
	{
		I2C->TWDR =(pI2C_Handle->DevAddr<<1) |(0x1);
	}
	else if(pI2C_Handle->TxRxState==Busy_In_Tx)
	{
		I2C->TWDR =(pI2C_Handle->DevAddr<<1)& ~(0x1);
	}

}

static void I2C_HandleTXEMaster(I2C_Handle_t *pI2C_Handle)
{
	if(pI2C_Handle->TxLen>0)
	{

	I2C->TWDR = *(pI2C_Handle->pTxBuffer);
	pI2C_Handle->TxLen--;
	pI2C_Handle->pTxBuffer++;

	}
	else if(pI2C_Handle->TxLen==0)
	{
		if(pI2C_Handle->Sr==0)
		{
		I2C_GenerateStopCond();
		}
		pI2C_Handle->TxRxState=I2C_Ready;
	}

}

static void I2C_HandleRXEMaster(I2C_Handle_t *pI2C_Handle)
{
	if(pI2C_Handle->RxLen >0 )
	{
		*(pI2C_Handle->pRxBuffer) = I2C->TWDR;
		pI2C_Handle->pRxBuffer++;
		pI2C_Handle->RxLen--;
	}
	else if(pI2C_Handle->RxLen==0)
	{
		I2C_GenerateStopCond();
		pI2C_Handle->TxRxState=I2C_Ready;

	}

}

void I2C_GenerateStartCond(void)
{
	TWCR |= (1<<I2C_TWCR_TWSTA);
}

static void I2C_GenerateStopCond(void)
{
	TWCR |= (1<<I2C_TWCR_TWSTO);
}

void I2C_EnablePeripheral(void)
{
	TWCR |= (1<<I2C_TWCR_TWEN);
}

static void I2C_DisablePeripheral(void)
{
	TWCR &= ~(1<<I2C_TWCR_TWEN);
}

static void I2C_ClearStartCond(void)
{
	TWCR &= ~(1<<I2C_TWCR_TWSTA);
}

static void I2C_ClearTWINTFlag(void)
{
	TWCR |= (1<<I2C_TWCR_TWINT);
}

static void I2C_DisablingACK(void)
{
	TWCR &=~ (1<<I2C_TWCR_TWEA);
}
static void I2C_EnableACK(void)
{
	TWCR |= (1<<I2C_TWCR_TWEA);
}
