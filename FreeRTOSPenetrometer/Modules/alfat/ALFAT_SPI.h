/**
*****************************************************************************
* File: ALFAT_SPI.h
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

#include <stdint.h>


#define MAX_DATA_BUFFER 		1024*1
#define DATA_RX_BUFFER_SIZE 	1024*4
#define DATA_TX_BUFFER_SIZE 	1024*4

#define WRITE_CMD 1
#define READ_CMD  2

//#define TRUE    1
//#define FALSE   0

uint32_t STM32_SPI_ReadBytes(uint8_t *data, uint32_t index, uint32_t length);
uint32_t STM32_SPI_WriteBytes(uint8_t *data, uint32_t index, uint32_t length);
uint32_t STM32_SPI_ActivePinState(void);
uint32_t STM32_SPI_BusyPinState(void);

#define NUM_BLOCK 10000
//#define DMA_SPI_SLAVE_ACTIVE
#ifdef DMA_SPI_SLAVE_ACTIVE
	#define BLOCK_SIZE 8*1024
#else
	#define BLOCK_SIZE 4*1024
#endif
#define USER_TIME_OUT 1000
#define USER_TIME_DELAY 1000

#define ALFAT_SPI_REG 		spiREG3

#define ALFAT_RESET_PIN		27
#define ALFAT_RESET_PORT	hetPORT1
#define ALFAT_RESET_ON		0
#define ALFAT_RESET_OFF		1

#define ALFAT_BUSY_PIN		0
#define ALFAT_BUSY_PORT		spiPORT3
#define ALFAT_BUSY_ON		1
#define ALFAT_BUSY_OFF		0

/* Doesn't do anything */
#define ALFAT_ACTIVE_PIN	25
#define ALFAT_ACTIVE_PORT	hetPORT1
#define ALFAT_ACTIVE_ON		0
#define ALFAT_ACTIVE_OFF	1

#define ALFAT_USB_PWR_PIN	2
#define ALFAT_USB_PWR_PORT	gioPORTA
#define ALFAT_USB_PWR_ON	0
#define ALFAT_USB_PWR_OFF	1

#define ALFAT_SPI_CS_PIN	29
#define ALFAT_SPI_CS_PORT	hetPORT1
#define ALFAT_SPI_CS_ON		0
#define ALFAT_SPI_CS_OFF	1

#define ALFAT_USB_SENS_PIN	3
#define ALFAT_USB_SENS_PORT	gioPORTA
#define ALFAT_USB_SENS_ON	0
#define ALFAT_USB_SENS_OFF	1

#define ALFAT_PWR_PIN		0
#define ALFAT_PWR_PORT		gioPORTA
#define ALFAT_PWR_ON		1
#define ALFAT_PWR_OFF		0


void alfat_delay(uint32_t val);
