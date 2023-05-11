/*
 * atmega32_DIO.h
 *
 *  Created on: Mar 21, 2023
 *      Author:Mohamed Ashraf
 */

#ifndef DRIVERS_INC_ATMEGA32_DIO_H_
#define DRIVERS_INC_ATMEGA32_DIO_H_


#include "atmega32.h"

//Macros for usable modes
#define OUTPUT 	1
#define INPUT	0
#define SET		1
#define RESET 	0
#define HIGH	SET
#define LOW		RESET

// for the types of the RESISTOR ON THE PIN
#define INPUT_PU_RES 	1
#define INPUT_PD_RES	0


//Configurable values in the DIOs
typedef struct DIO_Config{

	uint8_t PinNumber;
	uint8_t MODE;
	uint8_t PUPD;

}DIO_Config_t;

//Structure to handle everything on the DIO
typedef struct {

	DIO_RegDef_t *pDIOx;
	DIO_Config_t DIO_Config;

}DIO_Handle_t;



void DIO_Init(DIO_Handle_t *pDIO_Handle);
void DIO_WriteToPin(DIO_RegDef_t *pDIOx , uint8_t PinNumber , uint8_t HighOrLow);

void DIO_TogglePin(DIO_RegDef_t *pDIOx , uint8_t PinNumber);

void DIO_InitPort(DIO_Handle_t *pDIO_Handle);
void DIO_WriteToPort(DIO_RegDef_t *pDIOx,uint8_t value);

uint8_t DIO_ReadFromPin(  DIO_RegDef_t *pDIOx  ,  uint8_t PinNumber );

uint8_t DIO_ReadFromPort( DIO_RegDef_t *pDIOx);

#endif /* DRIVERS_INC_ATMEGA32_DIO_H_ */
