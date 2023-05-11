/*
 * atmega32_ext_int.h
 *
 *  Created on: Mar 22, 2023
 *      Author: m7mda
 */

#ifndef DRIVERS_INC_ATMEGA32_EXT_INT_H_
#define DRIVERS_INC_ATMEGA32_EXT_INT_H_

#include "atmega32.h"

//These macros are used to control the interrupt event occur at what event
#define EXTI_LOW_LEVEL		0
#define EXTI_ANY_CHANGE		1
#define EXTI_FALLING_EDGE	2
#define EXTI_RISING_EDGE	3



// choose between the two external interrupts you have
#define EXTI0		0
#define EXTI1		1
#define EXTI2		2

///////*************These macros are used for the prebuild initialization ******////
#define EXTI_Num		EXTI1
#define EXTI_InterruptMode	EXTI_LOW_LEVEL
/***************************************************/
typedef struct {
	uint8_t EXTI_Number;
	uint8_t InteruptMode;
}EXTI_Config_t;




void EXTI_Init(EXTI_Config_t * pEXTI_Config);
void EXTI_Disable(EXTI_Config_t * pEXTI_Config);

void EXTI0_ApplicationEventCallBack(void(*ptr)(void));
void EXTI1_ApplicationEventCallBack(void(*ptr)(void));
void EXTI2_ApplicationEventCallBack(void(*ptr)(void));

void EXTI_PreInit(void);
void EXTI_PreDisable(void);

#endif /* DRIVERS_INC_ATMEGA32_EXT_INT_H_ */
