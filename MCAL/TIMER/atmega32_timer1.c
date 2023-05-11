/*
 * atmega32_timer1.c
 *
 *  Created on: Apr 7, 2023
 *      Author: moham
 */


#include "atmega32_timer1.h"

void (*GlobalCallBack)(void)=NULL;


void TIMER1_CaptureInterrupt(void(*pCallBack)(void))
{
	EnableGlobalInterrupt();
	TIMERInterrupt->TIMSK |= (1<<TIMER_TIMSK_TICIE1);//Enable the interrupt for Input Capture Unit

	GlobalCallBack=pCallBack; //Set the application address to be excuted here
}
void TIMER1_EdgeDetectInput(uint8_t Edge)
{
	//Edge given by the user
	if(Edge==InputCapture_Falling)
	{
		TIMER1->TCCR1B &= ~(1<<TIMER1_TCCR1B_ICES1); // Enable capture falling edge
	}
	else if (Edge==InputCapture_Rising)
	{
		TIMER1->TCCR1B |= (1<<TIMER1_TCCR1B_ICES1); // Enable capture rising edge
	}
}

void TIMER1_RESET(void)
{
	TIMER1->ICR1=0;
	TIMER1->OCR1A=0;
	TIMER1->OCR1B=0;
	TIMER1->TCCR1A=0;
	TIMER1->TCCR1B=0;
	TIMER1->TCNT1=0;
}
void TIMER1_Init(TIMER1_Config_t *pTIMER1_Config)
{
	if(pTIMER1_Config->Mode==TIMER1_Mode_Normal)
	{
		TIMER1->TCCR1A &= ~(3<<TIMER1_TCCR1A_WGM10);
		TIMER1->TCCR1B &= ~(3<<TIMER1_TCCR1B_WGM12);
	}

	else if (pTIMER1_Config->Mode==TIMER1_Mode_CTC)
	{
		TIMER1->TCCR1A &= ~(3<<TIMER1_TCCR1A_WGM10);
		TIMER1->TCCR1B |= (1<<TIMER1_TCCR1B_WGM12);

		if(pTIMER1_Config->CompareMode==TIMER1_NonPWM_Normal)
		{
			TIMER1->TCCR1A &= ~(3<<TIMER1_TCCR1A_COM1A0);
			TIMER1->TCCR1A &= ~(3<<TIMER1_TCCR1A_COM1B0);
		}
		else if (pTIMER1_Config->CompareMode==TIMER1_NonPWM_Toggle)
		{
			TIMER1->TCCR1A |= (1<<TIMER1_TCCR1A_COM1A0);
		}
		else if(pTIMER1_Config->CompareMode==TIMER1_NonPWM_SetClear)
		{
			//LATER
		}
	}

	else if (pTIMER1_Config->Mode==TIMER1_Mode_FastPWM_ICR)
	{
		//Configuring the mode in two lines
		TIMER1->TCCR1A &= ~(1<<TIMER1_TCCR1A_WGM10);
		TIMER1->TCCR1A |= (1<<TIMER1_TCCR1A_WGM11);
		TIMER1->TCCR1B |= (3<<TIMER1_TCCR1B_WGM12);

		//Choosing the channel that will produce the PWM
		if(pTIMER1_Config->Channel==Channel_A)
		{
			TIMER1->TCCR1A |= (TIMER1_FastPWM_NonInverted<<TIMER1_TCCR1A_COM1A0);
		}
		else if(pTIMER1_Config->Channel==Channel_B)
		{
			//Later
		}
		else if(pTIMER1_Config->Channel==Channel_A_B)
		{
			if(pTIMER1_Config->FastPWM_Mode==TIMER1_FastPWM_Inverted)
			{
				TIMER1->TCCR1A |= (TIMER1_FastPWM_NonInverted<<TIMER1_TCCR1A_COM1A0);
				TIMER1->TCCR1A |= (TIMER1_FastPWM_NonInverted<<TIMER1_TCCR1A_COM1B0);
			}
		}

		TIMER1->ICR1=pTIMER1_Config->InputCaptureValue;
	}

	//setting the PRESCALER
	TIMER1->TCCR1B |= (pTIMER1_Config->Prescaler<<TIMER1_TCCR1B_CS10);
}


void TIMER1_SendPWM(float Freq,uint16_t DutyCycle,uint8_t channel)
{


	// must be in FASTPWM_ICR
	float temp;

	 temp=((float)(1/(float)Freq))*1000000;

	TIMER1->ICR1 =temp;
	if(channel == TIMER1_CHANNELA)
	{
	TIMER1->OCR1A=((float)((float)DutyCycle/100))*temp;//Channel A
	}
	else if (channel == TIMER1_CHANNELB)
	{
	TIMER1->OCR1B=((float)((float)DutyCycle/100))*temp;//Channel B
	}
}

void TIMER1_ServoPWM(uint8_t angle)
{

	//ICR1 must be already configured as 20000 as the SERVO motor wont work if its not
	TIMER1->OCR1A= 1000 +((float)angle)*(float)(1000/180); // Range is from 1000 to 2000 for the regular servo
	TIMER1->OCR1B= 1000 +((float)angle)*(float)(1000/180);

}

uint8_t GetFlagStat(uint8_t BitName)
{
	if( ((TIMERInterrupt->TIFR) >>BitName) & 0x1)
	{
		return SET;
	}
	else {
		return RESET;
	}
}

void TIMER1_DelaySync_ms(uint32_t time)
{
	uint32_t temp=0;

	//Delay will be used in CTC Mode
	temp=time*1000;

	TIMER1->OCR1A=temp/128;

	TIMER1->TCCR1B |= (5<<TIMER1_TCCR1B_CS10);

	while(! GetFlagStat(TIMER_TIFR_OCF1A));

	TIMERInterrupt->TIFR |= (1<<TIMER_TIFR_OCF1A);
}

void TIMER1_DelayAsync_ms(uint32_t time , void (*pApplication)(void))
{
	uint32_t temp=0;
	//Delay will be used in CTC Mode

	temp=time*1000;
	TIMER1->OCR1A=temp/128;

	GlobalCallBack=pApplication;


	EnableGlobalInterrupt();
	TIMERInterrupt->TIMSK |= (1<<TIMER_TIMSK_OCIE1A);

	TIMER1->TCCR1B |= (TIMER1_Prescaler1024<<TIMER1_TCCR1B_CS10);

}
// ISR TIMER1 CTC Interrupt
void __vector_7(void) __attribute__((signal));
void __vector_7(void)
{
	GlobalCallBack();
}

//ISR for the INPUT CAPTURE Interrupt
void __vector_6(void) __attribute__((signal));
void __vector_6(void)
{
	GlobalCallBack();
}
