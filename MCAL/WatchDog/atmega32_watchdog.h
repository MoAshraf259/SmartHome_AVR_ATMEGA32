/*
 * atmega32_watchdog.h
 *
 *  Created on: Apr 9, 2023
 *      Author: moham
 */

#ifndef MCAL_WATCHDOG_ATMEGA32_WATCHDOG_H_
#define MCAL_WATCHDOG_ATMEGA32_WATCHDOG_H_

#include "atmega32.h"

void Watchdog_Enable(void);
void Watchdog_Disable(void);
void Watchdog_SetSleepTime(uint8_t time);

#endif /* MCAL_WATCHDOG_ATMEGA32_WATCHDOG_H_ */
