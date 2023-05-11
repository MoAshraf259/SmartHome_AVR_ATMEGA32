/*
 * atmega32_timer.c
 *
 *  Created on: Apr 5, 2023
 *      Author: moham
 */

#include "atmega32_timer0.h"

	uint64_t Counter=0;
void (*pTIMER0_GlobalApp)(void)=NULL;

void TIMER0_ClearPrescaler(void)
{
	TIMER0->TCCR0 &= (0b11111000);// Clearing the first 3 bits (Prescaler bits)
}



void TIMER0_Init(void (*pFUNC_ISR)(void),TIMER0_Config_t *pTIMER0_Config)
{
	if(pTIMER0_Config->Waveform==TIMER0_Waveform_CTC)
	{
		//code to enable CTC mode
		TIMER0->TCCR0 |=  (1<<TIMER0_TCCR0_WGM01);  //CTC Mode enabled for timer0
		TIMER0->TCCR0 &= ~(1<<TIMER0_TCCR0_WGM00);

		TIMER0->TCCR0 |=(pTIMER0_Config->CompareMode_NonPWM<<TIMER0_TCCR0_COM00);
	}

	else if(pTIMER0_Config->Waveform==TIMER0_Waveform_FastPWM)
	{
		TIMER0->TCCR0 |=  (1<<TIMER0_TCCR0_WGM01);  //Fast PWM Mode enabled for timer0
		TIMER0->TCCR0 |=  (1<<TIMER0_TCCR0_WGM00);

		TIMER0->TCCR0 |= (pTIMER0_Config->CompareMode_FastPWM<<TIMER0_TCCR0_COM00);
	}

	else if(pTIMER0_Config->Waveform==TIMER0_Waveform_Normal)
	{
		//by default but just to make sure
		TIMER0->TCCR0 &= ~(1<<TIMER0_TCCR0_WGM01);  //Normal Mode enabled for timer0
		TIMER0->TCCR0 &= ~(1<<TIMER0_TCCR0_WGM00);
	}
	else if(pTIMER0_Config->Waveform==TIMER0_Waveform_PhaseCorrect)
	{
		TIMER0->TCCR0 |=  (1<<TIMER0_TCCR0_WGM00);  //PhaseCorrect Mode enabled for timer0
		TIMER0->TCCR0 &= ~(1<<TIMER0_TCCR0_WGM01);

		TIMER0->TCCR0 |= (pTIMER0_Config->CompareMode_PhaseCorrect<<TIMER0_TCCR0_COM00);
	}




	TIMER0_OCR0 = pTIMER0_Config->CompareMatchValue;//Setting the compare match  will cause an interrupt on OCIE0


	if(pTIMER0_Config->mode==TIMER0_Mode_Interrupt)
	{
		EnableGlobalInterrupt();
		pTIMER0_GlobalApp=pFUNC_ISR;
		TIMERInterrupt->TIMSK |= (pTIMER0_Config->InterruptMode<<TIMER_TIMSK_TOIE0);
	}

	TIMER0_ClearPrescaler();
	TIMER0->TCCR0 |=(pTIMER0_Config->Timer_Prescaler<<TIMER0_TCCR0_CS00);

}


void TIMER0_Delay_ms(uint32_t time)
{

}

void __vector_10(void) __attribute__((signal));
void __vector_10(void)
{
	if(pTIMER0_GlobalApp != NULL)
	{
		pTIMER0_GlobalApp();
	}

}

void __vector_11(void) __attribute__((signal));
void __vector_11(void)
{
	if(pTIMER0_GlobalApp != NULL)
	{
		pTIMER0_GlobalApp();
	}

}
