/**
*****************************************************************************
* File: ALFAT_SPI.c												   
* Copyright 2012 GHI Electronics, LLC 								   
* These are basic configurations to to access ALFAT SoC Processor through SPI interface.
* For demonstration purposes only. not to be used in production.
*
* THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
* ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
* CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.								   *
************************************************************************
**/

#include "bl_ALFAT_SPI.h"
#include "bl_ALFAT.h"
#include "bl_gio.h"
#include "bl_spi.h"
#include "bl_het.h"
//#include "Tasks.h"
#include "bootloader.h"

//#define DEBUG_ALFAT
#ifdef DEBUG_ALFAT
#include "sci_common.h"
#endif

//static unsigned char SPI_DATA_TX_BUFFER[DATA_TX_BUFFER_SIZE];
//static int spi_tx_fifo_count;
//static int spi_tx_fifo_in ;
//static int spi_tx_fifo_out ;
//static unsigned char SPI_DATA_RX_BUFFER[DATA_RX_BUFFER_SIZE];
//static int spi_rx_fifo_count;
//static int spi_rx_fifo_in ;
//static int spi_rx_fifo_out ;
//static unsigned char spi_rx_data;

//#pragma DATA_SECTION( bl_dataconfig1_t, ".fapi" );
bl_spiDAT1_t bl_dataconfig1_t;


void bl_nop(void)
{
	asm(" nop");
}

int bl_ALFAT_FlushBanner(void);

void bl_Delay(unsigned int val)
{
	unsigned short i,k;
	//unsigned char temp;

	for(i=0;i<val; i++){
		for(k=0; k<val; k++){
			bl_nop();
			//asm("_nop");
			//asm(" nop");
			//temp=2;
		}
	}
}


//
//unsigned short bl_alfat_spi_init(void)
//{
//
//	/*make sure gios are set correctly */
//	/*set reset output pin, bit6, port A */
//	bl_gioPORTA->DIR |= (1U << 6U);
//
//	//gioSetBit(bl_gioPORTA, 6, 0);
//	bl_gioPORTA->DCLR = 1U << 6;
//
//	/*set ready input pin, bit 0, port A */
//	bl_gioPORTA->DIR &= 0xfe;
//
//	/*set busy input pin, but 7, port A */
//	bl_gioPORTA->DIR &= 0x7f;
//
//
//	//release from reset
//	bl_gioPORTA->DSET = 1U << 6;
//	//gioSetBit(gioPORTA, 6, 1);
//
//	bl_spiInit();
//	//TODO: replace with timer
//	bl_Delay(1000);
//
//	/* Activate SIMO and SCS[0]*/
//	bl_spiREG2->PC0  |= (1U);  /* SCS[0] */
//	bl_spiREG2->PC0  |= (1U << 10U);  /* SIMO */
//
//	return BOOT_PASS;
//}
//


// This function will write data to file.
// It will process command 'L' or 'W' depend on is_interrupt_enable variable.
// If is_interrupt_enable is on, that means master use SPIx inetrrupt for sending.
// If is_interrupt_enable is of, that means master send data directly by SPIx->DR.
unsigned int bl_STM32_SPI_WriteBytes(unsigned char data[], unsigned int index, unsigned int length)
{
	 int start =index ;
	 int end = length + index;
	 int timeout =	USER_TIME_OUT;
	 unsigned short tbuf[3];
	 unsigned short rbuf[3];

	 // Add WRITE_CMD (1) to header
	 tbuf[0] =  WRITE_CMD;
	 //  Add size LSB
	 tbuf[1] =  length & 0xFF;
	 //  Add size MSB
	 tbuf[2] = (length>>8) & 0xFF;
	 rbuf[1] = 0;
	 // Wait for ALFAT is ready
	 // Send frame for write
	 // Add timeout
	 while ( rbuf[1] != 1 && timeout>0){
		 while(	bl_STM32_SPI_BusyPinState());
		 bl_hetPORT1 ->DCLR = 1U << 29;
		 //gioSetBit(ALFAT_SPI_CS_PORT, ALFAT_SPI_CS_PIN, ALFAT_SPI_CS_ON);
		 bl_spiTransmitAndReceiveData(bl_spiREG3, &bl_dataconfig1_t, 2, &tbuf[0], &rbuf[0]);
		 bl_hetPORT1 ->DSET = 1U <<29;
		 //gioSetBit(ALFAT_SPI_CS_PORT, ALFAT_SPI_CS_PIN, ALFAT_SPI_CS_OFF);
		 if (rbuf[1] != 1){
			bl_Delay(1);
			timeout--;
		 }
	 }

	 // Not timeout
	 if ( timeout!=0) {

		 // Send the 3rd byte
		 bl_hetPORT1 ->DCLR = 1U << 29;
		 //gioSetBit(ALFAT_SPI_CS_PORT, ALFAT_SPI_CS_PIN, ALFAT_SPI_CS_ON);
		 bl_spiTransmitAndReceiveData(bl_spiREG3, &bl_dataconfig1_t, 1, &tbuf[2],  &rbuf[3]);
		 bl_hetPORT1 ->DSET = 1U <<29;
		 //gioSetBit(ALFAT_SPI_CS_PORT, ALFAT_SPI_CS_PIN, ALFAT_SPI_CS_OFF);

		 while( start<	end)
		 {
			 // Wait while ALFAT is busy
			 while(	bl_STM32_SPI_BusyPinState());
			 tbuf[0] = data[start++];

			 bl_hetPORT1 ->DCLR = 1U << 29;
			 //gioSetBit(ALFAT_SPI_CS_PORT, ALFAT_SPI_CS_PIN, ALFAT_SPI_CS_ON);
			 bl_spiTransmitAndReceiveData(bl_spiREG3, &bl_dataconfig1_t, 1, &tbuf[0], &rbuf[0]);
			 bl_hetPORT1 ->DSET = 1U <<29;
			 //gioSetBit(ALFAT_SPI_CS_PORT, ALFAT_SPI_CS_PIN, ALFAT_SPI_CS_OFF);


#ifdef DEBUG_ALFAT
UART_putChar(UART, (uint8_t) tbuf[0]);
#endif
		 }
	 }

	 return (start-index);
}
//////////// SPI Read bytes ////////////////////////
unsigned int bl_STM32_SPI_ReadBytes(unsigned char data[], unsigned int index, unsigned int length)
{
	 int actual_size=0;
	 int start =index ;
	 int end;
	 unsigned short tbuf[3];
	 unsigned short rbuf[3];


#ifdef DEBUG_ALFAT
UART_putChar(UART, '\r');
UART_putChar(UART, '\n');
#endif

	 // Prepare frame header
	 // Add READ_CMD (2) to header
	 tbuf[0] =  READ_CMD;
	 //  Add size LSB
	 tbuf[1] =  length & 0xFF;
	 //  Add size MSB
	 tbuf[2] = (length>>8) & 0xFF;

	 // Send 3 bytes Header
	 // Read  3 bytes header back
	 bl_hetPORT1 ->DCLR = 1U << 29;
	 //gioSetBit(ALFAT_SPI_CS_PORT, ALFAT_SPI_CS_PIN, ALFAT_SPI_CS_ON);
	 bl_spiTransmitAndReceiveData(bl_spiREG3, &bl_dataconfig1_t, 3, &tbuf[0], &rbuf[0]);
	 bl_hetPORT1 ->DSET = 1U <<29;
	 //gioSetBit(ALFAT_SPI_CS_PORT, ALFAT_SPI_CS_PIN, ALFAT_SPI_CS_OFF);



	 // Get actual size
	 actual_size = 	(rbuf[1] | (rbuf[2]<<8));
	 // Check actual size
	 if (length > actual_size)
	 {
	 	length =  actual_size;
	 }
	 // Update end
	 end = 	length + index;
	 // Send 0x00
	 tbuf[0] = 0;
	 while(start<end)
	 {
		bl_hetPORT1 ->DCLR = 1U << 29;
		//gioSetBit(ALFAT_SPI_CS_PORT, ALFAT_SPI_CS_PIN, ALFAT_SPI_CS_ON);
		bl_spiTransmitAndReceiveData(bl_spiREG3, &bl_dataconfig1_t, 1, &tbuf[0], &rbuf[0]);
		bl_hetPORT1 ->DSET = 1U << 29;
		//gioSetBit(ALFAT_SPI_CS_PORT, ALFAT_SPI_CS_PIN, ALFAT_SPI_CS_OFF);
		data[start++] = rbuf[0];


#ifdef DEBUG_ALFAT
UART_putChar(UART, (uint8_t)rbuf[0]);
#endif

	 }
	 return (start-index);
}
////////////// STM32_SPI_ActivePinState /////////////////
// Read status GPIO active pin.
// Return high is active, low is not active
// Re-define this function depend on system
/////////////////////////////////////////////////////////
unsigned int bl_STM32_SPI_ActivePinState()
{
	return (bl_hetPORT1->DIN >> 25) & 1U;
	//return gioGetBit(ALFAT_ACTIVE_PORT, ALFAT_ACTIVE_PIN);
	//return (bl_gioPORTA->DIN >> 0) & 1U;
	//return gioGetBit(gioPORTA,0);
}
////////////// STM32_SPI_BusyPinState /////////////////
// Read status GPIO busy pin.
// Return high is busy, low is not busy
// Re-define this function depend on system
/////////////////////////////////////////////////////////
unsigned int bl_STM32_SPI_BusyPinState()
{
	return (bl_spiPORT3->DIN >> 0) & 1U;
	//return gioGetBit(ALFAT_BUSY_PORT, ALFAT_BUSY_PIN);
	//return (bl_gioPORTA->DIN >> 7) & 1U;
	//return gioGetBit(gioPORTA,7);
}




