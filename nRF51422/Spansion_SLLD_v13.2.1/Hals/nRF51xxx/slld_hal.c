/* slld_hal_example.c - SLLD Hardware Abstraction Layer example Code */

/**************************************************************************
* Copyright 2011 Spansion LLC. All Rights Reserved.
*
* This software is owned and published by:
* Spansion LLC, 915 DeGuigne Drive, Sunnyvale, CA 94088 ("Spansion").
*
* BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
* BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
*
* This software constitutes driver source code for use in programming Spansion's
* Flash memory components. This software is licensed by Spansion to be adapted only
* for use in systems utilizing Spansion's Flash memories. Spansion is not be
* responsible for misuse or illegal use of this software for devices not
* supported herein.	Spansion is providing this source code "AS IS" and will
* not be responsible for issues arising from incorrect user implementation
* of the source code herein.
*
* Spansion MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
* REGARDING THE SOFTWARE, ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED
* USE, INCLUDING, WITHOUT LIMITATION, NO IMPLIED WARRANTY OF MERCHANTABILITY,
* FITNESS FOR A	PARTICULAR PURPOSE OR USE, OR NONINFRINGEMENT.	Spansion WILL
* HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT, NEGLIGENCE OR
* OTHERWISE) FOR ANY DAMAGES ARISING FROM USE OR INABILITY TO USE THE SOFTWARE,
* INCLUDING, WITHOUT LIMITATION, ANY DIRECT, INDIRECT, INCIDENTAL,
* SPECIAL, OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA, SAVINGS OR PROFITS,
* EVEN IF Spansion HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
*
* This software may be replicated in part or whole for the licensed use,
* with the restriction that this Copyright notice must be included with
* this software, whether used in part or whole, at all times.
*/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "nrf_delay.h"
#include "app_error.h"
#include "app_util_platform.h"
#include "nrf_gpio.h"
#include "slld_targetspecific.h"
#include "spi_master.h"
#include "slld.h"
#include "slld_hal.h"
#include "LEDs.h"




#define TX_RX_BUFF_LEN		4100
//uint8_t tx_buffer[TX_RX_BUFF_LEN] __attribute__((at(0x20002000)));
//uint8_t rx_buffer[TX_RX_BUFF_LEN] __attribute__((at(0x20003000)));

uint8_t tx_buffer[TX_RX_BUFF_LEN];
uint8_t rx_buffer[TX_RX_BUFF_LEN];

// ***************************************************************************
//	FLASH_READ - HAL read function
//
//	input : device_num						device number to which operation will be done
//					command							 write a single command byte to flash
//					sys_addr							system address to be used
//					data_buffer					 Pointer to the data buffer where to store the read data
//					Number_Of_Read_Bytes	number of bytes to be read
//
//	return value : status of the operation - FAIL or SUCCESS
// ***************************************************************************
SLLD_STATUS FLASH_READ
(
BYTE		 command,												/* write a single command byte to flash */
ADDRESS	sys_addr,											 /* system address to be used */
BYTE		*data_buffer,										/* Pointer to the data buffer containing data to be written */
int			Number_Of_Read_Bytes						/* number of bytes to be read */
)
{
	SLLD_STATUS status = SLLD_OK;
	uint16_t data_cycle, Number_Of_Dummy_Bytes = 0;
	int i = 0;
	uint16_t j = 0;
	// Select the device
	nrf_gpio_pin_clear(EEPROM_CS_PIN);
	LED_TURN_ON(LED_RED);

	uint8_t * spi_tx_data_ptr = &tx_buffer[0];
	uint8_t * spi_rx_data_ptr = &rx_buffer[0];
	for (i = 0; i < TX_RX_BUFF_LEN; i++){
		tx_buffer[i] = 0;
		rx_buffer[i] = 0;
	}

	// Write the command
	// TODO add data to SPI buffer
	//		SPIDATA = command;
	*spi_tx_data_ptr = command;
	j++;

	// Write the address
	if (sys_addr != ADDRESS_NOT_USED)
	{
		switch (command)
		{
		default:
			{

				for (i = 2; i >= 0; i--, j++)
					*(spi_tx_data_ptr + j) = (uint8_t)(sys_addr >> (8 * i) & 0x000000FF);
				/*
								SPIDATA = (BYTE)((sys_addr >> 16) & 0x000000FF);
								SPIDATA = (BYTE)((sys_addr >>	8) & 0x000000FF);
								SPIDATA = (BYTE) (sys_addr				& 0x000000FF);
								*/
				break;
			}
		}
	}

	// Write the dummy bytes
	switch (command)
	{
	case SPI_FAST_READ_CMD:
	case SPI_FAST_READ_4B_CMD:
	case SPI_DUALIO_RD_CMD:
	case SPI_DUALIO_RD_4B_CMD:
	case SPI_QUADIO_RD_CMD:
	case SPI_QUADIO_RD_4B_CMD:
	case SPI_OTPR_CMD:
		{
			Number_Of_Dummy_Bytes = 1;
			// Write a dummy byte to the data bus
			/*
									for (data_cycle = 0; data_cycle < Number_Of_Dummy_Bytes; data_cycle++)
									SPIDATA = 0x0;
									*/
			for (data_cycle = 0; data_cycle < Number_Of_Dummy_Bytes; data_cycle++)
				*(spi_tx_data_ptr + data_cycle) = 0x00;

			break;
		}
	case SPI_DUALIO_HPRD_CMD:
	case SPI_DUALIO_HPRD_4B_CMD:
		{
			Number_Of_Dummy_Bytes = 1;
			/*
											// Write a dummy byte to the data bus - This is actually the mode bit
											for (data_cycle = 0; data_cycle < Number_Of_Dummy_Bytes; data_cycle++)
											SPIDATA_D = 0x0;
											*/
			for (data_cycle = 0; data_cycle < Number_Of_Dummy_Bytes; data_cycle++)
				*(spi_tx_data_ptr + data_cycle) = 0x00;
			break;
		}
	case SPI_QUADIO_HPRD_CMD:
	case SPI_QUADIO_HPRD_4B_CMD:
		{
			Number_Of_Dummy_Bytes = 3;
			/*
											// Write the dummy bytes to the data bus - The first byte is actually the mode bit
											for (data_cycle = 0; data_cycle < Number_Of_Dummy_Bytes; data_cycle++)
											SPIDATA_Q = 0x0;
											*/
			for (data_cycle = 0; data_cycle < Number_Of_Dummy_Bytes; data_cycle++)
				*(spi_tx_data_ptr + data_cycle) = 0x00;
			break;
		}
	case SPI_RES_CMD:
		{
			Number_Of_Dummy_Bytes = 3;
			/*
			// Write the dummy bytes to the data bus
			for (data_cycle = 0; data_cycle < Number_Of_Dummy_Bytes; data_cycle++)
			SPIDATA = 0x0;
			*/
			for (data_cycle = 0; data_cycle < Number_Of_Dummy_Bytes; data_cycle++)
				*(spi_tx_data_ptr + data_cycle) = 0x00;
			break;
		}
	default:
		{
			Number_Of_Dummy_Bytes = 0;
			break;
		}
	}

	/* Do the read/write here then offload it into the answer */
	spi_master_send_recv(SPI_MASTER_0, spi_tx_data_ptr, (j + Number_Of_Dummy_Bytes), spi_rx_data_ptr, (j + Number_Of_Dummy_Bytes) + Number_Of_Read_Bytes);

	// Read the data
	if (Number_Of_Read_Bytes != 0)
	{
		switch (command)
		{
		case SPI_DUALIO_RD_CMD:
		case SPI_DUALIO_RD_4B_CMD:
		case SPI_DUALIO_HPRD_CMD:
		case SPI_DUALIO_HPRD_4B_CMD:
			{
				// TODO add data to buffer
				/*
													// Read the data using the relevant mode
													for (data_cycle = 0; data_cycle < Number_Of_Read_Bytes; data_cycle++)
													*(data_buffer + data_cycle) = SPIDATA_D;
													*/
				for(data_cycle = 0; data_cycle < Number_Of_Read_Bytes; data_cycle++)
					*(data_buffer + data_cycle) = *(spi_rx_data_ptr + data_cycle + (j + Number_Of_Dummy_Bytes));
				break;
			}
		case SPI_QUADIO_RD_CMD:
		case SPI_QUADIO_RD_4B_CMD:
		case SPI_QUADIO_HPRD_CMD:
		case SPI_QUADIO_HPRD_4B_CMD:
			{
				// TODO add data to buffer
				/*
													// Read the data using the relevant mode
													for (data_cycle = 0; data_cycle < Number_Of_Read_Bytes; data_cycle++)
													*(data_buffer + data_cycle) = SPIDATA_Q;
													*/
				for(data_cycle = 0; data_cycle < Number_Of_Read_Bytes; data_cycle++)
					*(data_buffer + data_cycle) = *(spi_rx_data_ptr + data_cycle + (j + Number_Of_Dummy_Bytes));
				break;
			}
		default:
			{
				// TODO add data to buffer
				/*
								// Read the data using the relevant mode
								for (data_cycle = 0; data_cycle < Number_Of_Read_Bytes; data_cycle++)
								*(data_buffer + data_cycle) = SPIDATA;
								*/
				for(data_cycle = 0; data_cycle < Number_Of_Read_Bytes; data_cycle++)
					*(data_buffer + data_cycle) = *(spi_rx_data_ptr + data_cycle + (j + Number_Of_Dummy_Bytes));
				break;
			}
		}
	}

	// Deselect the device
	nrf_gpio_pin_set(EEPROM_CS_PIN);
	LED_TURN_OFF(LED_RED);

	return(status);
}


// ***************************************************************************
//	FLASH_WRITE - HAL write function
//
//	input : device_num							 device number to which operation will be done
//					command									write a single command byte to flash
//					sys_addr								 system address to be used
//					data_buffer							Pointer to the data buffer where to store the written data
//					Number_Of_Written_Bytes	number of bytes to be written
//
//	return value : status of the operation - FAIL or SUCCESS
// ***************************************************************************
SLLD_STATUS FLASH_WRITE
(
BYTE		 command,												/* write a single command byte to flash */
ADDRESS	sys_addr,											 /* system address to be used */
BYTE		*data_buffer,										/* Pointer to the data buffer containing data to be written */
int			Number_Of_Written_Bytes				 /* number of bytes to be written */
)
{
	SLLD_STATUS status = SLLD_OK;
	uint16_t i = 0;
	uint16_t j = 0;

	uint8_t * spi_tx_data_ptr = &tx_buffer[0];
	uint8_t * spi_rx_data_ptr = &rx_buffer[0];
	for (i = 0; i < TX_RX_BUFF_LEN; i++){
		tx_buffer[i] = 0;
		rx_buffer[i] = 0;
	}

	// Select the device
	nrf_gpio_pin_clear(EEPROM_CS_PIN);
	LED_TURN_ON(LED_RED);

	// Write the command
	*(spi_tx_data_ptr) = command;
	j++;

	// Write the address
	if (sys_addr != ADDRESS_NOT_USED)
	{
		// TODO add spi data tx
		*(spi_tx_data_ptr + j) = command;
		j++;

		for (i = 2; i >= 0; i--, j++)
		*(spi_tx_data_ptr + j) = (uint8_t)(sys_addr >> (8 * i) & 0x000000FF);

		/*
			SPIDATA = (BYTE)((sys_addr >> 16) & 0x000000FF);
			SPIDATA = (BYTE)((sys_addr >>	8) & 0x000000FF);
			SPIDATA = (BYTE) (sys_addr				& 0x000000FF);
			*/
	}

	// Write the data
	if (Number_Of_Written_Bytes != 0)
	{
		switch (command)
		{
		case SPI_QPP_CMD:
		case SPI_QPP_4B_CMD:
			{
				// Write the data using the relevant mode
				// TODO decypher line
				/*
											for (data_cycle = 0; data_cycle < Number_Of_Written_Bytes; data_cycle++)
											SPIDATA_Q = *(data_buffer + data_cycle);
											*/
				break;
			}
		default:
			{
				// Write the data using the relevant mode
				// TODO decypher line
				/*								for (data_cycle = 0; data_cycle < Number_Of_Written_Bytes; data_cycle++)
										SPIDATA = *(data_buffer + data_cycle);
										*/
				for (i = 0; i < Number_Of_Written_Bytes; i++, j++)
				*(spi_tx_data_ptr + j) = *(data_buffer + i);

				break;
			}
		}
	}
	/* Do the read/write here then offload it into the answer */
	spi_master_send_recv(SPI_MASTER_0, spi_tx_data_ptr, j, spi_rx_data_ptr, 0);

	// Deselect the device
	nrf_gpio_pin_set(EEPROM_CS_PIN);
	LED_TURN_OFF(LED_RED);

	return(status);
}


/*****************************************************************************/

/**@brief Handler for SPI0 master events.
*
* @param[in] spi_master_evt		SPI master event.
*/
void spi_master_event_handler(spi_master_evt_t spi_master_evt)
{
	switch (spi_master_evt.evt_type)
	{
		case SPI_MASTER_EVT_TRANSFER_STARTED:
		case SPI_MASTER_EVT_TYPE_MAX:
		case SPI_MASTER_EVT_TRANSFER_COMPLETED:
			//Data transmission is ended successful. 'rx_buffer' has data received from SPI slave.
			break;
		default:
			//No implementation needed.
			break;
	}
}



/**@brief Function for initializing a SPI master driver.
*
* @param[in] spi_master_instance       An instance of SPI master module.
* @param[in] spi_master_event_handler  An event handler for SPI master events.
* @param[in] lsb                       Bits order LSB if true, MSB if false.
*/
void spi_master_init(spi_master_hw_instance_t   spi_master_instance,
spi_master_event_handler_t spi_master_event_handler_arg,
const bool                 lsb)
{
	uint32_t err_code = NRF_SUCCESS;

	// Configure SPI master.
	spi_master_config_t spi_config = SPI_MASTER_INIT_DEFAULT;

	switch (spi_master_instance)
	{
		#ifdef SPI_MASTER_0_ENABLE
	case SPI_MASTER_0:
		{
			spi_config.SPI_Pin_SCK  = SPIM0_SCK_PIN;
			spi_config.SPI_Pin_MISO = SPIM0_MISO_PIN;
			spi_config.SPI_Pin_MOSI = SPIM0_MOSI_PIN;
			spi_config.SPI_Pin_SS   = SPIM0_SS_PIN;
		}
		break;
		#endif /* SPI_MASTER_0_ENABLE */

		#ifdef SPI_MASTER_1_ENABLE
	case SPI_MASTER_1:
		{
			spi_config.SPI_Pin_SCK  = SPIM1_SCK_PIN;
			spi_config.SPI_Pin_MISO = SPIM1_MISO_PIN;
			spi_config.SPI_Pin_MOSI = SPIM1_MOSI_PIN;
			spi_config.SPI_Pin_SS   = SPIM1_SS_PIN;
		}
		break;
		#endif /* SPI_MASTER_1_ENABLE */

	default:
		break;
	}

	spi_config.SPI_CONFIG_ORDER = (lsb ? SPI_CONFIG_ORDER_LsbFirst : SPI_CONFIG_ORDER_MsbFirst);

	err_code = spi_master_open(spi_master_instance, &spi_config);
	APP_ERROR_CHECK(err_code);

	// Register event handler for SPI master.
	spi_master_evt_handler_reg(spi_master_instance, spi_master_event_handler_arg);
}


void spi_EEPROM_init(){

	/* set up the /CS to output */
	nrf_gpio_cfg_output(EEPROM_CS_PIN);
	/* set up the hold to output */
	nrf_gpio_cfg_output(EEPROM_RST_PIN);
	/* set the write protect to output */
	nrf_gpio_cfg_output(EEPROM_WP_PIN);
	/* configure pins for SPI (master) */
	
	/* set CS high */
	nrf_gpio_pin_set(EEPROM_CS_PIN);
	/* set hold high */
	nrf_gpio_pin_set(EEPROM_RST_PIN);
	/* set WP high */
	nrf_gpio_pin_set(EEPROM_WP_PIN);

	spi_master_init(SPI_MASTER_0, &spi_master_event_handler, false);


}




