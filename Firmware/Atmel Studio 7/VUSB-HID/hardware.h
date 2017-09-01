/*
 * hardware.h
 *
 * Created: 22/12/2016 6:03:25 AM
 *  Author: K. C. Lee
 * Copyright (c) 2016 by K. C. Lee
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.

 If not, see http://www.gnu.org/licenses/gpl-3.0.en.html 
 */ 

#ifndef HARDWARE_H_
#define HARDWARE_H_

#include <avr/interrupt.h>
#include "platform.h"
#include "usbdrv/usbdrv.h"
#include "usbdrv/usbportability.h"
#include <util/delay.h>

#define CONCAT(X,Y)           (X ## Y)
#define REG(X,Y)              CONCAT(Y,X)

#define ENC_CLK_PORT          D
#define ENC_CLK               (1<<PORTD3)
#define ENC_CLK_IRQ           INT1_vect

#define ENC_PORT              C
#define ENC_DT                (1<<PORTC0)
#define ENC_SWITCH            (1<<PORTC1)

#define LED_PORT              C
#define LED_GREEN_N           (1<<PORTC2)
#define LED_AMBER_N           (1<<PORTC3)

#define LED_ON(X)             REG(LED_PORT,PORT) &= ~(X)
#define LED_OFF(X)            REG(LED_PORT,PORT) |= (X)
#define LED_TOGGLE(X)         REG(LED_PORT,PORT) ^= (X)

#define DDRB_INIT             0xff
#define DDRC_INIT             (uint8_t)~(ENC_DT|ENC_SWITCH)
#define DDRD_INIT             (uint8_t)~(ENC_CLK|(1<<USB_DMINUS)|(1<<USB_DPLUS)|0x03)

// Switch status is a shift register that shift left every 16.38ms
#define SW_RISING_EDGE        0x07      // 00000111  <- turned high for 49ms 
#define SW_FALLING_EDGE       0xf8      // 11111000  <- turned low for 49ms 
#define SW_EDGE_MASK          0x0f
#define SW_DEBOUNCE_MASK      0x03

#define SW_AT_MAKE            ((Switch_Status & SW_EDGE_MASK) == (SW_FALLING_EDGE & SW_EDGE_MASK))
#define SW_AT_BREAK           ((Switch_Status & SW_EDGE_MASK) == (SW_RISING_EDGE & SW_EDGE_MASK))
#define SW_MAKE               ((Switch_Status & SW_DEBOUNCE_MASK) == (SW_FALLING_EDGE & SW_DEBOUNCE_MASK))
#define SW_BREAK              ((Switch_Status & SW_DEBOUNCE_MASK) == (SW_RISING_EDGE & SW_DEBOUNCE_MASK))

#define ms_TO_TICKS(X)				((X)/16)

#define TIMER_DBL_BREAK       ms_TO_TICKS(250)
#define TIMER_CLICK_MAKE		  ms_TO_TICKS(150)
#define TIMER_LONG            ms_TO_TICKS(400)

enum SW_STATE
{
  SW_NONE, SW_PRESS, SW_DBL_BREAK, SW_DBL_CLICK, SW_HOLD, SW_BREAK_WAIT,
 };
  
#define REPORT_SIZE		    2

enum REPORT_ID
{
  ID_CONSUMER = 0x01
 };

enum ENC_STATES
{
  ENC_VOLUME, ENC_PLAYCTRL
 };

#define ENC_LAST  ENC_PLAYCTRL
#define ENC_FIRST ENC_VOLUME 

enum REPORT_BITMAP
{
  Cmd_Next_Track =0x01, Cmd_Prev_Track = 0x02, Cmd_Stop = 0x04, Cmd_Play_Pause = 0x08, 
  Cmd_Mute = 0x10, Cmd_Volume_Up = 0x20, Cmd_Volume_Down = 0x40
 };

enum REPORT_STATE
{
  REPORT_RDY, REPORT_CMD_RELEASE, REPORT_CMD
 };

void Init_HW(void);
void Timer_Task(void);
void Encoder_Task(void);
void HID_Task(void);

#endif /* HARDWARE_H_ */
