/*
 * atmega32_usart.c
 *
 *  Created on: Apr 9, 2023
 *      Author: moham
 */

#include "atmega32_usart.h"
#include "LCD.h"
void (*USARTTxGlobalCallBack)(void)=NULL;
void (*USARTRxGlobalCallBack)(void)=NULL;
void (*USARTUDRCallBack)(void)=NULL;

void USART_Init(USART_Config_t *pUSARTConfig)
{
	uint8_t temp_reg=0;

	if( pUSARTConfig->SyncAsync == USART_Sync)
	{
		temp_reg &= ~(1<<USART_UCSRB_RXCIE);
		temp_reg &= ~(1<<USART_UCSRB_TXCIE);
	}
	else if(pUSARTConfig->SyncAsync == USART_Async)
	{
		EnableGlobalInterrupt();
		//Enable the interrupt for each of the USART Operations
		temp_reg |= (1<<USART_UCSRB_RXCIE);
		temp_reg |= (1<<USART_UCSRB_TXCIE);
		//temp_reg |= (1<<USART_UCSRB_UDRIE);
	}

	//Mode Configuration
	if(pUSARTConfig->mode==USART_Mode_RxTx)
	{
		temp_reg |= (1<<USART_UCSRB_RXEN);
		temp_reg |= (1<<USART_UCSRB_TXEN);


	}
	else if(pUSARTConfig->mode==USART_Mode_RxOnly)
	{
		USART->UCSRB |=  (1<<USART_UCSRB_RXEN);
		USART->UCSRB &= ~(1<<USART_UCSRB_TXEN);
	}
	else if(pUSARTConfig->mode==USART_Mode_TxOnly)
	{
		USART->UCSRB |=  (1<<USART_UCSRB_TXEN);
		USART->UCSRB &= ~(1<<USART_UCSRB_RXEN);
	}

	USART->UCSRB =temp_reg;

	temp_reg=0;
	// This bit must be set to one before writing to UCSRC Otherwise data will be written to UBRRH
	temp_reg |= (1<<USART_UCSRC_URSEL);

	//Parity Configuration
	temp_reg |= (pUSARTConfig->parity<<USART_UCSRC_UPM0);

	//Number of stop bits Configuration
	temp_reg|= (pUSARTConfig->StopBits << USART_UCSRC_USBS);

	//Data register length configuration
	temp_reg|= (pUSARTConfig->CharSize<<USART_UCSRC_UCSZ0);

	//Polarity Configuration
	temp_reg|= (pUSARTConfig->Polarity<<USART_UCSRC_UCPOL);




	UCSRC = temp_reg;


	USART->UBRRL |= pUSARTConfig->BaudRate;
}

uint8_t USART_GetFlagStat(uint8_t flagname)
{
	if( (USART->UCSRA >> flagname) & (0x1))
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

void USART_SendData(USART_Config_t *pUSARTConfig, uint8_t *pTxBuffer,uint8_t Length)
{
	//this pointer used for the 9 bit data length as the buffer is 8bits
	//This will be needed for the extra bit
	uint16_t *pdata;
	//Wait until the Transmit buffer is empty
	while ( ! USART_GetFlagStat(USART_UCSRA_UDRE));

	while(Length !=0)
	{
		//Wait until the Transmit buffer is empty
		while ( ! USART_GetFlagStat(USART_UCSRA_UDRE));
		//USART->UCSRA |=(1<<USART_UCSRA_UDRE);


		//Check if the data length is more than 8 bits
		if(pUSARTConfig->CharSize == USART_CharSize_9)
		{
			pdata= (uint16_t*)pTxBuffer;
			uint16_t temp=*pdata;
			USART->UCSRB |= ((temp)>>8) & 0x1;
			USART->UDR |= *pdata &0xFF ;

			if(pUSARTConfig->parity==USART_Parity_Off)
			{
				pTxBuffer++;
				pTxBuffer++;
			}

		}
		else
		{
			//Here the data is less or equal to 8 bits so we don`t need to +2 the buffer even if there is parity
			USART->UDR = *pTxBuffer;
			pTxBuffer++;
		}

		while( ! USART_GetFlagStat(USART_UCSRA_TXC));
		USART->UCSRA |= (1<<USART_UCSRA_TXC);
		Length--;

	}

}

void USART_RecieveData(USART_Config_t *pUSARTConfig, uint8_t *pRxBuffer , uint8_t Length)
{
	while (Length !=0)
	{
		if(pUSARTConfig->CharSize==USART_CharSize_9)
		{

			//later

		}
		else
		{
			while( ! USART_GetFlagStat(USART_UCSRA_RXC) );
			*pRxBuffer =USART->UDR;
			pRxBuffer++;
		}
		Length--;
	}
}

void USART_SendDataIT(uint8_t *pTxBuffer,   uint8_t Length,     void (*CallBack)(void))
{
	USARTTxGlobalCallBack=CallBack;

	while(Length !=0)
	{
		while(!USART_GetFlagStat(USART_UCSRA_UDRE));
		USART->UDR= *pTxBuffer;
		Length--;
		pTxBuffer++;
		//extern uint8_t counter;
		//counter++;
	}

}
void USART_RecieveDataIT(void (*CallBack)(void))
{
	USARTRxGlobalCallBack=CallBack;
}


//USART IRQ HANDLER For RXC Receive complete
void __vector_13(void) __attribute__((signal));
void __vector_13(void)
{
	USARTRxGlobalCallBack();
}

//USART IRQ Handler for UDRE Data register empty
void __vector_14(void) __attribute__((signal));
void __vector_14(void)
{
	USARTUDRCallBack();
}

//USART IRQ Handler for TXC Sending complete
void __vector_15(void) __attribute__((signal));
void __vector_15(void)
{
	USARTTxGlobalCallBack();
}
