/*
 * platform.h
 *
 * Created: 11/02/2014 9:31:46 PM
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

#ifndef PLATFORM_H_
#define PLATFORM_H_

#define F_CPU			        16000000
#define USB_PORT          D
#define USB_DMINUS		    2
#define USB_DPLUS		      4

#define USB_POWER		      50
#define USB_INT_POLL      10

#define USB_CFG_SIZE	    43

#define USB_VENDOR_ID     0xc0, 0x16
#define USB_VENDOR_NAME   'J','B','B',' ','H','a','c','k'
#define USB_VENDOR_LEN    8

#define USB_DEVICE_ID     0xcd, 0x05
#define USB_DEVICE_NAME   'H','I','D',' ','M','u','l','t','i','m','e','d','i','a',' ','D','i','a','l'
#define USB_DEVICE_LEN    19

#define USB_SERIAL_NUM    '0','0','0','1'
#define USB_SERIAL_LEN    4

#define USB_IF_CLASS      3
#endif /* PLATFORM_H_ */
