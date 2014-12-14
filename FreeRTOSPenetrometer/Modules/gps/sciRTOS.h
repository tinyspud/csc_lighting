/*
	SafeRTOS Copyright (C) Wittenstein High Integrity Systems.

	See projdefs.h for version number information.

	SafeRTOS is distributed exclusively by Wittenstein High Integrity Systems,
	and is subject to the terms of the License granted to your organization,
	including its warranties and limitations on distribution.  It cannot be
	copied or reproduced in any way except as permitted by the License.

	Licenses are issued for each concurrent user working on a specified product
	line.

	WITTENSTEIN high integrity systems is a trading name of WITTENSTEIN
	aerospace & simulation ltd, Registered Office: Brown's Court, Long Ashton
	Business Park, Yanley Lane, Long Ashton, Bristol, BS41 9LB, UK.
	Tel: +44 (0) 1275 395 600, fax: +44 (0) 1275 393 630.
	E-mail: info@HighIntegritySystems.com
	Registered in England No. 3711047; VAT No. GB 729 1583 15

	http://www.SafeRTOS.com
*/

/*!
 ****************************************************************************
 * @brief  SafeRTOS Project Test Harness
 * @file   UART.h
 * @date   November 2012
 ****************************************************************************
 * @note   Serial USART Test Message Port Interface Definitions
 ****************************************************************************/

#ifndef UART_H
#define UART_H

/* SafeRTOS Includes */
#include "FreeRTOS.h"


/*-- Public CONSTANT Definitions -------------------------------------------*/

#define  UART_INIT_HARD			( 0x0012 )
#define  UART_INIT_SOFT			( 0x0022 )

#define  UART_MODE_ACTIVE		( 0x0102 )
#define  UART_MODE_POLLED		( 0x0202 )
#define  UART_MODE_DEFAULT		( UART_MODE_POLLED )

#define  UART_PORT_DIRECT		( 0x1002 )
#define  UART_PORT_SHARED		( 0x2002 )
#define  UART_PORT_DEFAULT		( UART_PORT_DIRECT )


/*-- Public ENUM-TYPE Definitions ------------------------------------------*/

/*-- Public FUNCTION Declarations ------------------------------------------*/

void  vSerialPortInit( void );

//portBaseType  xSerialGetChar( portCharType * pcGetCharBuf,
//							  portTickType    xGetCharWait );
//
//portBaseType  xSerialPutChar( portCharType cPutChar,
//							  portTickType xPutCharWait );
//
//void  vUARTInit( void );
//
//void  vUARTMode( portBaseType xModeIdent );
//void  vUARTPort( portBaseType xPortIdent );
//
//void  vUARTIntrHandler( void );
//
//portBaseType  xUARTChrPend( void );
//portBaseType  xUARTChrSent( void );
//
//portBaseType  xUARTChrRead( portCharType * pcChrReadBuf,
//								  portTickType    xChrReadWait );
//
//portBaseType  xUARTChrSend( portCharType cChrSendChar,
//								  portTickType xChrSendWait );
//
//portBaseType  xUARTMsgPend( void );
//portBaseType  xUARTMsgSent( void );
//portBaseType  xUARTMsgRead( portCharType * pcMsgReadBuf,
//								  portTickType    xMsgReadWait );
//portBaseType  xUARTMsgSend( portCharType * pcMsgSendBuf,
//								  portTickType    xMsgSendWait );
//
//portBaseType  xUARTGetChar( portCharType * pcGetCharBuf,
//								  portTickType    xGetCharWait );
//
//portBaseType  xUARTPutChar( portCharType cPutChar,
//								  portTickType xPutCharWait );
//
#endif  /* UART_H */
