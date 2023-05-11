/*
 * FinalSmartHome.c
 *
 *  Created on: May 8, 2023
 *      Author: MohmedAshraf
 */


#include <stdio.h>
#include <string.h>
#include "atmega32.h"
#include "LCD.h"
#include "EEPROM.h"

#define CLOSE_DOOR		1
#define OPEN_DOOR		2

#define ADMIN_MODE		1
#define USER_MODE		2

#define Lamp1	BIT_NUM_1
#define Lamp2	BIT_NUM_2
#define Lamp3	BIT_NUM_3

uint8_t MODE=1;
typedef struct
{
	uint8_t name_length;
	uint8_t pass_length;
	uint8_t name_index;
	uint8_t pass_index;
}User_Handle_t;
uint8_t NumberOfUsers=0;
uint16_t EEPROM_PC=0x01;
uint8_t EEPROM_PG=EEPROM_PAGE1;
uint8_t usernameEEPROM[10];
uint8_t usernameUART[7];
uint8_t passwordUART[5];
User_Handle_t user[10];


uint8_t userPnumber=0;
uint8_t usernumber=0;
void TIMER1_InterruptApp(void);
void USART_RXApp(void);
void USART_TXApp(void);
static uint8_t Handle_USERNAME(uint8_t UserNumber);
static uint8_t Handle_PASS(uint8_t UserNumber);
static void Add_USERNAME(uint8_t *Username,uint8_t length);
void Add_PASSWORD(uint8_t *password,uint8_t length);
void TIMER0_App(void);
void TakePass(void);
void BuzzerWelcomeMusic(void);
void HandleDoor(uint8_t data);
void HandleHomeFacilties(void);
void Add_NewUser(void );
void TakeInfo(uint8_t userNumber);
void ControlLights(uint8_t number);
void ControlAC(void);
DIO_Handle_t DIO1_Handle;

void DISABLE_TIMER1(void)
{
	TIMER1->TCCR1B &= ~(0x7);
}

TIMER0_Config_t TIMER0_Conf;

void TIMER0_MAININIT(void)
{
	TIMER0_Conf.CompareMatchValue=75;
	TIMER0_Conf.CompareMode_NonPWM=TIMER0_CTC_OC0Dis;
	TIMER0_Conf.InterruptMode=TIMER0_Interrupt_CTC;
	TIMER0_Conf.Timer_Prescaler=TIMER0_Prescaler256;
	TIMER0_Conf.Waveform=TIMER0_Waveform_CTC;
	TIMER0_Conf.mode=TIMER0_Mode_Interrupt;

	TIMER0_Init(&TIMER0_App, &TIMER0_Conf);
}

I2C_Handle_t I2C_Handle;
void I2C_MainInit(void)
{
	I2C_Handle.pI2C_Config.I2C_Mode=I2C_Mode_Sync;
	I2C_Handle.pI2C_Config.I2C_Ack=I2C_Ack_Enable;
	I2C_Handle.pI2C_Config.GenralCall_Status=I2C_GeneralCall_Enable;
	I2C_Handle.pI2C_Config.I2C_Prescaler=I2C_SCL_SPEED200K;
	I2C_Handle.pI2C_Config.I2C_Address=0x68;

	I2C_Init(&I2C_Handle);
}
void DIO_MainInit(void)
{
	DIO1_Handle.pDIOx=DIOC;
	DIO1_Handle.DIO_Config.MODE=OUTPUT;
	DIO1_Handle.DIO_Config.PinNumber=BIT_NUM_0;
	DIO_Init(&DIO1_Handle);

	DIO1_Handle.DIO_Config.PinNumber=BIT_NUM_2;
	DIO_Init(&DIO1_Handle);

	DIO1_Handle.pDIOx=DIOB;
	DIO1_Handle.DIO_Config.MODE=OUTPUT;
	DIO1_Handle.DIO_Config.PinNumber=BIT_NUM_0;
	DIO_Init(&DIO1_Handle);

	DIO1_Handle.DIO_Config.PinNumber=BIT_NUM_1;
	DIO_Init(&DIO1_Handle);

	DIO1_Handle.DIO_Config.PinNumber=BIT_NUM_2;
	DIO_Init(&DIO1_Handle);

	DIO1_Handle.DIO_Config.PinNumber=BIT_NUM_3;
	DIO_Init(&DIO1_Handle);

	DIO1_Handle.pDIOx=DIOD;
	DIO1_Handle.DIO_Config.PinNumber=BIT_NUM_5;
	DIO_Init(&DIO1_Handle);
}

USART_Config_t USART1_Config;
void USART_MainInitIT(void)
{
	USART1_Config.SyncAsync=USART_Async;
	USART1_Config.CharSize=USART_CharSize_8;
	USART1_Config.Polarity=USART_Polarity_Falling;
	USART1_Config.StopBits=USART_StopBits_1;
	USART1_Config.mode=USART_Mode_RxTx;
	USART1_Config.parity=USART_Parity_Off;
	USART1_Config.BaudRate=USART_Baudrate_9600;

	USART_Init(&USART1_Config);
}

void USART_MainInitSync(void)
{
	USART1_Config.SyncAsync=USART_Sync;
	USART1_Config.CharSize=USART_CharSize_8;
	USART1_Config.Polarity=USART_Polarity_Falling;
	USART1_Config.StopBits=USART_StopBits_1;
	USART1_Config.mode=USART_Mode_RxTx;
	USART1_Config.parity=USART_Parity_Off;
	USART1_Config.BaudRate=USART_Baudrate_9600;

	USART_Init(&USART1_Config);
}

ADC_Config_t ADC1_Config;

void ADC1_Init(void)
{
	ADC1_Config.PowerSource=ADC_Power_AVCC;
	ADC1_Config.ReadFirst=ADC_FirstRead_ADCH;
	ADC1_Config.mode=ADC_Mode_Polling;
	ADC1_Config.prescaler=ADC_Prescaler_128;
	ADC1_Config.Resolution=ADC_Res_10BIT;
	ADC_Init(&ADC1_Config);
}

TIMER1_Config_t TIMER1PWM_Config;
void TIMER1_PWMMainConfig(void)
{
	TIMER1PWM_Config.Mode=TIMER1_Mode_FastPWM_ICR;
	TIMER1PWM_Config.FastPWM_Mode=TIMER1_FastPWM_NonInverted;
	TIMER1PWM_Config.Channel=Channel_A;
	TIMER1PWM_Config.Prescaler=TIMER1_Prescaler8;
	TIMER1PWM_Config.InputCaptureValue=20000;
	TIMER1_ServoPWM(0);
	TIMER1_Init(&TIMER1PWM_Config);
}


TIMER1_Config_t TIMER1Delay_Config;

void TIMER1_DelayMainConfig(void)
{
	TIMER1Delay_Config.Mode=TIMER1_Mode_CTC;
	TIMER1Delay_Config.CompareMode=TIMER1_NonPWM_Normal;
	TIMER1Delay_Config.Prescaler=TIMER1_Prescaler1024;
	TIMER1_Init(&TIMER1Delay_Config);

}

uint8_t takeid[]="Enter your username :\r\n";
uint8_t takepass[]="Enter your pass\r\n";
uint8_t WELCOME[]="Welcome HOME\r\n";
uint8_t WRONG_ID[]="Wrong ID !\r\n";
uint8_t WRONG_PASS[]="Wrong PASS !\r\n";
uint8_t NoUsers[]="No Users Yet\r\n";
uint8_t EnterNewUSER[]="EnterNewUsername_6digits\r\n";
uint8_t EnterNewPASS[]="EnterNewPassword_4digits\r\n";
uint8_t System_Lock[]="System Locked !\r\n";
uint8_t NotOPTION[]="Not an Option\r\n";
uint8_t UserNumber[]="EnterUserNumber:\r\n";
uint8_t freshstartup[]="Hello Choose your mode 1-ADMIN 2-USER\r\n";
uint8_t notstartup[]="Hello Choose your mode 1-ADMIN 2-USER 3-Cancel\r\n";
uint8_t OrdersListUser[]="1-CloseDoor 2-OpenDoor 3-TurnLights1 4- TurnLights2 5-TurnLights3 6-ChangeUser\r\n";
uint8_t OrdersListAdmin[]="1-AddUser 2-ChangeUser\r\n";
uint8_t RxMsg=0;
uint8_t TempNotification[]="Current TEMP:";
uint8_t Admin[]="Ashraf";
uint8_t AdminPass[]="1234";
uint8_t BB[10];
uint8_t AdminOnly[]="Can`t add users Admin Only Mode\r\n";
uint8_t locked[]="System LOCKED \r\nPress K to Reset System\r\n";
uint8_t fresh=0;
int main(void)
{


	LCD_Init();
	DIO_MainInit();
	ADC1_Init();
	TIMER1_PWMMainConfig();
	USART_MainInitSync();
	I2C_MainInit();


	Add_USERNAME(Admin, sizeof(Admin)-1);
	Add_PASSWORD(AdminPass,sizeof(AdminPass)-1);


	TakePass();
	HandleDoor(OPEN_DOOR);
	BuzzerWelcomeMusic();

	fresh++;

	while(1)
	{
		HandleHomeFacilties();
	}

}


static uint8_t Handle_USERNAME(uint8_t UserNumber)
{
	uint8_t state;
	uint8_t counter=0;
	 memset(usernameEEPROM, '\0', sizeof(usernameEEPROM));
	 memset(usernameUART, '\0', sizeof(usernameUART));

	USART_RecieveData(&USART1_Config,usernameUART,6);

	EEPROM_ReceiveFromData(usernameEEPROM, user[UserNumber].name_index, user[UserNumber].name_length+1);



	for(uint8_t i=0;i<user[UserNumber].name_length;i++)
	{

		if(usernameUART[i]==usernameEEPROM[i])
		{
			counter++;
		}
	}
	if(counter==6)
	{
		state=1;
	}
	else{
		state =0;
	}
	return state;
}

static uint8_t Handle_PASS(uint8_t UserNumber)
{
	uint8_t state;
	uint8_t counter=0;

	 memset(usernameEEPROM, '\0', sizeof(usernameEEPROM));
	 memset(usernameUART, '\0', sizeof(usernameUART));

	USART_RecieveData(&USART1_Config,usernameUART,4);


	EEPROM_ReceiveFromData(usernameEEPROM, user[UserNumber].pass_index, user[UserNumber].pass_length+1);

	for(uint8_t i=0;i<user[UserNumber].pass_length;i++)
	{

		if(usernameUART[i]==usernameEEPROM[i])
		{
			counter++;
		}
	}
	if(counter==4)
	{
		state=1;
	}
	else{
		state =0;
	}
	return state;

}
static void Add_USERNAME(uint8_t *Username,uint8_t length)
{

	user[usernumber].name_index=EEPROM_PC;

	user[usernumber].name_length=length;

	uint8_t data=0;
	while(length>0)
	{
		data=*Username;
		EEPROM_SendToData(data, EEPROM_PC);
		EEPROM_PC++;
		Username++;
		length--;
	}

	usernumber++;
}

void Add_PASSWORD(uint8_t *password,uint8_t length)
{

	user[userPnumber].pass_index=EEPROM_PC;

	user[userPnumber].pass_length=length;

	uint8_t data=0;

	while(length>0)
	{
		data=*password;
		EEPROM_SendToData(data, EEPROM_PC);
		EEPROM_PC++;
		password++;
		length--;
	}
	userPnumber++;

}


void TakePass(void)
{
	uint8_t RxBuffer;
	uint8_t state;
	uint8_t Wcounter1=0,Wcounter2=0;
	if(fresh==0){
	USART_SendData(&USART1_Config, freshstartup, sizeof(freshstartup));
	}
	else{
		USART_SendData(&USART1_Config, notstartup, sizeof(notstartup));
	}
	USART_RecieveData(&USART1_Config, &RxBuffer, 1);
	if(RxBuffer=='1')
	{

		while(Wcounter1<3){
			USART_SendData(&USART1_Config, takeid, sizeof(takeid));
			state=Handle_USERNAME(0);
			if(state==1){
				break;
			}
			else{
				Wcounter1++;
				USART_SendData(&USART1_Config, WRONG_ID, sizeof(WRONG_ID));
			}
		}

	if(Wcounter1==3){
		uint8_t res=0;
		LCD_SendString(System_Lock);
		USART_SendData(&USART1_Config, locked, sizeof(locked));
		USART_RecieveData(&USART1_Config, &res, 1);
		if(res=='K')
		{
			Watchdog_Enable();
		}
		else{
		while(1);
		}
		}

		while(Wcounter2<3){
			USART_SendData(&USART1_Config, takepass, sizeof(takepass));
			state=Handle_PASS(0);
			if(state==1){
				USART_SendData(&USART1_Config, WELCOME, sizeof(WELCOME));
				break;
			}
			else{
				Wcounter2++;
				USART_SendData(&USART1_Config, WRONG_PASS, sizeof(WRONG_PASS));
			}
		}
		if(Wcounter2==3)
		{
			uint8_t res;
			LCD_SendString(System_Lock);
			USART_SendData(&USART1_Config, locked, sizeof(locked));
			USART_RecieveData(&USART1_Config, &res, 1);
			if(res=='K')
			{
				Watchdog_Enable();
			}
			else{
			while(1);
			}
		}
		MODE=ADMIN_MODE;
	}

	else if(RxBuffer=='2'){

		if(NumberOfUsers==0){
			USART_SendData(&USART1_Config, NoUsers, sizeof(NoUsers));
			TakePass();
		}

		else{
			uint8_t userNumber;
			USART_SendData(&USART1_Config,UserNumber , sizeof(UserNumber));
			USART_RecieveData(&USART1_Config, &userNumber, 1);

			TakeInfo(userNumber-48);
		}
		MODE=USER_MODE;
	}
	else if(RxBuffer=='3')
	{
		if(fresh==0)
		{
			TakePass();
		}
		else{
		HandleHomeFacilties();
		}
	}
	else{
		TakePass();
		}
}

void BuzzerWelcomeMusic(void){
	DIO_WriteToPin(DIOB, BIT_NUM_0, HIGH);
	_delay_us(700);
	DIO_WriteToPin(DIOB, BIT_NUM_0, LOW);
	DIO_WriteToPin(DIOB, BIT_NUM_0, HIGH);
	_delay_us(730);
	DIO_WriteToPin(DIOB, BIT_NUM_0, LOW);
	DIO_WriteToPin(DIOB, BIT_NUM_0, HIGH);
	_delay_us(714);
	DIO_WriteToPin(DIOB, BIT_NUM_0, LOW);
	DIO_WriteToPin(DIOB, BIT_NUM_0, HIGH);
	_delay_us(650);
	DIO_WriteToPin(DIOB, BIT_NUM_0, LOW);

}

void HandleDoor(uint8_t data){

	if(data==OPEN_DOOR)
	{
		TIMER1_ServoPWM(180);
	}
	else if(data==CLOSE_DOOR)
	{
		TIMER1_ServoPWM(0);
	}
}

void HandleHomeFacilties(void)
{
	uint8_t order;
	ControlAC();
	if(MODE==ADMIN_MODE){
		USART_SendData(&USART1_Config, OrdersListAdmin, sizeof(OrdersListAdmin));
		USART_RecieveData(&USART1_Config, &order, 1);
		switch(order){
		case '1':
			Add_NewUser();
			break;

		case '2':
			TakePass();
			break;

		default :
			USART_SendData(&USART1_Config, NotOPTION, sizeof(NotOPTION));
			break;
		}
	}

	else if(MODE==USER_MODE){
		USART_SendData(&USART1_Config, OrdersListUser, sizeof(OrdersListUser));
		USART_RecieveData(&USART1_Config, &order, 1);
	switch(order){
	case '1':
		HandleDoor(CLOSE_DOOR);
		break;

	case '2':
		HandleDoor(OPEN_DOOR);
		break;

	case '3':
		ControlLights(Lamp1);
		break;

	case '4':
		ControlLights(Lamp2);
		break;

	case '5':
		ControlLights(Lamp3);
		break;

	case '6':
		TakePass();
		break;

	default :
		USART_SendData(&USART1_Config, NotOPTION, sizeof(NotOPTION));
		break;
	}
	}

}

void TIMER0_App(void)
{

}
void TakeInfo(uint8_t userNumber)
{

	uint8_t state;
	uint8_t Wcounter1=0,Wcounter2=0;
	while(Wcounter1<3){
		USART_SendData(&USART1_Config, takeid, sizeof(takeid));
		state=Handle_USERNAME(userNumber);
		if(state==1){
			break;
		}
		else{
			Wcounter1++;
			USART_SendData(&USART1_Config, WRONG_ID, sizeof(WRONG_ID));
		}
	}

	if(Wcounter2==3)
	{
		uint8_t res;
		LCD_SendString(System_Lock);
		USART_SendData(&USART1_Config, locked, sizeof(locked));
		USART_RecieveData(&USART1_Config, &res, 1);
		if(res=='K')
		{
			Watchdog_Enable();
		}
		else{
		while(1);
		}
	}

	while(Wcounter2<3){
		USART_SendData(&USART1_Config, takepass, sizeof(takepass));
		state=Handle_PASS(userNumber);
		if(state==1){
			USART_SendData(&USART1_Config, WELCOME, sizeof(WELCOME));
			break;
		}
		else{
			Wcounter2++;
			USART_SendData(&USART1_Config, WRONG_PASS, sizeof(WRONG_PASS));
		}
	}
	if(Wcounter2==3)
	{
		uint8_t res;
		LCD_SendString(System_Lock);
		USART_SendData(&USART1_Config, locked, sizeof(locked));
		USART_RecieveData(&USART1_Config, &res, 1);
		if(res=='K')
		{
			Watchdog_Enable();
		}
		else{
		while(1);
		}
	}
}
void Add_NewUser(void)
{
	memset(usernameEEPROM, '\0', sizeof(passwordUART));
	memset(usernameUART, '\0', sizeof(usernameUART));

	USART_SendData(&USART1_Config, EnterNewUSER, sizeof(EnterNewUSER));
	USART_RecieveData(&USART1_Config, usernameUART, 6);

	Add_USERNAME(usernameUART, 6);


	USART_SendData(&USART1_Config, EnterNewPASS, sizeof(EnterNewPASS));
	USART_RecieveData(&USART1_Config, passwordUART, 4);

	Add_PASSWORD(passwordUART, 4);
	NumberOfUsers++;
}

void ControlLights(uint8_t number)
{
	DIO_TogglePin(DIOB, number);
}

void ControlAC(void)
{
	uint32_t Temperature=0;
	ADC_StartConversionSync(ADC_ChannelNumber0);
	Temperature=ADC_Read(&ADC1_Config);
	Temperature=((Temperature*500)/1024);
	lcd_set_cursor(1, 1);
	LCD_SendString(TempNotification);
	lcd_set_cursor(1, 14);
	LCD_SendNumber(Temperature);
	if(Temperature>37)
	{
		DIO_WriteToPin(DIOC, 2, HIGH);
	}
	else
	{
		DIO_WriteToPin(DIOC, 2, LOW);
	}
}
