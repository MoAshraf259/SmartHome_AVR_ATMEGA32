/*
 * atmega32_ext_int.c
 *
 *  Created on: Mar 22, 2023
 *      Author: m7mda
 */

#include "atmega32_ext_int.h"

void(*EXTI0_MainApplication)(void)=NULL;
void(*EXTI1_MainApplication)(void)=NULL;
void(*EXTI2_MainApplication)(void)=NULL;
/************************This part is for RUNTIME Functions for external interrupts ************/
void EXTI_Init(EXTI_Config_t * pEXTI_Config)
{
	EnableGlobalInterrupt();

	if(pEXTI_Config->EXTI_Number==EXTI0)
	{
		GICR |= (1<<GICR_INT0);//Enabling the bit that is responsible for enabling External Interrupt 0

		MCUCR |=((pEXTI_Config->InteruptMode)<<0);//0 indicates the first two bits to control the interrupt type
	}

	else if(pEXTI_Config->EXTI_Number==EXTI1)
	{
		GICR |= (1<<GICR_INT1);
		MCUCR |=((pEXTI_Config->InteruptMode)<<2);//2 indicates the second two bits to control the interrupt type
	}

	else if(pEXTI_Config->EXTI_Number==EXTI2)
	{
		GICR |= (1<<GICR_INT2);
		if (pEXTI_Config->InteruptMode==EXTI_RISING_EDGE)		// Enabling the rising edge on the INT2
		{
			MCUCSR |= (1<<MCUCSR_ISC2);
		}
		else if(pEXTI_Config->InteruptMode==EXTI_FALLING_EDGE) // Enabling the FALLING edge on the INT2
		{
			MCUCSR &= ~(1<<MCUCSR_ISC2);
		}
	}
}

void EXTI_Disable(EXTI_Config_t * pEXTI_Config)
{
	if(pEXTI_Config->EXTI_Number==EXTI0)
	{
		GICR &= ~(1<<GICR_INT0);//Disabling the bit that is responsible for enabling External Interrupt 0
	}
	else if(pEXTI_Config->EXTI_Number==EXTI1)
	{
		GICR &= ~(1<<GICR_INT1);//Disabling the bit that is responsible for enabling External Interrupt 1
	}
	else if(pEXTI_Config->EXTI_Number==EXTI2)
	{
		GICR &= ~(1<<GICR_INT2);//Disabling the bit that is responsible for enabling External Interrupt 2
	}
}

/*******************Handling the ISR From the application given from the user *********/


void EXTI0_ApplicationEventCallBack(void(*ptr)(void))
{
	EXTI0_MainApplication=ptr;
}

void EXTI1_ApplicationEventCallBack(void(*ptr)(void))
{
	EXTI1_MainApplication=ptr;
}

void EXTI2_ApplicationEventCallBack(void(*ptr)(void))
{
	EXTI2_MainApplication=ptr;
}


//These are the ISRs created by the programmer to handle the ISR without the user seeing it in main file
void __vector_1(void) __attribute__((signal));
void __vector_1(void)
{
	if(EXTI0_MainApplication!=NULL)
	{
		EXTI0_MainApplication();
	}
}

void __vector_2(void) __attribute__((signal));
void __vector_2(void)
{

	if(EXTI1_MainApplication!=NULL)
	{
		EXTI1_MainApplication();
	}

}

void __vector_3(void) __attribute__((signal));
void __vector_3(void)
{
	if(EXTI2_MainApplication!=NULL)
	{
		EXTI2_MainApplication();
	}
}





/********This part is for PREBUILD External Interrupt ****************/

void EXTI_PreDisable(void)
{
#if EXTI_Num == EXTI0
	GICR &= ~(1<<GICR_INT0);

#elif EXTI_Num ==EXTI1
	GICR &= ~(1<<GICR_INT1);

#elif EXTI_Num ==EXTI1
	GICR &= ~(1<<GICR_INT2);

#endif
}
void EXTI_PreInit(void)
{
	EnableGlobalInterrupt();


#if EXTI_Num == EXTI0
	GICR |= (1<<GICR_INT0);


#if EXTI_InterruptMode == EXTI_LOW_LEVEL

	MCUCR |=((EXTI_LOW_LEVEL)<<0);

#elif EXTI_InterruptMode == EXTI_ANY_CHANGE
	MCUCR |=((EXTI_ANY_CHANGE)<<0);
#elif EXTI_InterruptMode == EXTI_FALLING_EDGE
	MCUCR |=((EXTI_FALLING_EDGE)<<0);
#elif EXTI_InterruptMode == EXTI_RISING_EDGE
	MCUCR |=((EXTI_RISING_EDGE)<<0);

#endif
#endif
#if EXTI_Num == EXTI1
	GICR |= (1<<GICR_INT1);


#if EXTI_InterruptMode == EXTI_LOW_LEVEL

	MCUCR |=((EXTI_LOW_LEVEL)<<0);

#elif EXTI_InterruptMode == EXTI_ANY_CHANGE
	MCUCR |=((EXTI_ANY_CHANGE)<<0);
#elif EXTI_InterruptMode == EXTI_FALLING_EDGE
	MCUCR |=((EXTI_FALLING_EDGE)<<0);
#elif EXTI_InterruptMode == EXTI_RISING_EDGE
	MCUCR |=((EXTI_RISING_EDGE)<<0);

#endif
#endif

#if EXTI_Num == EXTI2
	GICR |= (1<<GICR_INT2);


#if EXTI_InterruptMode == EXTI_RISING_EDGE
	MCUCSR |= (1<<MCUCSR_ISC2);
#elif EXTI_InterruptMode == EXTI_FALLING_EDGE
	MCUCSR &= ~(1<<MCUCSR_ISC2);
#endif
#endif

}

