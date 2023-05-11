/*
 * atmega32_timer1.h
 *
 *  Created on: Apr 7, 2023
 *      Author: moham
 */

#ifndef MCAL_TIMER_ATMEGA32_TIMER1_H_
#define MCAL_TIMER_ATMEGA32_TIMER1_H_

#include "atmega32.h"


#define TIMER1_Mode_Normal				0
#define TIMER1_Mode_PhaseCorrect8		1
#define TIMER1_Mode_PhaseCorrect9		2
#define TIMER1_Mode_PhaseCorrect10		3
#define TIMER1_Mode_CTC					4
#define TIMER1_Mode_FastPWM8			5
#define TIMER1_Mode_FastPWM9			6
#define TIMER1_Mode_FastPWM10			7
#define TIMER1_Mode_FastPWM_ICR			8

#define TIMER1_Prescaler1				1
#define TIMER1_Prescaler8				2
#define TIMER1_Prescaler64				3
#define TIMER1_Prescaler256				4
#define TIMER1_Prescaler1024			5

#define TIMER1_NonPWM_Normal			0
#define	TIMER1_NonPWM_Toggle			1
#define TIMER1_NonPWM_ClearSet			2
#define TIMER1_NonPWM_SetClear			3

#define Channel_A			0
#define Channel_B			1
#define Channel_A_B			2

#define InputCapture_Falling		0
#define InputCapture_Rising			1

#define  TIMER1_FastPWM_NonInverted		2
#define  TIMER1_FastPWM_Inverted		3
#define  TIMER1_FastPWM_NoPins			0

#define TIMER1_CHANNELA			0
#define TIMER1_CHANNELB			1

typedef struct{
	uint16_t InputCaptureValue;
	uint16_t CompareValue;
	uint8_t Mode;
	uint8_t Prescaler;
	uint8_t CompareMode;
	uint8_t Channel;
	uint8_t FastPWM_Mode;
}TIMER1_Config_t;

void TIMER1_Init(TIMER1_Config_t *pTIMER1_Config);

void TIMER1_SendPWM(float Freq,uint16_t DutyCycle,uint8_t channel);
void TIMER1_ServoPWM(uint8_t angle);

void USART_ApplicationEventCallBack(void (*pApplication)(void));

void TIMER1_CaptureInterrupt(void(*pCallBack)(void));
void TIMER1_EdgeDetectInput(uint8_t Edge);

void TIMER1_DelayAsync_ms(uint32_t time,void (*pApplication)(void));
void TIMER1_DelaySync_ms(uint32_t time);

void TIMER1_RESET(void);
#endif /* MCAL_TIMER_ATMEGA32_TIMER1_H_ */
