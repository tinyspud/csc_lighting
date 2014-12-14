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
 * @brief  SafeRTOS CCS Project Test Harness
 * @file   UART.c
 * @date   March 2013
 ****************************************************************************
 * @note   Serial UART Test Message Port Interface Procedures
  ****************************************************************************
 * @note   Project Dependent - Modify for Individual Project Requirements
 ****************************************************************************/

/* Library Includes */
#include <string.h>

/* SafeRTOS Includes */
#include "FreeRTOS.h"

/* Test Harness Includes */
#include "SerialUartDef.h"
#include "sciRTOS.h"


/*-- Local MACRO Definitions -----------------------------------------------*/


/*-- Local CONSTANT Definitions --------------------------------------------*/

/*!- Wait Period After Polled Send Character  - Modify for given Project */
#define UART_WAIT_INTR			( 1000 )
#define UART_WAIT_POLL			( 5000 )

/*!- Serial Test Port Operation Control */
#define UART_ENABLE				( 11 )
#define UART_ENABLE_RXR			( 12 )
#define UART_ENABLE_TXR			( 13 )

#define UART_DISABLE				( 21 )
#define UART_DISABLE_RXR			( 22 )
#define UART_DISABLE_TXR			( 23 )

/*!- No Block on Receiver Character Queue */
#define  UART_BLOCK_NONE			( ( portTickType ) 0 )

/*!- Character TX & RX Queue Lengths */
#define UART_QUEUE_RX_LEN		(  64 )
#define UART_QUEUE_TX_LEN		( 128 )

#define UART_QUEUE_RX_BUF_LEN	(  64 + portQUEUE_OVERHEAD_BYTES )
#define UART_QUEUE_TX_BUF_LEN	( 128 + portQUEUE_OVERHEAD_BYTES )


/*-- Local ENUM-TYPE Definitions -------------------------------------------*/


/*-- Local VARIABLE Declarations -------------------------------------------*/

/*!- Serial Test Operating Parameters */
//#pragma DATA_SECTION( xUARTMode, "lnkDIAG_DATA_SECTION" );
static portBaseType  xUARTMode	= UART_MODE_POLLED;

//#pragma DATA_SECTION( xUARTPort, "lnkDIAG_DATA_SECTION" );
static portBaseType xUARTPort = UART_PORT_DIRECT;

/*!- Serial I/O Counters */
//#pragma DATA_SECTION( xSerialRxChrCnt, "lnkDIAG_DATA_SECTION" );
static portBaseType xSerialRxChrCnt = 0;

//#pragma DATA_SECTION( xSerialRxMsgCnt, "lnkDIAG_DATA_SECTION" );
static portBaseType xSerialRxMsgCnt = 0;

//#pragma DATA_SECTION( xSerialTxChrCnt, "lnkDIAG_DATA_SECTION" );
static portBaseType xSerialTxChrCnt = 0;

/*!- Serial I/O Instrumentation */
//#pragma DATA_SECTION( xSerialRxChrMax, "lnkDIAG_DATA_SECTION" );
static portBaseType xSerialRxChrMax = 0;

//#pragma DATA_SECTION( xSerialRxErrBit, "lnkDIAG_DATA_SECTION" );
static portBaseType xSerialRxErrBit = 0;

//#pragma DATA_SECTION( xSerialRxErrCnt, "lnkDIAG_DATA_SECTION" );
static portBaseType xSerialRxErrCnt = 0;

//#pragma DATA_SECTION( xSerialRxErrInt, "lnkDIAG_DATA_SECTION" );
static portBaseType xSerialRxErrInt = 0;

//#pragma DATA_SECTION( xSerialRxTmoMin, "lnkDIAG_DATA_SECTION" );
static portBaseType xSerialRxTmoMin = ( UART_WAIT_POLL * 2 );

//#pragma DATA_SECTION( xSerialTxChrMax, "lnkDIAG_DATA_SECTION" );
static portBaseType xSerialTxChrMax = 0;

//#pragma DATA_SECTION( xSerialTxErrCnt, "lnkDIAG_DATA_SECTION" );
static portBaseType xSerialTxErrCnt = 0;

//#pragma DATA_SECTION( xSerialTxErrInt, "lnkDIAG_DATA_SECTION" );
static portBaseType xSerialTxErrInt = 0;

//#pragma DATA_SECTION( xSerialTxTmoMin, "lnkDIAG_DATA_SECTION" );
static portBaseType xSerialTxTmoMin = ( UART_WAIT_POLL * 2 );

/*!- Serial Transmit Character Pending Indicator */
//#pragma DATA_SECTION( xSerialTxQueWait, "lnkDIAG_DATA_SECTION" );
static volatile portBaseType  xSerialTxQueWait	= pdFALSE;

/*!- Character I/O Queue Buffer Handle Identifiers */
//#pragma DATA_SECTION( xSerialQueueRxHnd, "lnkDIAG_DATA_SECTION" );
static xQueueHandle xSerialQueueRxHnd = 0;

//#pragma DATA_SECTION( xSerialQueueTxHnd, "lnkDIAG_DATA_SECTION" );
static xQueueHandle xSerialQueueTxHnd = 0;

/*!- Character I/O Queue Buffers */
#pragma DATA_ALIGN( acSerialQueueg_rx_buffer_for_gps_msg, 8 )
static portInt8Type acSerialQueueg_rx_buffer_for_gps_msg[ UART_QUEUE_RX_BUF_LEN ] = { 0 };

#pragma DATA_ALIGN( acSerialQueueTxBuf, 8 )
static portInt8Type acSerialQueueTxBuf[ UART_QUEUE_TX_BUF_LEN ] = { 0 };


/*-- Local FUNCTION Declarations -------------------------------------------*/

//static portBaseType  xUARTInitHard( void );
static portBaseType  xUARTInitSoft( void );

static portBaseType  xUARTRxRdy( portBaseType xRxRdyBit,
									   portTickType xRxRdyTmo );

static portBaseType  xUARTSend( portCharType cSendChar,
									  portTickType xSendWait );

static portBaseType  xUARTTxChr( portCharType cTxChrVal,
							 		   portTickType	xTxChrTmo );

static portBaseType  xUARTTxReg( portCharType cTxRegChr,
								       portTickType	xTxRegTmo );

static portBaseType  xUARTTxRdy( portBaseType xTxRdyBit,
									   portTickType xTxRdyTmo );

static void  vUARTControl( portBaseType xCtrlState );
static void  vUARTCtrlIrq( portBaseType xCtrlIrqState );


/*!
 ****************************************************************************
 * @brief	Test Harness Serial Test Port Initialisation Interface
 ****************************************************************************/
void  vSerialPortInit( void )
{
	/*!- Initialise Serial Test Port */
	vUARTInit( );

	/*!- Set Interrupt Operating Mode */
	vUARTMode( UART_MODE_ACTIVE );
}

/*!
 ****************************************************************************
 * @brief	Test Harness Serial Test Port Get Character Interface
 ****************************************************************************
 * @param   pcGetCharBuf  - Received Character Buffer Address
 * @param    xGetCharWait - Received Character Wait Time Period
 ****************************************************************************
 * @return  Result Status ( PASS upon Success, else FAIL )
 ****************************************************************************/
portBaseType  xSerialGetChar( portCharType* pcGetCharBuf,
							  portTickType	 xGetCharWait )
{
	/*!- Read Serial Test Port Received Character */
	return  xUARTChrRead( ( portCharType * ) pcGetCharBuf, xGetCharWait );
}

/*!
 ****************************************************************************
 * @brief	Test Harness Serial Test Port Put Character Interface
  ****************************************************************************
 * @param   cPutChar 	 - Transmit Character
 * @param   xGetCharWait - Transmit Character Wait Time Period
 ****************************************************************************
 * @return  Result Status ( PASS upon Success, else FAIL )
 ****************************************************************************/
portBaseType  xSerialPutChar( portCharType cPutChar,
							  portTickType xPutCharWait )
{
	/*!- Send Serial Test Port Transmit Character */
	return  xUARTChrSend( cPutChar, xPutCharWait );
}

/*!
 ****************************************************************************
 * @brief	Serial Test Designated UART Port Initialisation
 ****************************************************************************
 * @param	xInitIdent - Initialisation Type Identifier ( Hard or SOFT ).
 ****************************************************************************/
void  vUARTInit( void )
{
	/*!- Initialise Device Interface */
	//( void ) xUARTInitHard( );

	/*!- Initialise Operating Services */
	( void ) xUARTInitSoft( );

	/*!- Initialise Operating Parameters */
	vUARTPort( UART_PORT_DIRECT );
	vUARTControl( UART_DISABLE );
	vUARTMode( UART_MODE_POLLED );
}

/*!
 ****************************************************************************
 * @brief   Set Serial Test Operating Mode
 ****************************************************************************
 * @param	xModeIdent - Mode Identifier ( ACTIVE or POLLED )
 ****************************************************************************/
void  vUARTMode( portBaseType xModeIdent )
{
portBaseType  xModeCount = 1;

	switch( xModeIdent )
	{
		case UART_MODE_ACTIVE:
			xUARTMode = xModeIdent;
			vUARTCtrlIrq( UART_ENABLE );
			break;

		case UART_MODE_POLLED:
			/*!- Before going from ACTIVE into POLLED Operating Mode */
			if( xUARTMode == UART_MODE_ACTIVE )
			{
				/*!- Ensure Interrupts are Enabled! */
				vUARTCtrlIrq( UART_ENABLE );

				/*!- Must Wait While Transmit Buffer is Flushed Empty */
				while( xModeCount > 0 )
				{
					xModeCount = xUARTChrSent( );
				}
			}

			xUARTMode = xModeIdent;
			vUARTCtrlIrq( UART_DISABLE );
			break;

		default:
			break;
	}
}

/*!
 ****************************************************************************
 * @brief   Set Serial Test Port Output
 ****************************************************************************
 * @param	xPortIdent - Mode Identifier ( DIRECT or SHARED )
 ****************************************************************************/
void  vUARTPort( portBaseType xPortIdent )
{
	switch( xPortIdent )
	{
		case UART_PORT_DIRECT:
			xUARTPort = xPortIdent;

			if( xUARTMode == UART_MODE_ACTIVE )
			{
				/*!- Enable USART Interrupts */
				vUARTCtrlIrq( UART_ENABLE );
			}
			break;

		case UART_PORT_SHARED:
			xUARTPort = xPortIdent;

			if( xUARTMode == UART_MODE_ACTIVE )
			{
				/*!- Disable USART Interrupts */
				vUARTCtrlIrq( UART_DISABLE );
			}
			break;

		default:
			break;
	}
}

/*!
 ****************************************************************************
 * @brief   Serial Test UART Port Interrupt Handler
 ****************************************************************************
 * @note	Called directly via System Vector Table
 ****************************************************************************
 * @note	Only Operational when MODE = ACTIVE.
 ****************************************************************************/
#pragma CODE_STATE(sciHighLevelInterrupt, 32)
#pragma INTERRUPT(sciHighLevelInterrupt, IRQ)
void sciHighLevelInterrupt( void )
{
portCharType cTestIntrChar;
portBaseType xTestIntrState;
portBaseType xTestIntrValue;
portBaseType xTestIntrWoken;

	/*!- Read Source of USART Interrupt */
	xTestIntrWoken = pdFALSE;
	xTestIntrState = serSCI1_INT_VECT_0_REGISTER;

	/*!- Read & Mask USART Operating Status Flags */
	xTestIntrValue  = serSCI1_FLR_INT_REGISTER;
	xTestIntrValue &= ( serSCI_FLR_PE_BIT | serSCI_FLR_OE_BIT | serSCI_FLR_FE_BIT );

	/*!- USART Operating Receiver Error? */
	if( xTestIntrValue != 0 )
	{
		xSerialRxErrBit  = xTestIntrValue;
		xSerialRxErrCnt += 1;
	}

	switch( xTestIntrState )
	{
		case serSRC_INT_RECEIVE:
			/*!- Read Received Character Directly from USART */
			xTestIntrValue  = serSCI1_RD_REGISTER;
			xTestIntrValue &= serSCI_DATA_MASK;
			cTestIntrChar   = ( portCharType ) xTestIntrValue;

			/*!- Post Received Character onto Queue
			 *   which also Returns Higher Priority Task Awaken Indicator */
			xTestIntrState = xQueueSendFromISR( xSerialQueueRxHnd,
												   &cTestIntrChar,
												   &xTestIntrWoken );
			/*!- Bump Received Message Counter? */
			if( xTestIntrState == pdPASS )
			{
				xSerialRxChrCnt += 1;

				if( cTestIntrChar == '\n' )
				{
					xSerialRxMsgCnt += 1;
				}
			}
			else
			{
				xSerialRxErrInt  = xTestIntrState;
				xSerialRxErrCnt += 1;
			}

			/*!- Ensure USART Receive Interrupt Enabled */
			serSCI1_SET_INT_REGISTER = serSET_INT_RX_BIT;
			break;

		case serSRC_INT_TRANSMIT:
			if(	xSerialTxChrCnt > 0 )
			{
				xSerialTxChrCnt -= 1;
			}

			/*!- Interrupt Caused by TxData Register Becoming Empty
			 *   Are there any more characters to transmit? */
			xTestIntrState = xQueueReceiveFromISR( xSerialQueueTxHnd,
													  &cTestIntrChar,
													  &xTestIntrWoken );
			if( xTestIntrState == pdPASS )
			{
				/*!- Single Character Retrieved from Queue */
				xSerialTxChrCnt += 1;

				/*!- Write Character Directly into USART */
				serSCI1_TD_REGISTER = ( portUInt32Type ) cTestIntrChar;

				/*!- Ensure USART Transmit Interrupts Enabled */
				serSCI1_SET_INT_REGISTER = serSET_INT_TX_BIT;
			}
			else if( xTestIntrState == errQUEUE_EMPTY )
			{
				/*!- Done for Now, So Allow Direct USART Operation */
				xSerialTxQueWait = pdFALSE;

				/*!- Disable Transmit Interrupts */
				serSCI1_CLR_INT_REGISTER = serCLR_INT_TX_BIT;
			}
			else
			{
				xSerialTxErrInt  = xTestIntrState;
				xSerialTxErrCnt += 1;
			}
			break;

		default:
			/*!- Phantom Interrupt, So Clear Flags and Return */
			xTestIntrValue  = serSCI1_SET_LVL_REGISTER;
			xTestIntrValue  = ~xTestIntrValue;
			xTestIntrValue &= 0x07000303;

			serSCI1_FLR_INT_REGISTER = xTestIntrValue;
			break;
	}

	/*!- Exit ISR: If a task was awakened by either a character being
	 *			   received or transmitted then a context switch will occur */
	portYIELD_FROM_ISR( xTestIntrWoken );
}

/*!
 ****************************************************************************
 * @brief   Serial Test Received Characters Pending
 ****************************************************************************
 * @return  Result Status ( Character Count, else 0 )
 ****************************************************************************/
portBaseType xUARTChrPend( void )
{
portBaseType xChrPendResult = 0;

portUInt32Type ulChrPendCount;
portUInt32Type ulChrPendStatus;

	/*!- Has USART Port Received a Character? */
	ulChrPendStatus  = serSCI1_FLR_INT_REGISTER;
	ulChrPendStatus &= serFLR_RDY_RX_BIT;

	/*!- Check when ACTIVE Operating Mode */
	if( xUARTMode == UART_MODE_ACTIVE )
	{
		 xChrPendResult = xQueueMessagesWaiting( xSerialQueueRxHnd,
											       &ulChrPendCount );
		if(	xChrPendResult == pdPASS )
		{
			xChrPendResult = ( portBaseType ) ulChrPendCount;
		}
	}

	/*!- Check when POLLED Operating Mode */
	if( xUARTMode == UART_MODE_POLLED )
	{
		ulChrPendStatus = xUARTRxRdy( pdFALSE, UART_WAIT_POLL );

		/*!- Yes, USART Port Received Character Waiting? */
		if( ulChrPendStatus > 0 )
		{
			/*!- Yes, Set Result Status Accordingly */
			xChrPendResult = 1;

			xSerialRxChrCnt += 1;
		}
	}

	return  xChrPendResult;
}

/*!
 ****************************************************************************
 * @brief   Serial Test Transmitted Characters Pending
 ****************************************************************************
 * @return  Result Status ( Character Count, else 0 )
 ****************************************************************************/
portBaseType  xUARTChrSent( void )
{
portBaseType  xChrSentResult = 0;

	/*!- Action when DIRECT Port Mode */
	if( xUARTPort == UART_PORT_DIRECT )
	{
		xChrSentResult = xSerialTxChrCnt;
	}

	/*!- Action when SHARE with Test Port Mode */
	if( xUARTPort == UART_PORT_SHARED )
	{
		xChrSentResult = xSerialTxChrCnt;
	}

	return  xChrSentResult;
}

/*!
 ****************************************************************************
 * @brief   Serial Test Read Received Character
 ****************************************************************************
 * @param   pcChrReadBuf  - Read Character Buffer Address
 * @param   xChrReadWait  - Read Character Buffer Timeout Period
 ****************************************************************************
 * @return  Result Status ( PASS upon Success, else FAIL )
 ****************************************************************************/
portBaseType  xUARTChrRead( portCharType * pcChrReadBuf,
								  portTickType    xChrReadWait )
{
portBaseType  xChrReadResult = pdFAIL;
portBaseType  xChrReadStatus;

portCharType    cChrReadChar;
portUInt32Type  ulChrReadCount;
portUInt32Type  ulChrReadValue;

	if( xUARTMode == UART_MODE_POLLED )
	{
		if( xSerialRxChrCnt > 0 )
		{
			xSerialRxChrCnt -= 1;

			/*!- Read Received Character Directly from Device */
			ulChrReadValue = serSCI1_RD_REGISTER;
			cChrReadChar   = ( portCharType ) ulChrReadValue;

			xChrReadResult = pdPASS;
		}
	}

	if( xUARTMode == UART_MODE_ACTIVE )
	{
		xChrReadStatus = xQueueMessagesWaiting( xSerialQueueRxHnd,
												  &ulChrReadCount );
		if(	xChrReadStatus == pdPASS )
		{
			xSerialRxChrCnt = ulChrReadCount;

			if(	ulChrReadCount > xSerialRxChrMax )
			{
				xSerialRxChrMax = ulChrReadCount;
			}
		}

		xChrReadResult = xQueueReceive( xSerialQueueRxHnd,
										    &cChrReadChar, xChrReadWait );
	}

	if(	xChrReadResult == pdPASS )
	{
		*pcChrReadBuf = cChrReadChar;
	}
	else
	{
		*pcChrReadBuf = '\0';
	}

	return  xChrReadResult;
}

/*!
 ****************************************************************************
 * @brief   Serial Test Send Character
 ****************************************************************************
 * @param   cChrSendChar - Send Character
 * @param   xChrSendWait - Send Character Wait Time Period
 ****************************************************************************
 * @return  Result Status ( PASS upon Success, else FAIL )
 ****************************************************************************/
portBaseType  xUARTChrSend( portCharType cChrSendChar,
								  portTickType xChrSendWait )
{
	/*!- Load Single Character to USART Port */
	return  xUARTSend( cChrSendChar, xChrSendWait );
}

/*!
 ****************************************************************************
 * @brief   Serial Test Received Message Pending
 ****************************************************************************
 * @return  Result Status ( Message Count, else 0 )
 ****************************************************************************/
portBaseType  xUARTMsgPend( void )
{
	return  xSerialRxMsgCnt;
}

/*!
 ****************************************************************************
 * @brief   Serial Test Transmit Message Pending
 ****************************************************************************
 * @return  Result Status ( Message Count, else 0 )
 ****************************************************************************/
portBaseType  xUARTMsgSent( void )
{
	return  xSerialTxChrCnt;
}

/*!
 ****************************************************************************
 * @brief   Serial Test Read Received Message
 ****************************************************************************
 * @param   pcMsgReadBuf  - Read Message Character Buffer Address
 * @param    xMsgReadWait - Read Message Character Wait Time Period
 ****************************************************************************
 * @return  Result Status ( Read Message Length, else FAIL ( 0 ) )
 ****************************************************************************/
portBaseType  xUARTMsgRead( portCharType * pcMsgReadBuf,
								  portTickType    xMsgReadWait )
{
portBaseType xMsgReadResult = pdFAIL;
portBaseType xMsgReadStatus = pdPASS;
portBaseType xMsgReadCount  = 0;
portBaseType xMsgReadIndex  = 0;
portCharType cMsgReadChar   = '\0';

	if( xUARTMode == UART_MODE_ACTIVE )
	{
		xMsgReadStatus = pdFAIL;

		if( xSerialRxMsgCnt > 0 )
		{
			while( cMsgReadChar != '\n' )
			{
				 xMsgReadStatus = xQueueReceive( xSerialQueueRxHnd,
												 &cMsgReadChar, xMsgReadWait );
				if(	xMsgReadStatus != pdPASS )
				{
					break;
				}
				else
				{
					*( pcMsgReadBuf + xMsgReadIndex ) = cMsgReadChar;

					xMsgReadCount += 1;
					xMsgReadIndex += 1;

					xSerialRxChrCnt -= 1;
				}
			}
		}

		if(	xMsgReadStatus == pdPASS )
		{
			*( pcMsgReadBuf + xMsgReadIndex ) = '\0';

			xMsgReadResult = xMsgReadCount;

			if( xSerialRxMsgCnt > 0 )
			{
				xSerialRxMsgCnt -= 1;
			}
		}
	}

	return  xMsgReadResult;
}

/*!
 ****************************************************************************
 * @brief   Serial Test Send Message
 ****************************************************************************
 * @param   pcMsgSendBuf  - Send Message Character Buffer Address
 * @param    xMsgSendWait - Send Message Character Wait Time Period
 ****************************************************************************
 * @return  Result Status ( PASS upon Success, else FAIL )
 ****************************************************************************/
portBaseType  xUARTMsgSend( portCharType * pcMsgSendBuf,
								  portTickType    xMsgSendWait )
{
portBaseType xMsgSendResult = pdFAIL;
portBaseType xMsgSendCount  = strlen( pcMsgSendBuf );
portBaseType xMsgSendIndex  = 0;
portCharType cMsgSendChar   = '\0';

	if( xMsgSendCount > UART_QUEUE_TX_LEN )
	{
		xMsgSendCount = UART_QUEUE_TX_LEN;
	}

	for( xMsgSendIndex = 0; xMsgSendIndex < xMsgSendCount; xMsgSendIndex++ )
	{
		cMsgSendChar = *( pcMsgSendBuf + xMsgSendIndex );

		xMsgSendResult = xUARTSend( cMsgSendChar, xMsgSendWait );

		if(	xMsgSendResult != pdPASS )
		{
			break;
		}
	}

	return  xMsgSendResult;
}


/****************************************************************************
 *  LOCAL FUNCTIONS
 ****************************************************************************/

/*!
 ****************************************************************************
 * @brief   Serial Test Send Character
 ****************************************************************************
 * @param   cSendChar - Send Character
 * @param   xSendWait - Send Character Wait Time Period
 ****************************************************************************
 * @return  Result Status ( PASS upon Success, else FAIL )
 ****************************************************************************/
static portBaseType  xUARTSend( portCharType cSendChar,
									  portTickType xSendWait )
{
portBaseType  xSendResult = pdFAIL;

	if( xUARTMode == UART_MODE_ACTIVE )
	{
		/*!- Yes, Queue Transmit Character */
		if( xSendWait < UART_WAIT_INTR )
		{
			xSendWait = UART_WAIT_INTR;
		}

		xSendResult = xUARTTxChr( cSendChar, xSendWait );
	}

	if( xUARTMode == UART_MODE_POLLED )
	{
		if( xSendWait < UART_WAIT_POLL )
		{
			xSendWait = UART_WAIT_POLL;
		}

		/*!- Ensure USART TxChar Buffer is Ready */
		xSendResult = xUARTTxRdy( pdFALSE, xSendWait );

		if( xSendResult == pdPASS )
		{
			/*!- Load Message Character into USART */
			serSCI1_TD_REGISTER = ( portUInt32Type ) cSendChar;

			/*!- Wait for USART TxChar to Complete */
			xSendResult = xUARTTxRdy( pdFALSE, xSendWait );
		}
	}

	return  xSendResult;
}

/*!
 ****************************************************************************
 * @brief   Serial Test Transmit Interrupt Character
 ****************************************************************************
 * @param   cTxChrVal - Transmit Character
 * @param   xTxChrTmo - Transmit Character Timeout Period
 ****************************************************************************
 * @return  Result Status ( PASS upon Success, else FAIL )
 ****************************************************************************
 * @note  If no characters are available or arrive before blocking time
 *        expires then operation deemed to of failed.
 ****************************************************************************/
static portBaseType  xUARTTxChr( portCharType cTxChrVal,
							 		   portTickType	xTxChrTmo )
{
portBaseType    xTxChrResult;
portBaseType    xTxChrStatus;
portUInt32Type ulTxChrCount;

	/*!- Ensure We Aren't Interrupted */
	portENTER_CRITICAL();
	{
		/*!- Can We Write Directly to USART? */
		if( xSerialTxQueWait == pdFALSE )
		{
			/*!- Yes, Load the Transmit Data Register with Character */
			xTxChrResult = xUARTTxReg( cTxChrVal, xTxChrTmo );
		}
		else
		{
			/*!- We cannot write directly to the USART, so queue the character.
			 *   We can get blocked for the maximum wait time if there is no
			 *   space in the queue, which is fine, within a critical section
			 *   as each task has its own critical section management */
			xTxChrResult = xQueueSend( xSerialQueueTxHnd, &cTxChrVal, xTxChrTmo );

			xTxChrStatus = xQueueMessagesWaiting( xSerialQueueTxHnd, &ulTxChrCount );

			if(	xTxChrStatus == pdPASS )
			{
				if(	ulTxChrCount > xSerialTxChrMax )
				{
					xSerialTxChrMax = ulTxChrCount;
				}
			}

			/*!- ReActivate Write Character Output? */
			if( xSerialTxQueWait == pdFALSE )
			{
				/*!- Get Next TX Character form Buffer */
				xTxChrResult = xQueueReceive( xSerialQueueTxHnd, &cTxChrVal,
											  	  	 UART_BLOCK_NONE );
				if( xTxChrResult == pdPASS )
				{
					/*!- Load the Transmit Data Register with Character */
					xTxChrResult = xUARTTxReg( cTxChrVal, xTxChrTmo );
				}
			}
		}
	}
	portEXIT_CRITICAL();

	if( xTxChrResult != pdPASS )
	{
		xSerialTxErrCnt -= 1;
	}

	/* Operational Error Checking */
	if( ( xSerialRxErrBit > 0   ) ||
	    ( xSerialRxErrCnt > 0   ) || ( xSerialTxErrCnt > 0   ) ||
		( xSerialRxChrMax > 60  ) || ( xSerialTxChrMax > 60  ) ||
		( xSerialRxTmoMin < 500 ) || ( xSerialTxTmoMin < 500 ) )
	{
		vParTestSetLED( 9, 0 );

		/* Keep Compiler Not Used Warnings at Bay! */
		xSerialTxErrInt = xSerialRxErrInt;
		xSerialRxErrInt = xSerialTxErrInt;
	}

	return  xTxChrResult;
}

/*!
 ****************************************************************************
 * @brief   Serial Test Load Transmit Register
 ****************************************************************************
 * @param   cTxRegVal - Transmit Character
 * @param   xTxRegTmo - Transmit Character Timeout Period
 ****************************************************************************
 * @return  Result Status ( PASS upon Success, else FAIL )
 ****************************************************************************/
static portBaseType  xUARTTxReg( portCharType cTxRegVal,
							 		   portTickType xTxRegTmo )
{
portBaseType  xTxRegValue;
portBaseType  xTxRegResult;

	/*!- Wait for Transmit Data Register to be Ready */
	xTxRegResult = xUARTTxRdy( pdTRUE, xTxRegTmo );

	if( xTxRegResult == pdPASS )
	{
		/*!- Yes, Set Queue Used Indication Flag */
		xSerialTxQueWait = pdTRUE;

		/*!- Save Last Transmit Character & Bump Counter */
		xSerialTxChrCnt += 1;

		/*!- Write Character Directly into USART Transmit Buffer */
		serSCI1_TD_REGISTER = ( portUInt32Type ) cTxRegVal;

		/*!- Ensure USART Port Interrupts are Enabled */
		xTxRegValue  = serSET_INT_RX_BIT;
		xTxRegValue |= serSET_INT_TX_BIT;
		serSCI1_SET_INT_REGISTER = xTxRegValue;
	}

	return  xTxRegResult;
}

/*!
 ****************************************************************************
 * @brief   Serial Test Receiver Ready
 ****************************************************************************
 * @param   xRxRdyBit - Wait Receiver Type ( pdFALSE = Ready )
 * @param   xRxRdyTmo - Wait Time Period
 ****************************************************************************
 * @return  Result Status ( PASS when Ready, else FAIL )
 ****************************************************************************/
static portBaseType  xUARTRxRdy( portBaseType xRxRdyBit,
									   portTickType xRxRdyTmo )
{
portBaseType  xRxRdyPeriod = xRxRdyTmo;
portBaseType  xRxRdyResult = pdFAIL;
portBaseType  xRxRdyStatus = 0;

	/*!- While USART RX Buffer NOT Ready */
	while( xRxRdyPeriod > 0 )
	{
		/*!- Save Lowest Timeout Period */
		if( xRxRdyPeriod < xSerialRxTmoMin )
		{
			xSerialRxTmoMin = xRxRdyPeriod;
		}

		/*!- Read USART Port Operating Status */
		xRxRdyStatus  = serSCI1_FLR_INT_REGISTER;

		/*!- Isolate USART Port Required Status */
		if( xRxRdyBit == pdFALSE )
		{
			xRxRdyStatus &= serFLR_RDY_RX_BIT;
		}
		else
		{
			xRxRdyStatus &= 0;
		}

		/*!- Has USART Port Read Character? */
		if( xRxRdyStatus != 0 )
		{
			/*!- Yes, Clear TimeOut Period Counter to Exit */
			xRxRdyPeriod = 0;
			xRxRdyResult = pdPASS;
		}
		else
		{
			/*!- No, Decrement TimeOut Period Counter */
			xRxRdyPeriod -= 1;
		}
	}

	return  xRxRdyResult;
}

/*!
 ****************************************************************************
 * @brief   Serial Test Transmitter Ready
 ****************************************************************************
 * @param   xTxRdyBit - Wait Transmitter Type ( pdFALSE = Empty )
 * @param   xTxRdyTmo - Wait Time Period
 ****************************************************************************
 * @return  Result Status ( PASS when Ready, else FAIL )
 ****************************************************************************/
static portBaseType  xUARTTxRdy( portBaseType xTxRdyBit,
									   portTickType xTxRdyTmo )
{
portBaseType  xTxRdyPeriod = xTxRdyTmo;
portBaseType  xTxRdyResult = pdFAIL;
portBaseType  xTxRdyStatus = 0;

	/*!- While USART TX Buffer NOT Ready */
	while( xTxRdyPeriod > 0 )
	{
		/*!- Save Lowest Timeout Period */
		if( xTxRdyPeriod < xSerialTxTmoMin )
		{
			xSerialTxTmoMin = xTxRdyPeriod;
		}

		/*!- Has USART Port Sent Character? */
		xTxRdyStatus = serSCI1_FLR_INT_REGISTER;

		if( xTxRdyBit == pdFALSE )
		{
			xTxRdyStatus &= serFLR_EMP_TX_BIT;
		}
		else
		{
			xTxRdyStatus &= serFLR_RDY_TX_BIT;
		}

		if( xTxRdyStatus > 0 )
		{
			/*!- Yes, Clear TimeOut Period Counter to Exit */
			xTxRdyPeriod = 0;
			xTxRdyResult = pdPASS;
		}
		else
		{
			/*!- No, Decrement TimeOut Period Counter */
			xTxRdyPeriod -= 1;
		}
	}

	return  xTxRdyResult;
}

/*!
 ****************************************************************************
 * @brief	Serial Test UART Port Software Initialisation
 ****************************************************************************
 * @return	Result Status ( TRUE upon Success, else FALSE ).
 ****************************************************************************/
static portBaseType  xUARTInitSoft( void )
{
portBaseType  xSoftResult   = pdFALSE;
portBaseType  xSoftStatus   = pdFALSE;
portBaseType  xSoftEntrySiz = 0;

	xSoftEntrySiz     = ( portBaseType ) sizeof( portCharType );
	xSerialQueueRxHnd = 0;
	xSerialQueueTxHnd = 0;

	/*!- Create Queue Buffer used to hold Rx Characters */
	xSoftStatus = xQueueCreate( acSerialQueueg_rx_buffer_for_gps_msg,
								UART_QUEUE_RX_BUF_LEN,
								UART_QUEUE_RX_LEN,
							    xSoftEntrySiz, &xSerialQueueRxHnd );

	if( xSoftStatus != pdFALSE )
	{
		/*!- Create Queue Buffer used to hold Tx Characters */
		xSoftStatus = xQueueCreate( acSerialQueueTxBuf,
									UART_QUEUE_TX_BUF_LEN,
									UART_QUEUE_TX_LEN,
								    xSoftEntrySiz, &xSerialQueueTxHnd );
	}

	/*!- Check Everything Created & Set Result Status Accordingly */
	if( xSoftStatus != pdFALSE )
	{
		if( xSerialQueueRxHnd != 0 )
		{
			if( xSerialQueueTxHnd != 0 )
			{
				xSoftResult = pdTRUE;
			}
		}
	}

	return  xSoftResult;
}

///*!
// ****************************************************************************
// * @brief	Serial Test UART Port Hardware Initialisation
// ****************************************************************************
// * @return	Result Status ( TRUE upon Success, else FALSE ).
// ****************************************************************************/
//static portBaseType  xUARTInitHard( void )
//{
//portBaseType  xInitResult = pdTRUE;
//
//portUInt32Type  ulInitClock;
//portUInt32Type  ulInitValue;
//
//	portENTER_CRITICAL();
//	{
//		/* Enable SCI 1 by setting RESET bit. */
//		serSCI1_GCR0_REGISTER = serSCI_GCR0_RESET_BIT;
//
//		/* Clear SWnRST to 0 before configuring the SCI. */
//		serSCI1_GCR1_REGISTER &= ~serSCI_GCR1_SWNRST_BIT;
//
//	    /* Disable all interrupts */
//		serSCI1_CLR_INT_REGISTER = 0xFFFFFFFF;
//		serSCI1_CLR_LVL_REGISTER = 0xFFFFFFFF;
//
//	    /* Global Control */
//		ulInitValue  = serSCI_GCR1_TXENA_BIT;
//		ulInitValue |= serSCI_GCR1_RXENA_BIT;
//		ulInitValue |= serSCI_GCR1_CONT_BIT;
//		ulInitValue |= serSCI_GCR1_CLOCK_BIT;
//		ulInitValue |= serSCI_GCR1_LOOPBACK_BIT;
//		ulInitValue |= serSCI_GCR1_1_STOP_BIT;
//		ulInitValue |= serSCI_GCR1_NO_PARITY;
//		ulInitValue |= serSCI_GCR1_ASYNC_MODE;
//		serSCI1_GCR1_REGISTER = ulInitValue;
//
//	    /* Calculate Port Baud Rate @ 80mHz */
//		/*  1040 = baud rate   4800 */
//		/*   520 = baud rate   9600 */
//		/*   260 = baud rate  19200 */
//		/*   130 = baud rate  38400 */
//		/*    85 = baud rate  57600 */
//		/*    65 = baud rate  76800 */
//		/*    42 = baud rate 115200 */
//		/* Fractional divider = M, prescaler = P */
//		/* baudrate = clock / ( 16 * ( P + 1 + ( M / 16 ) ) ) */
//
//		/* work out fractional divider and place at bits 27:24 */
//		ulInitValue   = configCPU_CLOCK_HZ;
//		ulInitValue  /= serSCI1_BAUD_RATE;
//		ulInitValue  %= serSCI1_BAUD_SCALING;
//		ulInitValue <<= 24;
//		ulInitValue  &= 0x0F000000;
//
//		/* work out prescaler for bits 23:0 and bitwise-or with fractional divider */
//		ulInitClock  = configCPU_CLOCK_HZ;
//		ulInitClock /= serSCI1_BAUD_RATE;
//		ulInitClock /= serSCI1_BAUD_SCALING;
//		ulInitClock -= 1UL;
//		ulInitClock &= 0x00FFFFFF;
//
//		ulInitClock |= ulInitValue;
//
//		serSCI1_BRS_REGISTER = ulInitClock;
//
//	    /** length 8 data bits */
//		ulInitValue  = serSCI_FORMAT_8_BIT_CHARS;
//		serSCI1_FORMAT_REGISTER = ulInitValue;
//
//	    /** - set SCI pins functional mode */
//		ulInitValue  = serSCI_PIO_0_TX_FUNC;
//		ulInitValue |= serSCI_PIO_0_RX_FUNC;
//		serSCI1_PIO_0_REGISTER = ulInitValue;
//
//	    /** - set SCI pins pullup/pulldown select */
//		ulInitValue  = serSCI_PSL_0_TX_PIN;
//		ulInitValue |= serSCI_PSL_0_RX_PIN;
//		ulInitValue |= serSCI_PSL_0_CK_PIN;
//		serSCI1_PSL_0_REGISTER = ulInitValue;
//
//	    /** - set interrupt level */
//		ulInitValue  = 0;
//	    serSCI1_SET_LVL_REGISTER = ulInitValue;
//
//	    /** - set interrupt enable */
//		ulInitValue  = serSET_INT_RX_BIT;
//		ulInitValue |= serSET_INT_TX_BIT;
//	    serSCI1_SET_INT_REGISTER = ulInitValue;
//
//	    /** - Finally Enable SCI */
//		ulInitValue  = serSCI_GCR1_SWNRST_BIT;
//	    serSCI1_GCR1_REGISTER |= ulInitValue;
//	}
//	portEXIT_CRITICAL();
//
//	return  xInitResult;
//}

/*!
 ****************************************************************************
 * @brief	Serial Test UART Port Operation Control
 ****************************************************************************
 * @param	xCtrlState - Port Control Operation ( ENABLE or DISABLE ).
 ****************************************************************************/
static void  vUARTControl( portBaseType xCtrlState )
{
	switch( xCtrlState )
	{
		case UART_ENABLE:	/*!- USART Operation Enable */
			//serSCI1_GCR1_REGISTER |= serSCI_GCR1_RXENA_BIT;
			//serSCI1_GCR1_REGISTER |= serSCI_GCR1_TXENA_BIT;
			break;

		case UART_DISABLE:	/*!- USART Operation Disable */
			//serSCI1_GCR1_REGISTER &= ~serSCI_GCR1_RXENA_BIT;
			//serSCI1_GCR1_REGISTER &= ~serSCI_GCR1_TXENA_BIT;
			break;

		default:
			break;
	}
}

/*!
 ****************************************************************************
 * @brief	Serial Test UART Port Interrupt Control
 ****************************************************************************
 * @param	xCtrlIrqState - Port Interrupt Control ( ENABLE or DISABLE ).
 ****************************************************************************/
static void  vUARTCtrlIrq( portBaseType xCtrlIrqState )
{
portUInt32Type  ulCtrlIrqValue;

	if( ( xCtrlIrqState >= UART_ENABLE  ) &&
		( xCtrlIrqState <  UART_DISABLE ) )
	{
		/* Ensure VIM Table Interrupt Bit Set */
		//serSCI1_VIM_SET2_REGISTER |= serSCI1_VIM_IRQ_SET;
		serSCI1_VIM_SET0_REGISTER |= serSCI1_VIM_IRQ_SET;
	}

	switch( xCtrlIrqState )
	{
	case UART_ENABLE:
		ulCtrlIrqValue  = serSET_INT_RX_BIT;
		ulCtrlIrqValue |= serSET_INT_TX_BIT;
		serSCI1_SET_INT_REGISTER = ulCtrlIrqValue;
		break;

	case UART_ENABLE_RXR:
		ulCtrlIrqValue = serSET_INT_RX_BIT;
		serSCI1_SET_INT_REGISTER = ulCtrlIrqValue;
		break;

	case UART_ENABLE_TXR:
		ulCtrlIrqValue = serSET_INT_TX_BIT;
		serSCI1_SET_INT_REGISTER = ulCtrlIrqValue;
		break;

	case UART_DISABLE:
		ulCtrlIrqValue  = serCLR_INT_RX_BIT;
		ulCtrlIrqValue |= serCLR_INT_TX_BIT;
		serSCI1_CLR_INT_REGISTER = ulCtrlIrqValue;
		break;

	case UART_DISABLE_RXR:
		ulCtrlIrqValue = serCLR_INT_RX_BIT;
		serSCI1_CLR_INT_REGISTER = ulCtrlIrqValue;
		break;

	case UART_DISABLE_TXR:
		ulCtrlIrqValue = serCLR_INT_TX_BIT;
		serSCI1_CLR_INT_REGISTER = ulCtrlIrqValue;
		break;

	default:
		break;
	}
}
