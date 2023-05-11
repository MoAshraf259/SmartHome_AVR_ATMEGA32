/*
 * LCD.c
 *
 *  Created on: Mar 28, 2023
 *      Author: Mohamed Ashraf
 */
#include "LCD.h"

void LCD_Init(void)
{
	DIO_Handle_t DIO_LCD;
	DIO_LCD.pDIOx=LCD_DIO_PORT;
	DIO_LCD.DIO_Config.MODE=OUTPUT;
	DIO_InitPort(&DIO_LCD);


	DIO_LCD.pDIOx=Control_Port; //The controlling bits PORT

	DIO_LCD.DIO_Config.PinNumber=BIT_NUM_7;//RSS PIN
	DIO_Init(&DIO_LCD);

	DIO_LCD.DIO_Config.PinNumber=BIT_NUM_6;//RW pin
	DIO_Init(&DIO_LCD);

	DIO_LCD.DIO_Config.PinNumber=BIT_NUM_5;//Enable PIN
	DIO_Init(&DIO_LCD);


	_delay_ms(30);

#if LCD_MODE ==_8BIT_MODE
{
	LCD_SendCMD(0x38); //Function set
	_delay_ms(100);

	LCD_SendCMD(0x0E); //Display on or OFF
	_delay_ms(100);

	LCD_SendCMD(0x01);//Clear the LCD
	_delay_ms(100);

}
#elif (LCD_MODE ==_4BIT_MODE)
{
	LCD_SendCMD(0x3);

	_delay_ms(30);





	LCD_SendCMD(0x3);

	_delay_ms(30);
	LCD_SendCMD(0x3);

	_delay_ms(30);
	LCD_SendCMD(0x2);

	_delay_us(50);
	LCD_SendCMD(LCD_CMD_4DL_2N_5X8F);

	_delay_us(50);
	LCD_SendCMD(LCD_CMD_DON_CURON);

	_delay_us(50);
	LCD_SendCMD(LCD_CMD_DIS_CLEAR);

	_delay_us(50);
	LCD_SendCMD(LCD_CMD_INCADD);
}
#endif
}

void LCD_SendCMD(uint8_t cmd)
{

#if LCD_MODE==_8BIT_MODE
	DIO_WriteToPin(DIOC,LCD_RS,LOW);
	DIO_WriteToPin(DIOC,LCD_RW,LOW);

	DIO_WriteToPort(LCD_DIO_PORT,cmd);

	DIO_WriteToPin(DIOC,LCD_EN,HIGH);
	_delay_ms(100);
	DIO_WriteToPin(DIOC,LCD_EN,LOW);

#elif LCD_MODE== _4BIT_MODE

	DIO_WriteToPin(Control_Port,LCD_RS,LOW);
	DIO_WriteToPin(Control_Port,LCD_RW,LOW);

	write_4_bits(cmd>>4);

	DIO_WriteToPin(Control_Port,LCD_EN,HIGH);
	_delay_us(100);
	DIO_WriteToPin(Control_Port,LCD_EN,LOW);

	write_4_bits(cmd & 0x0F);
	_delay_us(100);
	DIO_WriteToPin(Control_Port,LCD_EN,HIGH);

			_delay_us(100);
		DIO_WriteToPin(Control_Port,LCD_EN,LOW);



#endif
}

void LCD_SendChar(uint8_t character)
{
#if(LCD_MODE==_8BIT_MODE)
	DIO_WriteToPin(Control_Port,LCD_RS,HIGH);
	DIO_WriteToPin(Control_Port,LCD_RW,LOW);

	DIO_WriteToPort(LCD_DIO_PORT,character);

	DIO_WriteToPin(Control_Port,LCD_EN,HIGH);
	_delay_ms(100);
	DIO_WriteToPin(Control_Port,LCD_EN,LOW);

#elif(mode ==_4BIT_MODE)

		DIO_WriteToPin(DIOC,LCD_RS,HIGH);
		DIO_WriteToPin(DIOC,LCD_RW,LOW);

		write_4_bits(character>>4);

		DIO_WriteToPin(DIOC,LCD_EN,HIGH);
		_delay_us(50);
		DIO_WriteToPin(DIOC,LCD_EN,LOW);
		_delay_us(50);



		write_4_bits(character & 0x0F);

		DIO_WriteToPin(DIOC,LCD_EN,HIGH);;
		_delay_us(50);
		DIO_WriteToPin(DIOC,LCD_EN,LOW);
		_delay_us(50);

#endif
}



void LCD_SendString(uint8_t *pTxBuffer)
{
	while(*pTxBuffer != '\0')
	{
		LCD_SendChar(*(pTxBuffer));

		pTxBuffer++;
	}
}

void LCD_SendSpecialCharacter(uint8_t *pPattern,uint8_t location,uint8_t row,uint8_t col)
{
	uint8_t i;
	    if(location<8)
	    {
	     LCD_SendCMD(0x40 + (location*8));  /* Command 0x40 and onwards forces
	                                      the device to point CGRAM address */
	    	//LCD_SendCMD(64);
	       for(i=0;i<8;i++)
	       {// Write 8 byte for generation of 1 character
	           LCD_SendChar(pPattern[i]);
	       }
	    }
	    lcd_set_cursor(row,col);
	    LCD_SendChar(location);
}

void clr_lcd(void)
{
#if LCD_MODE == _8BIT_MODE
	LCD_SendCMD(0x1);
#elif LCD_MODE == _4BIT_MODE
	LCD_SendCMD(LCD_CMD_DIS_CLEAR);
#endif
}

void write_4_bits(uint8_t value)
{
	DIO_WriteToPin(LCD_DIO_PORT, LCD_PIN_4, (value>>0)& 0x1);
	DIO_WriteToPin(LCD_DIO_PORT, LCD_PIN_5, (value>>1)& 0x1);
	DIO_WriteToPin(LCD_DIO_PORT, LCD_PIN_6, (value>>2)& 0x1);
	DIO_WriteToPin(LCD_DIO_PORT, LCD_PIN_7, (value>>3)& 0x1);
}

void lcd_set_cursor(uint8_t row, uint8_t column)
{
  column--;
  switch (row)
  {
    case 1:
      /* Set cursor to 1st row address and add index*/
      LCD_SendCMD((column |= 0x80));
      break;
    case 2:
      /* Set cursor to 2nd row address and add index*/
    	LCD_SendCMD((column |= 0xC0));
      break;
    default:
      break;
  }
}

void LCD_SendNumber(uint16_t number)
{
	uint8_t temp[8];
	uint8_t counter=0;
	uint8_t i;
	for(i=0;i<8;i++)
	{
		temp[i]=number%10;
		number=number/10;
		if (number == 0){
		break;
		}
		counter++;
	}

	for(i=counter;i>=0;i--)
	{
		if(i==0)
		{
			switch(temp[i])
			{
			case 1:
				LCD_SendChar('1');
				break;
			case 2:
				LCD_SendChar('2');
				break;
			case 3:
				LCD_SendChar('3');
				break;
			case 4:
				LCD_SendChar('4');
				break;
			case 5:
				LCD_SendChar('5');
				break;
			case 6:
				LCD_SendChar('6');
				break;
			case 7:
				LCD_SendChar('7');
				break;
			case 8:
				LCD_SendChar('8');
				break;
			case 9:
				LCD_SendChar('9');
				break;
			case 0:
				LCD_SendChar('0');
				break;

			default:
				break;
			}
			break;
		}
		else{
		switch(temp[i])
		{
		case 1:
			LCD_SendChar('1');
			break;
		case 2:
			LCD_SendChar('2');
			break;
		case 3:
			LCD_SendChar('3');
			break;
		case 4:
			LCD_SendChar('4');
			break;
		case 5:
			LCD_SendChar('5');
			break;
		case 6:
			LCD_SendChar('6');
			break;
		case 7:
			LCD_SendChar('7');
			break;
		case 8:
			LCD_SendChar('8');
			break;
		case 9:
			LCD_SendChar('9');
			break;
		case 0:
			LCD_SendChar('0');
			break;

		default:
			break;
		}
		}


	}

}
