/*
 * atmega32_timer.h
 *
 *  Created on: Apr 5, 2023
 *      Author: moham
 */

#ifndef MCAL_TIMER_ATMEGA32_TIMER0_H_
#define MCAL_TIMER_ATMEGA32_TIMER0_H_

#include "atmega32.h"

#define TIMER0_Prescaler0			1
#define TIMER0_Prescaler8			2
#define TIMER0_Prescaler64			3
#define TIMER0_Prescaler256			4
#define TIMER0_Prescaler1024		5


#define TIMER0_Waveform_Normal			0
#define TIMER0_Waveform_PhaseCorrect	1
#define TIMER0_Waveform_CTC				2
#define TIMER0_Waveform_FastPWM			3

#define TIMER0_CTC_OC0Dis		0
#define TIMER0_CTC_ToggleOC0	1
#define TIMER0_CTC_ClearOC0		2
#define TIMER0_CTC_SetOC0		3

#define TIMER0_FastPWM_Normal			0
#define TIMER0_FastPWM_NonInverting		2
#define TIMER0_FastPWM_Inverting		3

#define TIMER0_PhaseCorrect_Normal		0
#define TIMER0_PhaseCorrect_SetDown		2
#define TIMER0_PhaseCorrect_SetUp		3

#define TIMER0_Interrupt_OF			1
#define TIMER0_Interrupt_CTC		2
#define TIMER0_Interrupt_CTC_OF		3

#define TIMER0_Mode_Interrupt		1
#define TIMER0_Mode_Polling			0

typedef struct{
	uint8_t mode;
	uint8_t Waveform;
	uint8_t CompareMode_NonPWM;
	uint8_t CompareMode_FastPWM;
	uint8_t CompareMode_PhaseCorrect;
	uint8_t Timer_Prescaler;
	uint8_t CompareMatchValue;
	uint8_t InterruptMode;
}TIMER0_Config_t;


void TIMER0_Init(void (*pCTC_ISR)(void),TIMER0_Config_t *pTIMER0_Config);
void TIMER0_Delay_ms(uint32_t time);

#endif /* MCAL_TIMER_ATMEGA32_TIMER0_H_ */
