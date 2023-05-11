/*
 * atmega32_watchdog.c
 *
 *  Created on: Apr 9, 2023
 *      Author: moham
 */


#include "atmega32_watchdog.h"

void Watchdog_Enable(void)
{
	WDTCR |= (1<<WDTCR_WDE);
}

void Watchdog_Disable(void)
{
	WDTCR =0b00011000;
	WDTCR=0;
}

void Watchdog_SetSleepTime(uint8_t time)
{
	WDTCR |= 6;
}
