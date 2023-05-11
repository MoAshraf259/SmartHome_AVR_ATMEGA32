/*
 * atmega32_DIO.c
 *
 *  Created on: Mar 21, 2023
 *      Author: Mohamed Ashraf
 */



#include "atmega32_DIO.h"

void DIO_Init(DIO_Handle_t *pDIO_Handle)
{

	if(pDIO_Handle->DIO_Config.MODE ==OUTPUT)
	{
		//Enable the DDR Register as an OUTPUT for a specific pin number given by the user
		pDIO_Handle->pDIOx->DDRx |= (1<<pDIO_Handle->DIO_Config.PinNumber);
	}

	else if (pDIO_Handle->DIO_Config.MODE ==INPUT)
	{

		//Enable the DDR Register as an INPUT for a specific pin number given by the user
		pDIO_Handle->pDIOx->DDRx &= ~(1<<pDIO_Handle->DIO_Config.PinNumber);
		//DDRx 0

		//This part is to ENABLE or DISABLE the pull up & pull down resistor
		//On the configuration user has to give on the CONFIG Structure the type of PUPD he needs

		pDIO_Handle->pDIOx->PORTx |= (pDIO_Handle->DIO_Config.PUPD  <<  pDIO_Handle->DIO_Config.PinNumber);

	}

}

void DIO_WriteToPin(DIO_RegDef_t *pDIOx , uint8_t PinNumber , uint8_t HighOrLow)
{
	if(HighOrLow==HIGH)
	{
		//Setting the pin chosen by the user as HIGH
		pDIOx->PORTx |= (1<<PinNumber);
	}

	if(HighOrLow==LOW)
	{
		//Setting the pin chosen by the user as LOW
		pDIOx->PORTx &= ~(1<<PinNumber);
	}
}




uint8_t DIO_ReadFromPin(DIO_RegDef_t *pDIOx ,  uint8_t PinNumber  )
{
	uint8_t value=0;
	value |= (pDIOx->PINx >> PinNumber) & (0x1);
	return value;
}




void DIO_TogglePin(DIO_RegDef_t *pDIOx , uint8_t PinNumber)
{
	pDIOx->PORTx ^= (1<<PinNumber);
}



void DIO_InitPort(DIO_Handle_t *pDIO_Handle)
{
	if(pDIO_Handle->DIO_Config.MODE==OUTPUT)
	{
		pDIO_Handle->pDIOx->DDRx |= 0xFF;
	}
	else if(pDIO_Handle->DIO_Config.MODE==INPUT)
	{
		pDIO_Handle->pDIOx->DDRx = 0;

		if(pDIO_Handle->DIO_Config.PUPD==INPUT_PU_RES)
		{
			pDIO_Handle->pDIOx->PORTx |= 0xFF;
		}
		else if (pDIO_Handle->DIO_Config.PUPD==INPUT_PD_RES)
		{
			pDIO_Handle->pDIOx->PORTx = 0;
		}
	}
}
void DIO_WriteToPort(DIO_RegDef_t *pDIOx,uint8_t value)
{
		pDIOx->PORTx = value;
}

uint8_t DIO_ReadFromPort( DIO_RegDef_t *pDIOx)
{
	uint8_t value=0;
	value = (pDIOx->PINx) & (0x7);
	return value;
}

