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

#include "ALFAT.h"
#include "gio.h"
#include "spi.h"
#include "het.h"
#include "ALFAT_SPI.h"


//#define DEBUG_ALFAT
#ifdef DEBUG_ALFAT
#include "sci_common.h"
#endif

uint8_t SPI_DATA_TX_BUFFER[DATA_TX_BUFFER_SIZE];
int32_t spi_tx_fifo_count;
int32_t spi_tx_fifo_in ;
int32_t spi_tx_fifo_out ;
uint8_t SPI_DATA_RX_BUFFER[DATA_RX_BUFFER_SIZE];
int32_t spi_rx_fifo_count;
int32_t spi_rx_fifo_in ;
int32_t spi_rx_fifo_out ;
uint8_t spi_rx_data;


spiDAT1_t dataconfig1_t;


int32_t ALFAT_FlushBanner(void);

void alfat_delay(uint32_t val)
{
	uint16_t i,k;

	for(i=0;i<val; i++){
		for(k=0; k<val; k++){
			_nop();
		}
	}
}





// This function will write data to file.
// It will process command 'L' or 'W' depend on is_interrupt_enable variable.
// If is_interrupt_enable is on, that means master use SPIx inetrrupt for sending.
// If is_interrupt_enable is of, that means master send data directly by SPIx->DR.
uint32_t STM32_SPI_WriteBytes(uint8_t data[], uint32_t index, uint32_t length)
{
	 int32_t start =index ;
	 int32_t end = length + index;
	 int32_t timeout =	USER_TIME_OUT;
	 uint16_t tbuf[3];
	 uint16_t rbuf[3];

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
	 while ( rbuf[1] != TRUE && timeout>0){
		 while(	STM32_SPI_BusyPinState());
		 gioSetBit(ALFAT_SPI_CS_PORT, ALFAT_SPI_CS_PIN, ALFAT_SPI_CS_ON);
		 spiTransmitAndReceiveData(ALFAT_SPI_REG, &dataconfig1_t, 2, &tbuf[0], &rbuf[0]);
		 gioSetBit(ALFAT_SPI_CS_PORT, ALFAT_SPI_CS_PIN, ALFAT_SPI_CS_OFF);
		 if (rbuf[1] != TRUE){
			alfat_delay(1);
			timeout--;
		 }
	 }

	 // Not timeout
	 if ( timeout!=0) {

		 // Send the 3rd byte
		 gioSetBit(ALFAT_SPI_CS_PORT, ALFAT_SPI_CS_PIN, ALFAT_SPI_CS_ON);
		 spiTransmitAndReceiveData(ALFAT_SPI_REG, &dataconfig1_t, 1, &tbuf[2],  &rbuf[3]);
		 gioSetBit(ALFAT_SPI_CS_PORT, ALFAT_SPI_CS_PIN, ALFAT_SPI_CS_OFF);

		 while( start<	end)
		 {
			 // Wait while ALFAT is busy
			 while(	STM32_SPI_BusyPinState());
			 tbuf[0] = data[start++];

			 gioSetBit(ALFAT_SPI_CS_PORT, ALFAT_SPI_CS_PIN, ALFAT_SPI_CS_ON);
			 spiTransmitAndReceiveData(ALFAT_SPI_REG, &dataconfig1_t, 1, &tbuf[0], &rbuf[0]);
			 gioSetBit(ALFAT_SPI_CS_PORT, ALFAT_SPI_CS_PIN, ALFAT_SPI_CS_OFF);


#ifdef DEBUG_ALFAT
UART_putChar(UART, (uint8_t) tbuf[0]);
#endif
		 }
	 }

	 return (start-index);
}
//////////// SPI Read bytes ////////////////////////
uint32_t STM32_SPI_ReadBytes(uint8_t data[], uint32_t index, uint32_t length)
{
	 int32_t actual_size=0;
	 int32_t start =index ;
	 int32_t end;
	 uint16_t tbuf[3];
	 uint16_t rbuf[3];


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
	 gioSetBit(ALFAT_SPI_CS_PORT, ALFAT_SPI_CS_PIN, ALFAT_SPI_CS_ON);
	 spiTransmitAndReceiveData(ALFAT_SPI_REG, &dataconfig1_t, 3, &tbuf[0], &rbuf[0]);
	 gioSetBit(ALFAT_SPI_CS_PORT, ALFAT_SPI_CS_PIN, ALFAT_SPI_CS_OFF);



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

		 gioSetBit(ALFAT_SPI_CS_PORT, ALFAT_SPI_CS_PIN, ALFAT_SPI_CS_ON);
		spiTransmitAndReceiveData(ALFAT_SPI_REG, &dataconfig1_t, 1, &tbuf[0], &rbuf[0]);
		 gioSetBit(ALFAT_SPI_CS_PORT, ALFAT_SPI_CS_PIN, ALFAT_SPI_CS_OFF);
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
uint32_t STM32_SPI_ActivePinState()
{
	return gioGetBit(ALFAT_ACTIVE_PORT, ALFAT_ACTIVE_PIN);
}
////////////// STM32_SPI_BusyPinState /////////////////
// Read status GPIO busy pin.
// Return high is busy, low is not busy
// Re-define this function depend on system
/////////////////////////////////////////////////////////
uint32_t STM32_SPI_BusyPinState()
{
	return gioGetBit(ALFAT_BUSY_PORT, ALFAT_BUSY_PIN);
}



