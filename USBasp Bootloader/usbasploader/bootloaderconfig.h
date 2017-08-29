// Required: port and bits used for both USB data lines (D+ must also connect to INT0)
#define USB_CFG_IOPORTNAME  D
#define USB_CFG_DMINUS_BIT  2
#define USB_CFG_DPLUS_BIT   4

// Nothing more is required in this file. Everything else is optional and customizes options.

// Without any configuration options below this, the bootloader will run after any kind of reset
// and wait indefinitely until avrdude connects. Once avrdude disconnects, the user
// program gets run. Override by copying configuration lines from bootloaderconfig-palette.h

// Bootloader runs only when reset is triggered externally (e.g. a reset button).
#define BOOTLOADER_ON_RESET 1
#define BOOTLOADER_CAN_EXIT 0

//**** Code size reduction
// Least-important features listed first
//#define HAVE_READ_LOCK_FUSE         0 // Disable read fuse bytes
#define HAVE_FLASH_BYTE_READACCESS  0 // Disable read individual flash bytes
//#define HAVE_EEPROM_BYTE_ACCESS     0 // Disable read/write individual eeprom bytes
#define HAVE_EEPROM_PAGED_ACCESS    0 // Disable upload/download eeprom
//#define HAVE_FLASH_PAGED_READ       0 // Disable download flash

// move bootloaderconfig.inc into here
#define F_CPU 16000000
#define BOOTLOADER_ADDRESS 0x1800
