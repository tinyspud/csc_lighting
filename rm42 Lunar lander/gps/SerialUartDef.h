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
 * @file   SerialUartDef.h
 * @date   November 2012
 ****************************************************************************
 * @note   Serial UART Port Interface Definitions
  ****************************************************************************
 * @note   Project Dependent - Modify for Individual Project Requirements
 ****************************************************************************/

#ifndef  SERIAL_UART_DEF_H
#define  SERIAL_UART_DEF_H


/*-- Public MACRO Definitions ----------------------------------------------*/


/*-- Public CONSTANT Definitions -------------------------------------------*/

/*!- Serial UART Port Register Bit Definitions */

#define serSCI_GCR0_RESET_BIT				( ( portUInt32Type ) 0x00000001 )

#define serSCI_GCR1_TXENA_BIT				( ( portUInt32Type ) 0x02000000 )
#define serSCI_GCR1_RXENA_BIT				( ( portUInt32Type ) 0x01000000 )
#define serSCI_GCR1_CONT_BIT				( ( portUInt32Type ) 0x00020000 )
#define serSCI_GCR1_LOOPBACK_BIT			( ( portUInt32Type ) 0x00010000 )
#define serSCI_GCR1_MBUF_MODE_BIT			( ( portUInt32Type ) 0x00000000 )
#define serSCI_GCR1_SWNRST_BIT				( ( portUInt32Type ) 0x00000080 )
#define serSCI_GCR1_CLOCK_BIT				( ( portUInt32Type ) 0x00000020 )
#define serSCI_GCR1_1_STOP_BIT				( ( portUInt32Type ) 0x00000000 )
#define serSCI_GCR1_NO_PARITY				( ( portUInt32Type ) 0x00000000 )
#define serSCI_GCR1_ASYNC_MODE				( ( portUInt32Type ) 0x00000002 )

#define serCLR_INT_RX_BIT					( ( portUInt32Type ) 0x00000200 )
#define serCLR_INT_TX_BIT					( ( portUInt32Type ) 0x00000100 )

#define serFLR_RDY_RX_BIT					( ( portUInt32Type ) 0x00000200 )
#define serFLR_RDY_TX_BIT					( ( portUInt32Type ) 0x00000100 )
#define serFLR_EMP_TX_BIT					( ( portUInt32Type ) 0x00000800 )

#define serSET_INT_RX_BIT					( ( portUInt32Type ) 0x00000200 )
#define serSET_INT_TX_BIT					( ( portUInt32Type ) 0x00000100 )

#define serSRC_INT_RECEIVE					( ( portUInt32Type ) 11 )
#define serSRC_INT_TRANSMIT					( ( portUInt32Type ) 12 )

#define serSCI_FORMAT_7_BIT_CHARS			( ( portUInt32Type ) 0x00000006 )
#define serSCI_FORMAT_8_BIT_CHARS			( ( portUInt32Type ) 0x00000007 )

#define serSCI_DATA_MASK					( ( portUInt32Type ) 0x000000FF )

#define serSCI_PIO_0_TX_FUNC				( ( portUInt32Type ) 0x00000004 )
#define serSCI_PIO_0_RX_FUNC				( ( portUInt32Type ) 0x00000002 )

#define serSCI_PSL_0_CK_PIN					( ( portUInt32Type ) 0x00000001 )
#define serSCI_PSL_0_RX_PIN					( ( portUInt32Type ) 0x00000002 )
#define serSCI_PSL_0_TX_PIN					( ( portUInt32Type ) 0x00000004 )

#define serSCI_FLR_PE_BIT					( ( portUInt32Type ) 0x01000000 )
#define serSCI_FLR_OE_BIT					( ( portUInt32Type ) 0x02000000 )
#define serSCI_FLR_FE_BIT					( ( portUInt32Type ) 0x04000000 )





/*!- SCI2/LIN Baud Rate Clock Definitions */
#define serSCI1_BAUD_RATE					( ( portUInt32Type ) 115200 )
#define serSCI1_BAUD_SCALING				( ( portUInt32Type ) 59 )

/*!- SCI2/LIN VIM Table (SCI-1) Register Definitions */
#define serSCI1_VIM_IRQ_BIT				( ( portUInt32Type ) 0x00000001 )
#define serSCI1_VIM_IRQ_CLR				( ~serSCI1_VIM_IRQ_BIT )
#define serSCI1_VIM_IRQ_SET				(  serSCI1_VIM_IRQ_BIT )


#define serSCI1_VIM_SET0_REGISTER			( *( volatile portUInt32Type * ) 0xFFFFFE38 )
#define serSCI1_VIM_CLR0_REGISTER			( *( volatile portUInt32Type * ) 0xFFFFFE48 )

/*!- SCI2/LIN Register Definitions */
#define serSCI1_GCR0_REGISTER				( *( volatile portUInt32Type * ) 0xFFF7E500 )
#define serSCI1_GCR1_REGISTER				( *( volatile portUInt32Type * ) 0xFFF7E504 )
#define serSCI1_SET_INT_REGISTER			( *( volatile portUInt32Type * ) 0xFFF7E50C )
#define serSCI1_CLR_INT_REGISTER			( *( volatile portUInt32Type * ) 0xFFF7E510 )
#define serSCI1_SET_LVL_REGISTER			( *( volatile portUInt32Type * ) 0xFFF7E514 )
#define serSCI1_CLR_LVL_REGISTER			( *( volatile portUInt32Type * ) 0xFFF7E518 )
#define serSCI1_FLR_INT_REGISTER			( *( volatile portUInt32Type * ) 0xFFF7E51C )
#define serSCI1_INT_VECT_0_REGISTER		( *( volatile portUInt32Type * ) 0xFFF7E520 )
#define serSCI1_INT_VECT_1_REGISTER		( *( volatile portUInt32Type * ) 0xFFF7E524 )
#define serSCI1_FORMAT_REGISTER			( *( volatile portUInt32Type * ) 0xFFF7E528 )
#define serSCI1_BRS_REGISTER				( *( volatile portUInt32Type * ) 0xFFF7E52C )
#define serSCI1_RD_REGISTER				( *( volatile portUInt32Type * ) 0xFFF7E534 )
#define serSCI1_TD_REGISTER				( *( volatile portUInt32Type * ) 0xFFF7E538 )
#define serSCI1_PIO_0_REGISTER				( *( volatile portUInt32Type * ) 0xFFF7E53C )
#define serSCI1_PSL_0_REGISTER				( *( volatile portUInt32Type * ) 0xFFF7E55C )





/*!- SCI 1 Baud Rate Clock Definitions */
#define serSCILIN_BAUD_RATE					( ( portUInt32Type ) 38400)
#define serSCILIN_BAUD_SCALING				( ( portUInt32Type ) 16 )

/*!- SCI 1 VIM Table (SCI-2) Register Definitions */
#define serSCILIN_VIM_IRQ_BIT				( ( portUInt32Type ) 0x00002000 )
#define serSCILIN_VIM_IRQ_CLR				( ~serSCILIN_VIM_IRQ_BIT )
#define serSCILIN_VIM_IRQ_SET				(  serSCILIN_VIM_IRQ_BIT )


#define serSCILIN_VIM_SET2_REGISTER			( *( volatile portUInt32Type * ) 0xFFFFFE30 )
#define serSCILIN_VIM_CLR2_REGISTER			( *( volatile portUInt32Type * ) 0xFFFFFE40 )

/*!- SCI 1 UART Port (SCI-2) Register Definitions */
#define serSCILIN_GCR0_REGISTER				( *( volatile portUInt32Type * ) 0xFFF7E400 )
#define serSCILIN_GCR1_REGISTER				( *( volatile portUInt32Type * ) 0xFFF7E404 )
#define serSCILIN_SET_INT_REGISTER			( *( volatile portUInt32Type * ) 0xFFF7E40C )
#define serSCILIN_CLR_INT_REGISTER			( *( volatile portUInt32Type * ) 0xFFF7E410 )
#define serSCILIN_SET_LVL_REGISTER			( *( volatile portUInt32Type * ) 0xFFF7E414 )
#define serSCILIN_CLR_LVL_REGISTER			( *( volatile portUInt32Type * ) 0xFFF7E418 )
#define serSCILIN_FLR_INT_REGISTER			( *( volatile portUInt32Type * ) 0xFFF7E41C )
#define serSCILIN_INT_VECT_0_REGISTER		( *( volatile portUInt32Type * ) 0xFFF7E420 )
#define serSCILIN_INT_VECT_1_REGISTER		( *( volatile portUInt32Type * ) 0xFFF7E424 )
#define serSCILIN_FORMAT_REGISTER			( *( volatile portUInt32Type * ) 0xFFF7E428 )
#define serSCILIN_BRS_REGISTER				( *( volatile portUInt32Type * ) 0xFFF7E42C )
#define serSCILIN_RD_REGISTER				( *( volatile portUInt32Type * ) 0xFFF7E434 )
#define serSCILIN_TD_REGISTER				( *( volatile portUInt32Type * ) 0xFFF7E438 )
#define serSCILIN_PIO_0_REGISTER				( *( volatile portUInt32Type * ) 0xFFF7E43C )
#define serSCILIN_PSL_0_REGISTER				( *( volatile portUInt32Type * ) 0xFFF7E45C )


#endif  /* SERILA_UART_DEF_H */
