/*
 * atmega32_ADC.h
 *
 *  Created on: Apr 4, 2023
 *      Author: Mohamed Ashraf
 */

#ifndef MCUL_DRIVERS_INC_ATMEGA32_ADC_H_
#define MCUL_DRIVERS_INC_ATMEGA32_ADC_H_

#include "atmega32.h"

#define ADC_Prescaler_2			1
#define ADC_Prescaler_4			2
#define ADC_Prescaler_8			3
#define ADC_Prescaler_16		4
#define ADC_Prescaler_32		5
#define ADC_Prescaler_64		6
#define ADC_Prescaler_128		7


#define ADC_ChannelNumber0		0
#define ADC_ChannelNumber1		1
#define ADC_ChannelNumber2		2
#define ADC_ChannelNumber3		3
#define ADC_ChannelNumber4		4
#define ADC_ChannelNumber5		5
#define ADC_ChannelNumber6		6
#define ADC_ChannelNumber7		7

#define ADC_Res_8BIT			0
#define ADC_Res_10BIT			1

#define ADC_Mode_Interrupt		0
#define ADC_Mode_Polling		1

#define ADC_Power_AREF			0
#define ADC_Power_AVCC			1
#define ADC_Power_Internal		3

#define ADC_FirstRead_ADCH		1
#define ADC_FirstRead_ADCL		0

typedef struct {
	uint8_t mode;
	uint8_t prescaler;
	uint8_t ReadFirst;
	uint8_t PowerSource;
	uint8_t Resolution;
}ADC_Config_t;

void ADC_Init(ADC_Config_t *pADC_Config);
void ADC_StartConversionAsync(uint8_t ChannelNumber);
void ADC_StartConversionSync(uint8_t ChannelNumber);

uint16_t ADC_Read(ADC_Config_t *pADC_Config);
void ADC_ApplicaionEventCallBack(uint16_t(*ptr)(void));

#endif /* MCUL_DRIVERS_INC_ATMEGA32_ADC_H_ */
