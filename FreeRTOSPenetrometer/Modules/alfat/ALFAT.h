/**
*****************************************************************************
* File: ALFAT.h											   
* Copyright 2012 GHI Electronics, LLC 								   
* his is a very basic driver to use ALFAT SoC Processor
* For demonstration purposes only. not to be used in production.
*
* THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
* ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
* CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.								   *
************************************************************************
**/

#include <stdio.h>
#include <ctype.h>                    /* character functions                  */
#include <string.h>                   /* string and memory functions          */

#include <stdint.h>

#define SPI_BUS
#ifdef SPI_BUS // using SPI example
	#include "ALFAT_SPI.h"
	#define WriteBytes   	STM32_SPI_WriteBytes
	#define ReadBytes   	STM32_SPI_ReadBytes
	#define OpenBus 		STM32_SPI_Open
	#define ActivePinState 	STM32_SPI_ActivePinState
	#define Disable_Bus_Interrupt	STM32_SPI_DisableInterrupt
	#define Enable_Bus_Interrupt STM32_SPI_EnableInterrupt
#endif


#define ERR_UNEXPECTED 99
#define ERR_NO_ERROR 0
#define DATA_BUFFER_SIZE 1024


void ALFAT_OpenBus(void);
int32_t EncodeStatusCode(uint8_t data[]);
int32_t EncodeErrorCode(uint8_t *data, int32_t index);
int32_t ALFAT_GetVersion(int32_t * major, int32_t *minor, int32_t * build);
int32_t ALFAT_InitializeMedia(uint8_t *drv);
int32_t ALFAT_CloseFile(int32_t handle);
int32_t ALFAT_WriteFile(uint8_t handle,uint8_t* content, uint32_t length);
int32_t ALFAT_OpenFile(uint8_t handle,uint8_t* filename, uint8_t *mode);
int32_t ALFAT_SendCommand(uint8_t *cmd);
int32_t ALFAT_ReadFile(int32_t handle,uint8_t *buffer,int32_t length, uint8_t *filler);
int32_t ALFAT_ReadStatus(uint8_t statbuf[]);
int32_t ALFAT_DeleteFile(uint8_t* filename);
int32_t ALFAT_FlushBanner(void);
int32_t ALFAT_CopyFile(int32_t handle_src,int32_t handle_des,int32_t index, int32_t length);
int32_t ALFAT_FIND_FILE(char path[], uint32_t* file_size);
int32_t ALFAT_FileTell(uint8_t handle);
