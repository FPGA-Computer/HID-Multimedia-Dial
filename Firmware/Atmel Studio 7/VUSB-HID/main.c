/*
 * VUSB-HID.c
 *
 * Created: 26/08/2017 5:09:00 PM
 * Author : K. C. Lee
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

int main(void)
{ 
  Init_HW();
  
  usbInit();
  usbDeviceDisconnect();
  _delay_ms(250);
  usbDeviceConnect();
  sei();				                      /* set global interrupt enable */ 
  
  // main event loop

  for(;;)
  {
    usbPoll();
    HID_Task();
    Timer_Task();    
    Encoder_Task();
   }
 }

usbMsgLen_t usbFunctionSetup(uchar setupData[8])
{
  usbRequest_t *Request = (void *)setupData;   // cast to structured data for parsing
  
  if(Request->bRequest==USBRQ_HID_SET_REPORT)
  { 
    return(USB_NO_MSG);    
   }
  else if (Request->bRequest==USBRQ_HID_GET_REPORT)
  { 
   }

  return 0;                               // ignore all unknown requests
}

// generated from HID Descriptor Tool (DT): http://www.usb.org/developers/hidpage/dt2_4.zip

const PROGMEM char usbHidReportDescriptor[] =
 {
    0x05, 0x0c,                    // USAGE_PAGE (Consumer Devices)
    0x09, 0x01,                    // USAGE (Consumer Control)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x85, 0x01,                    //   REPORT_ID (1)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x04,                    //   REPORT_COUNT (4)
    0x19, 0xb5,                    //   USAGE_MINIMUM (Scan Next Track)
    0x29, 0xb7,                    //   USAGE_MAXIMUM (Stop)
    0x09, 0xcd,                    //   USAGE (Unassigned)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x09, 0xe2,                    //   USAGE (Mute)
    0x81, 0x06,                    //   INPUT (Data,Var,Rel)
    0x95, 0x02,                    //   REPORT_COUNT (2)
    0x09, 0xe9,                    //   USAGE (Volume Up)
    0x09, 0xea,                    //   USAGE (Volume Down)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)
    0xc0                           // END_COLLECTION
  };

