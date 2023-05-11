/*
 * atmega32_ADC.c
 *
 *  Created on: Apr 4, 2023
 *      Author: Mohamed Ashraf
 */


#include "atmega32_ADC.h"

uint16_t(*pADC_ValueReturn)(void)=NULL;

void ADC_Init(ADC_Config_t *pADC_Config)
{
	if(pADC_Config->mode == ADC_Mode_Interrupt)
	{
		EnableGlobalInterrupt();
		ADC->ADCSRA |= (1<<ADC_ADCSRA_ADIE);//Enable the interrupt
	}
	else if(pADC_Config->mode == ADC_Mode_Polling)
	{
		/*do nothing*/
	}

	ADC->ADCSRA |= (pADC_Config->prescaler<<ADC_ADCSRA_ADPS0);//Set the prescaler value

	ADC->ADMUX |= (pADC_Config->PowerSource << ADC_ADMUX_REFS0);// Choosing the power source


	if(pADC_Config->ReadFirst==ADC_FirstRead_ADCH)
	{
		ADC->ADMUX |= (1<<ADC_ADMUX_ADLAR);//Read the ADCH First
	}
	else if (pADC_Config->ReadFirst==ADC_FirstRead_ADCL)
	{
		ADC->ADMUX &= ~(1<<ADC_ADMUX_ADLAR);//Read the ADCH First
	}

	ADC->ADCSRA |= (1<<ADC_ADCSRA_ADEN);//Enable the ADC

}
void ADMUX_CLEAR(void)
{
	ADC->ADMUX &= ~(0x11111);
}

void ADC_StartConversionSync(uint8_t ChannelNumber)
{
	ADMUX_CLEAR();
	ADC->ADMUX |= (ChannelNumber<<ADC_ADMUX_MUX0);

	ADC->ADCSRA |= (1<<ADC_ADCSRA_ADSC);//start conversion

	while(!((ADC->ADCSRA )>>ADC_ADCSRA_ADIF)&0x1 );//This flag is set when the conversion is complete !
	//Here the user will wait until the conversion is complete
	//and when it`s done the next function will be the read function

	ADC->ADCSRA |= (1<<ADC_ADCSRA_ADIF);
	//You have to clear the flag manually because it`s only cleared when the ISR is excuted
}
void ADC_StartConversionAsync(uint8_t ChannelNumber)
{
	ADMUX_CLEAR();
	ADC->ADMUX |= (ChannelNumber<<ADC_ADMUX_MUX0);
	ADC->ADCSRA |= (1<<ADC_ADCSRA_ADSC);//start conversion
	//After this the user will wait for the Interrupt to handle the ISR Application
	//The processor will let the ADC handle the conversion and when it`s done the ISR Will handle the rest
}

void ADC_ApplicaionEventCallBack(uint16_t(*ptr)(void))
{
	pADC_ValueReturn=ptr;
}


uint16_t ADC_Read(ADC_Config_t *pADC_Config)
{

	uint16_t data=0;

	if(pADC_Config->ReadFirst==ADC_FirstRead_ADCH)
	{
		if(pADC_Config->Resolution==ADC_Res_8BIT)
		{

			data= ADC->ADCH;

		}
		else if(pADC_Config->Resolution==ADC_Res_10BIT)
		{
			data=(ADC_FullDataRegister)>>6;

		}
	}

	else if(pADC_Config->ReadFirst==ADC_FirstRead_ADCL)
	{
		if(pADC_Config->Resolution==ADC_Res_8BIT)
		{
			data= (uint8_t)(ADC_FullDataRegister);

		}
		else if(pADC_Config->Resolution==ADC_Res_10BIT)
		{
			data=(ADC_FullDataRegister);

		}
	}
	return data;
}


void __vector_16(void) __attribute__((signal));
void __vector_16(void)
{
	if(pADC_ValueReturn != NULL)
	{
		pADC_ValueReturn();
	}
}
