/*
 * atmega32_usart.h
 *
 *  Created on: Apr 9, 2023
 *      Author: moham
 */

#ifndef MCAL_USART_ATMEGA32_USART_H_
#define MCAL_USART_ATMEGA32_USART_H_
#include "atmega32.h"


#define USART_Sync			0
#define USART_Async			1

#define USART_Mode_RxTx			0
#define USART_Mode_RxOnly		1
#define USART_Mode_TxOnly		2

#define USART_Parity_EvenOn			2
#define USART_Parity_OddOn			3
#define USART_Parity_Off			0

#define USART_StopBits_1			0
#define USART_StopBits_2			1

#define USART_CharSize_5			0
#define USART_CharSize_6			1
#define USART_CharSize_7			2
#define USART_CharSize_8			3
#define USART_CharSize_9			7

#define USART_Polarity_Rising		0
#define USART_Polarity_Falling		1

#define USART_Baudrate_2400		207
#define USART_Baudrate_4800		103
#define USART_Baudrate_9600  	51
#define USART_Baudrate_14400 	34
#define USART_Baudrate_19200	34
#define USART_Baudrate_28800	16


typedef struct{

	uint8_t SyncAsync;
	uint8_t mode;
	uint8_t parity;
	uint8_t StopBits;
	uint8_t CharSize;
	uint8_t Polarity;
	uint8_t BaudRate;

}USART_Config_t;


void USART_Init(USART_Config_t *pUSARTConfig);

void USART_SendData(USART_Config_t *pUSARTConfig,uint8_t *pTxBuffer,uint8_t Length);
void USART_RecieveData(USART_Config_t *pUSARTConfig  , uint8_t *pRxBuffer  ,  uint8_t Length);




void USART_SendDataIT    (uint8_t *pTxBuffer,   uint8_t Length,    void (*CallBack)(void));
void USART_RecieveDataIT(void (*CallBack)(void));




#endif /* MCAL_USART_ATMEGA32_USART_H_ */
