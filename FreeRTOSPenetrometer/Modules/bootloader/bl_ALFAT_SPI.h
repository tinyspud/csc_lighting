/**
*****************************************************************************
* File: bl_ALFAT_SPI.h
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



#define MAX_DATA_BUFFER 		1024*1
#define DATA_RX_BUFFER_SIZE 	1024*4
#define DATA_TX_BUFFER_SIZE 	1024*4

#define WRITE_CMD 1
#define READ_CMD  2

unsigned int bl_STM32_SPI_ReadBytes(unsigned char *data, unsigned int index, unsigned int length);
unsigned int bl_STM32_SPI_WriteBytes(unsigned char *data, unsigned int index, unsigned int length);

unsigned int bl_STM32_SPI_ActivePinState(void);
unsigned int bl_STM32_SPI_BusyPinState(void);
void bl_Delay(unsigned int val);

#define NUM_BLOCK 10000
#define BLOCK_SIZE 4*1024
#define USER_TIME_OUT 1000
#define USER_TIME_DELAY 400			/*TODO: optimize delay */

