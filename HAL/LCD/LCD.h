/*
 * LCD.h
 *
 *  Created on: Mar 28, 2023
 *      Author: Mohamed Ashraf
 */

#ifndef LCD_LCD_H_
#define LCD_LCD_H_

#include "atmega32.h"



#define Control_Port	DIOC
#define LCD_DIO_PORT 	DIOA

#define LCD_PIN_0	BIT_NUM_0
#define LCD_PIN_1	BIT_NUM_1
#define LCD_PIN_2	BIT_NUM_2
#define LCD_PIN_3	BIT_NUM_3

#define LCD_PIN_4	BIT_NUM_4 //Used for the 4 bits mode !
#define LCD_PIN_5	BIT_NUM_5
#define LCD_PIN_6	BIT_NUM_6
#define LCD_PIN_7	BIT_NUM_7

#define LCD_RS	BIT_NUM_7
#define LCD_RW	BIT_NUM_6
#define LCD_EN	BIT_NUM_5

#define _4BIT_MODE		0
#define _8BIT_MODE		1
#define LCD_MODE 	0


#define LCD_CMD_4DL_2N_5X8F  		0x28
#define LCD_CMD_DON_CURON    		0x0E
#define LCD_CMD_INCADD       		0x06
#define LCD_CMD_DIS_CLEAR    		0X01
#define LCD_CMD_DIS_RETURN_HOME  	0x02





void LCD_Init(void);

void LCD_SendCMD(uint8_t cmd);

void LCD_SendChar(uint8_t character);

void LCD_SendString(uint8_t *pTxBuffer);

void LCD_SendSpecialCharacter(uint8_t *pPattern,uint8_t location,uint8_t row,uint8_t col);

void clr_lcd(void);

void write_4_bits(uint8_t value);

void lcd_set_cursor(uint8_t row, uint8_t column);
void LCD_SendNumber(uint16_t number);


#endif /* HAL_LCD_LCD_H_ */
