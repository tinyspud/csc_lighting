/* Project wide defines - give every file the ability to see hardware, see main,
 * do debug, get the time, enqueue commands with delays */
#ifndef __DCP_COMMON
#define __DCP_COMMON
#include <msp430.h>
#include <stdint.h>

#include "SystemTimer.h"

#include "main.h"
#include "UARTprotocol.h"
#include "UARTInterface.h"


/* Project wide defines */
#define NULL	0

#define TRUE	1
#define FALSE	0

#define MAX_UINT_32_T	(0xFFFFFFFF)
#define MIN_UINT_32_T	(0x00000000)
#define MAX_UINT_16_T	(0xFFFF)
#define MIN_UINT_16_T	(0x0000)
#define MAX_UINT_8_T	(0xFF)
#define MIN_UINT_8_T	(0x00)

#define INVALID_INT_INDEX	(-1)
#define INVALID_UINT16_INDEX	MAX_UINT_16_T

#define MAX_STRING_LENGTH	(255)
#define GROUP_LENGTH_PLACEHOLDER	(0)

/* Project wide macros used */
#define PULL_UCHAR_FROM_PTR(ptr, idx)	*(ptr + idx); idx++;
#define PULL_UINT_FROM_PTR(ptr, idx)	(unsigned int)(*(ptr + idx) << 8) | (unsigned int)(*(ptr + idx + 1)); idx += 2;



#define PERIOD_EA_SESSION_CHECKIN	1

/* Parameters for iAP2 device identification */
/* Notes:
 * All strings must be null terminated ('\0' character) with a max
 * length of MAX_STRING_LENGTH (defined below)
 * Be careful of the RAM used to store all of these arrays.  This
 * iAP2 link is stripped down and as minimal as I could get it but
 * it still represents a large amount of the memory of this device.
 *  */
#define NAME_STRING				"Disposable Coagulation Profiler\0"
#define MODEL_STRING			"0\0"
#define MANUFACTURER_STRING		"Oceanit Laboratories\0"
#define FIRMWARE_STRING			"0.0.1\0"
#define HARDWARE_STRING			"0.0.0\0"

/* Current and supported languages (2 char string) */
#define CURRENTLANG_STRING		"en\0"
#define SUPPORTEDLANG_STRING	"en\0"

#define REVERSE_DNS_ID_STRING	"com.oceanit.dcp0\0"
#define SERIAL_TRANSPORT_COMPONENT_NAME	"Serial\0"
#define MAX_CURRENT_DRAWN_FROM_DEVICE	1 /* mA */

/* Only uncomment if you are enabling the external accessory to use the USB
 * Host mode */
//#define ENABLE_EXTERNAL_ACCESSORY_NATIVE_TRANSPORT_USB_HOST

/* Number of seconds for the link to timeout.  Once the link times out
 * then the accessory goes back to broadcasting looking for the iphone.
 * Any iAP2 link level message that is received and successfully interpreted
 * by the accessory will kick this timer.
 *  */
#define IAP2_LINK_TIMEOUT 180 /* Seconds */

/* Uncomment this if you are debugging with the PC simulation tool in
 * the same git repository as this.  PC Ctrl Example; otherwise
 * comment this line out.  This is used to generate a predictable iAP2 link
 * starting address (less scripting required on the PC side)
 * If this is commented out then the system will use a "random" number to
 * initiate the link (uses the system's timer)
 *  */
//#define DEBUG_W_PC

/* Uncomment this if you want the iphone to charge when it's plugged in to
 * the accessory (if DEBUG_W_PC is included then this will be off by default) */
#define CHARGE_IPHONE

/* Chose one power amount to give to the iPhone (if DEBUG_W_PC is included then this
 * is set to 0mA) - This will go with the lowest defined amount */
//#define POWER_FOR_IPHONE_0mA
#define POWER_FOR_IPHONE_1000mA
//#define POWER_FOR_IPHONE_2100mA
//#define POWER_FOR_IPHONE_2400mA
//
//


#define DEBUG_W_PC

// #define APPMATCHTEAMID_INCLUDED

//#define KEEP_POWER_MESSAGES_CYCLED


/* This is used to determine the amount of power sent to the iPhone */
#ifdef DEBUG_W_PC
#define POWER_AVAILABLE_TO_IPHONE 0
#define IPHONE_SHOULD_CHARGE	FALSE

#else
#ifdef POWER_FOR_IPHONE_0mA
#define POWER_AVAILABLE_TO_IPHONE 0
#define IPHONE_SHOULD_CHARGE	FALSE

#else
#ifdef POWER_FOR_IPHONE_1000mA
#ifdef CHARGE_IPHONE
#define POWER_AVAILABLE_TO_IPHONE 1000
#define IPHONE_SHOULD_CHARGE	TRUE

#else
#define POWER_AVAILABLE_TO_IPHONE 0
#define IPHONE_SHOULD_CHARGE	FALSE

#endif
#else
#ifdef POWER_FOR_IPHONE_2100mA
#ifdef CHARGE_IPHONE
#define POWER_AVAILABLE_TO_IPHONE 2100
#define IPHONE_SHOULD_CHARGE	TRUE

#else
#define POWER_AVAILABLE_TO_IPHONE 0
#define IPHONE_SHOULD_CHARGE	FALSE

#endif
#else
#ifdef POWER_FOR_IPHONE_2400mA
#ifdef CHARGE_IPHONE
#define POWER_AVAILABLE_TO_IPHONE 2400
#define IPHONE_SHOULD_CHARGE	TRUE

#else
#define POWER_AVAILABLE_TO_IPHONE 0
#define IPHONE_SHOULD_CHARGE	FALSE

#endif
#else
#define POWER_AVAILABLE_TO_IPHONE 0
#define IPHONE_SHOULD_CHARGE	FALSE

#endif
#endif
#endif
#endif
#endif









#endif
