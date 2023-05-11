/*
 * atmega32.h
 *
 *  Created on: Mar 21, 2023
 *      Author: Mohamed Ashraf
 */

#ifndef DRIVERS_INC_ATMEGA32_H_
#define DRIVERS_INC_ATMEGA32_H_

//#include <stdint.h>
#include <util/delay.h>

#define NULL	0


#define __vo  volatile

//base address of each PORT
#define PORTD_BASEADDR			(0x30U)
#define PORTC_BASEADDR			(0x33U)
#define PORTB_BASEADDR			(0x36U)
#define PORTA_BASEADDR			(0x39U)

//Base address of the SPI
#define SPI_BASEADDR			(0x2DU)
//Base address of the ADC
#define ADC_BASEADDR			(0x24U)

#define ADC_FullDataRegister		(*((__vo uint16_t*)0x24))

#define I2C_BASEADDR			(0x20U)

//These addresses are used for the external interrupt
#define SREG      (*((__vo uint8_t*)0x5F))
#define GICR      (*((__vo uint8_t*)0x5B))
#define GIFR      (*((__vo uint8_t*)0x5A))
#define MCUCR     (*((__vo uint8_t*)0x55))
#define MCUCSR    (*((__vo uint8_t*)0x54))

#define WDTCR	  (*((__vo uint8_t*)0x41))
//base address of the structure that carries the interrupt registers of the TIMERs
#define TIMERInterrupt_BASEADDR		(0x58U)

//TIMERs Base addresses
#define TIMER0_BASEADDR				(0x52U)
#define TIMER1_BASEADDR				(0x46U)
#define TIMER2_BASEADDR				(0x42U)

//Function like macro to control the GLOBAL Interrupt
#define EnableGlobalInterrupt();	SREG |= (1<<SREG_GLOBAL_INTERUPT);
#define DisableGlobalInterrupt();	SREG &= ~(1<<SREG_GLOBAL_INTERUPT);


//USART Base addresses
#define USART_BASEADDR				(0x29U)

#define UCSRC			(*((__vo uint8_t*)0x40))
#define UBRRH			(*((__vo uint8_t*)0x40))

//#define UDR			(*((__vo uint8_t*)0x2C))
//As all the registers we need is contiguous we but in a structure
//this structure is for controlling the DIOs
typedef struct
{
	__vo uint8_t PINx;
	__vo uint8_t DDRx;
	__vo uint8_t PORTx;

}DIO_RegDef_t;

//As all the registers we need is contiguous we but in a structure
//this structure is for controlling the SPI Protocol
typedef struct {
	__vo uint8_t SPCR;
	__vo uint8_t SPSR;
	__vo uint8_t SPDR;
}SPI_RegDef_t;


//All the registers used for the ADC Driver
typedef struct {
	__vo uint8_t ADCL;
	__vo uint8_t ADCH;
	__vo uint8_t ADCSRA;
	__vo uint8_t ADMUX;
}ADC_RegDef_t;

//This structure carries the registers responsible for the interrupt of the TIMERs
typedef struct{
	__vo uint8_t TIFR;
	__vo uint8_t TIMSK;
}TIMERInterrupt_RegDef_t;

typedef struct{
	__vo uint8_t TCNT0;
	__vo uint8_t TCCR0;
}TIMER0_RegDef_t;
#define TIMER0_OCR0		(*((__vo uint8_t*)0x5C))  //Defined alone because of the arrangement of the addresses



typedef struct{
	__vo uint16_t ICR1;
	__vo uint16_t OCR1B;
	__vo uint16_t OCR1A;
	__vo uint16_t TCNT1;
	__vo uint8_t TCCR1B;
	__vo uint8_t TCCR1A;
}TIMER1_RegDef_t;


typedef struct{
	__vo uint8_t ASSR;
	__vo uint8_t OCR2;
	__vo uint8_t TCNT2;
	__vo uint8_t TCCR2;
}TIMER2_RegDef_t;
/***********************************************/


//**************************USART Register Definition ***********************/
typedef struct
{
	__vo uint8_t UBRRL;
	__vo uint8_t UCSRB;
	__vo uint8_t UCSRA;
	__vo uint8_t UDR;
}USART_RegDef_t;


typedef struct
{
	__vo uint8_t TWBR;
	__vo uint8_t TWSR;
	__vo uint8_t TWAR;
	__vo uint8_t TWDR;
}I2C_RegDef_t;

#define TWCR		(*((__vo uint8_t*)0x56))

//Use this to access the TIMERs interrupt registers
#define TIMERInterrupt		((TIMERInterrupt_RegDef_t*)TIMERInterrupt_BASEADDR)

//Use this to access the TIMERs registers
#define TIMER0				((TIMER0_RegDef_t*)TIMER0_BASEADDR)
#define TIMER1				((TIMER1_RegDef_t*)TIMER1_BASEADDR)
#define TIMER2				((TIMER2_RegDef_t*)TIMER2_BASEADDR)

//Used by many peripherals
#define SFIOR	(*((__vo uint8_t*)0x50)) // not in the same arrangement so we define it alone

//Use this macro to access the arranged USART Registers
#define USART		((USART_RegDef_t*)USART_BASEADDR)

//Use this macro to access the arranged registers controlling the I2C Peripheral
#define I2C			((I2C_RegDef_t*)I2C_BASEADDR)


/*These are the addresses used by the handle structure
to control the Whole pin modes TO BE MORE GENERIC*/
#define DIOD	((DIO_RegDef_t*)PORTD_BASEADDR)
#define DIOC	((DIO_RegDef_t*)PORTC_BASEADDR)
#define DIOB	((DIO_RegDef_t*)PORTB_BASEADDR)
#define DIOA	((DIO_RegDef_t*)PORTA_BASEADDR)

//The pointer to the structure that carries all the SPI registers we need
#define SPI		((SPI_RegDef_t*)SPI_BASEADDR)

//The pointer to the first register of the ADC Registers structure
#define ADC	((ADC_RegDef_t*)ADC_BASEADDR)


//Bit numbers macros to use
#define BIT_NUM_0	0
#define BIT_NUM_1	1
#define BIT_NUM_2	2
#define BIT_NUM_3	3
#define BIT_NUM_4	4
#define BIT_NUM_5	5
#define BIT_NUM_6	6
#define BIT_NUM_7	7

// all the bits used in the SPI Control Register macros
#define SPI_SPCR_SPR0		0
#define SPI_SPCR_SPR1		1
#define SPI_SPCR_CPHA		2
#define SPI_SPCR_CPOL		3
#define SPI_SPCR_MSTR		4
#define SPI_SPCR_DORD		5
#define SPI_SPCR_SPE		6
#define SPI_SPCR_SPIE		7

//all the bits used in the SPI Status register macros
#define SPI_SPSR_SPI2X		0
#define SPI_SPSR_WCOL		6
#define SPI_SPSR_SPIF		7

//The bit number of the global interrupt enable in the SREG Register
#define SREG_GLOBAL_INTERUPT	7
#define Bit_Copy_Storage		6

//Bit number on the GIC Register to determine  which external interrupt is enabled
#define GICR_INT2		5
#define GICR_INT0		6
#define GICR_INT1		7

#define MCUCSR_ISC2		6


//The bit number of each and every bit used for the ADC Configuration
#define ADC_SFIOR_ADTS0		5
#define ADC_SFIOR_ADTS1		6
#define ADC_SFIOR_ADTS2		7

#define ADC_ADCSRA_ADPS0	0
#define ADC_ADCSRA_ADPS1	1
#define ADC_ADCSRA_ADPS2	2
#define ADC_ADCSRA_ADIE		3
#define ADC_ADCSRA_ADIF		4
#define ADC_ADCSRA_ADATE	5
#define ADC_ADCSRA_ADSC		6
#define ADC_ADCSRA_ADEN		7

#define ADC_ADMUX_MUX0		0
#define ADC_ADMUX_MUX1		1
#define ADC_ADMUX_MUX2		2
#define ADC_ADMUX_MUX3		3
#define ADC_ADMUX_MUX4		4
#define ADC_ADMUX_ADLAR		5
#define ADC_ADMUX_REFS0		6
#define ADC_ADMUX_REFS1		7

/********************MACROS For the bit numbers of the Registers used in TIMERs*******/

#define TIMER0_TCCR0_CS00		0
#define TIMER0_TCCR0_CS01		1
#define TIMER0_TCCR0_CS02		2
#define TIMER0_TCCR0_WGM01		3
#define TIMER0_TCCR0_COM00		4
#define TIMER0_TCCR0_COM01		5
#define TIMER0_TCCR0_WGM00		6
#define TIMER0_TCCR0_FOC0		7

#define TIMER_TIMSK_TOIE0		0
#define TIMER_TIMSK_OCIE0		1
#define TIMER_TIMSK_OCIE1A		4
#define TIMER_TIMSK_TICIE1		5

#define TIMER_TIFR_TOV0			0
#define TIMER_TIFR_OCF0			1
#define TIMER_TIFR_TOV1			2
#define TIMER_TIFR_OCF1B		3
#define TIMER_TIFR_OCF1A		4
#define TIMER_TIFR_ICF1			5
#define TIMER_TIFR_TOV0			0
#define TIMER_TIFR_OCF0			1

//TIMER 1 !!!

#define TIMER1_TCCR1A_WGM10		0
#define TIMER1_TCCR1A_WGM11		1
#define TIMER1_TCCR1A_FOC1B		2
#define TIMER1_TCCR1A_FOC1A		3
#define TIMER1_TCCR1A_COM1B0	4
#define TIMER1_TCCR1A_COM1B1	5
#define TIMER1_TCCR1A_COM1A0	6
#define TIMER1_TCCR1A_COM1A1	7

#define TIMER1_TCCR1B_CS10		0
#define TIMER1_TCCR1B_CS11		1
#define TIMER1_TCCR1B_CS12		2
#define TIMER1_TCCR1B_WGM12		3
#define TIMER1_TCCR1B_WGM13		4
#define TIMER1_TCCR1B_ICES1		6
#define TIMER1_TCCR1B_ICNC1		7


//TIMER2 !!!!

#define TIMER2_ASSR_TCR2UB		0
#define TIMER2_ASSR_OCR2UB		1
#define TIMER2_ASSR_TCN2UB		2
#define TIMER2_ASSR_AS2			3

#define TIMER2_TCCR2_CS20		0
#define TIMER2_TCCR2_CS21		1
#define TIMER2_TCCR2_CS22		2
#define TIMER2_TCCR2_WGM21		3
#define TIMER2_TCCR2_COM20		4
#define TIMER2_TCCR2_COM21		5
#define TIMER2_TCCR2_WGM20		6
#define TIMER2_TCCR2_FOC2		7


//WATCHDOG timer BITS
#define WDTCR_WDE		3
#define WDTCR_WDTOE		4

//USART Registers BITS
#define USART_UCSRB_TXB8		0
#define USART_UCSRB_RXB8		1
#define USART_UCSRB_UCSZ2		2
#define USART_UCSRB_TXEN		3
#define USART_UCSRB_RXEN		4
#define USART_UCSRB_UDRIE		5
#define USART_UCSRB_TXCIE		6
#define USART_UCSRB_RXCIE		7

#define USART_UCSRC_UCPOL		0
#define USART_UCSRC_UCSZ0		1
#define USART_UCSRC_UCSZ1		2
#define USART_UCSRC_USBS		3
#define USART_UCSRC_UPM0		4
#define USART_UCSRC_UPM1		5
#define USART_UCSRC_UMSEL		6
#define USART_UCSRC_URSEL		7

#define USART_UCSRA_MPCM		0
#define USART_UCSRA_U2X			1
#define USART_UCSRA_PE			2
#define USART_UCSRA_DOR			3
#define USART_UCSRA_FE			4
#define USART_UCSRA_UDRE		5
#define USART_UCSRA_TXC			6
#define USART_UCSRA_RXC			7

//Macros for the I2C Register bits
#define I2C_TWCR_TWIE			0
#define I2C_TWCR_TWEN			2
#define I2C_TWCR_TWWC			3
#define I2C_TWCR_TWSTO			4
#define I2C_TWCR_TWSTA			5
#define I2C_TWCR_TWEA			6
#define I2C_TWCR_TWINT			7

#define I2C_TWSR_TWPS0			0
#define I2C_TWSR_TWPS1			1
#define I2C_TWSR_TWS3			3
#define I2C_TWSR_TWS4			4
#define I2C_TWSR_TWS5			5
#define I2C_TWSR_TWS6			6
#define I2C_TWSR_TWS7			7

#define I2C_TWAR_TWGCE 	0
#include "atmega32_DIO.h"
#include "atmega32_ext_int.h"
#include "atmega32_spi.h"
#include "atmega32_ADC.h"
#include "atmega32_timer0.h"
#include "atmega32_timer1.h"
#include "atmega32_watchdog.h"
#include "atmega32_usart.h"
#include "atmega32_i2c.h"

#endif /* DRIVERS_INC_ATMEGA32_H_ */
