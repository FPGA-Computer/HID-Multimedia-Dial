/*
 * hardware.c
 *
 * Created: 22/12/2016 6:03:49 AM
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

#include "hardware.h"

uint8_t ReportIn[REPORT_SIZE], Report_State = REPORT_RDY;

// Rotary encoder - positive count is clockwise
volatile int8_t Encoder_;
static uint8_t Encoder_Prev;
int8_t Encoder = 0;
  
ISR(ENC_CLK_IRQ)
{
  uint8_t Enc_Status;

  Enc_Status = (REG(ENC_CLK_PORT,PIN) & ENC_CLK)|(REG(ENC_PORT,PIN) & ENC_DT);
  sei();
  
  // allow interrupt nesting after this point  
  if((Encoder_Prev == ENC_DT) && (Enc_Status == ENC_CLK))
    Encoder_--;    
  else if((!Encoder_Prev) && (Enc_Status == (ENC_CLK|ENC_DT)))
    Encoder_++;

  Encoder_Prev = Enc_Status;
}

static uint8_t Switch_Status = SW_EDGE_MASK, Sw_Timer, Sw_State = SW_NONE, Enc_State = ENC_VOLUME;

void Timer_Task(void)
{
  // timer0 overflow IRQ doesn't work nicely with V-USB, so use polling loop
  if(TIFR & (1<<TOV0))
  {
    // clears Timer 0 overflow flag
    TIFR = (1<<TOV0);                     
  
    if(Sw_Timer)
      Sw_Timer--;

    // keep track of switch by storing history in a shift register
    Switch_Status <<= 1;
  
    if(REG(ENC_PORT,PIN) & ENC_SWITCH)
      Switch_Status |= 1;
    
    // key press state machine
    switch(Sw_State)
    {
      case SW_NONE:
        if(SW_AT_MAKE)
        {
          Sw_State = SW_PRESS;
          Sw_Timer = TIMER_CLICK_MAKE;
        }
        break;

      case SW_PRESS:
        if(!Sw_Timer)                                 // Double click times out -> Pressed
        {                                             
          if(Report_State == REPORT_RDY)              // cheat: wait for HID to be ready
          {
            Sw_State = SW_HOLD;
            Sw_Timer = TIMER_LONG;
          
            // normal click
            ReportIn[1] = (Enc_State == ENC_VOLUME)?Cmd_Mute:Cmd_Play_Pause;
            Report_State = REPORT_CMD;
          }          
        }
        else if (SW_BREAK)
        {
          Sw_State = SW_DBL_BREAK;
          Sw_Timer = TIMER_DBL_BREAK;
        }           
        break;
      
      case SW_DBL_BREAK:
        if(!Sw_Timer)                                 // break is too long, treat it as no pressed
          Sw_State = SW_NONE;
        else if(SW_AT_MAKE)
          Sw_State = SW_DBL_CLICK; 
        break;

      case SW_DBL_CLICK:
        // handle double click here
        LED_TOGGLE(LED_GREEN_N);
        Enc_State = !Enc_State;
          
        Sw_Timer = TIMER_LONG;
        Sw_State = SW_BREAK_WAIT;        
        break;
      
      case SW_HOLD:
        if(!Sw_Timer)                                 // Long press
        {
          if(Report_State == REPORT_RDY)              // cheat: wait to HID to be ready
          {
            ReportIn[1] = (Enc_State == ENC_VOLUME)?Cmd_Play_Pause:Cmd_Stop;
            Sw_State = SW_BREAK_WAIT;          
            Report_State = REPORT_CMD;
          }                     
        }
        else if (SW_BREAK)
          Sw_State = SW_NONE;
        break;
        
      case SW_BREAK_WAIT:
        if (SW_BREAK)
          Sw_State = SW_NONE;
      break;                
    } 
  }
}

void Encoder_Task(void)
{
  if(Report_State == REPORT_RDY)
  {
    if(Encoder_)              // Update Encoder from IRQ value
    {
      uint8_t Encoder_IRQ;
      
      cli();                  // disable interrupt and gain access to Encoder_Count
      Encoder_IRQ = Encoder_; // Keep it very short
      Encoder_ = 0;
      sei();
      
      Encoder += Encoder_IRQ;
    }
    
    // report delta value as a series of Inc/Dec events
    if(Encoder >0)
    {
      Encoder--;
      ReportIn[1]|= (Enc_State == ENC_VOLUME)?Cmd_Volume_Up:Cmd_Next_Track;
      Report_State = REPORT_CMD;
    }
    else if (Encoder <0)
    {
      Encoder++;
      ReportIn[1]|= (Enc_State == ENC_VOLUME)?Cmd_Volume_Down:Cmd_Prev_Track;
      Report_State = REPORT_CMD;
    }
  }    
}    

void HID_Task(void)
{
  if ((Report_State != REPORT_RDY) && (usbInterruptIsReady()))
  {
    switch(Report_State)
    {
      case REPORT_CMD:
        usbSetInterrupt((void *)ReportIn,REPORT_SIZE);
        Report_State = REPORT_CMD_RELEASE;
        break;
        
      case REPORT_CMD_RELEASE:
        // clear all bits in report
        ReportIn[1] = 0;
        usbSetInterrupt((void *)ReportIn,REPORT_SIZE);
        Report_State = REPORT_RDY;
        break;
    }
  }
}

void Init_HW(void)
{ 
  LED_OFF(LED_AMBER_N|LED_GREEN_N);
  REG(LED_PORT,PORT) |= LED_AMBER_N|LED_GREEN_N;
  
  // Set unused GPIO to output
  DDRB = DDRB_INIT;
  DDRC = DDRC_INIT;
  DDRD = DDRD_INIT;

  // Timer 0 = 16MHz/1024: 64us resolution, 16.384ms overflow
  TCCR0 = (1<<CS02)|(1<<CS00);

  // INT1 = level change for encoder
  MCUCR |= (1<<ISC10);
  GICR |= (1<<INT1);
  
  ReportIn[0] = ID_CONSUMER;
}  
