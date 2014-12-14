/**
*****************************************************************************
* File: blALFAT.h
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


#include "bl_ALFAT_SPI.h"
#define bl_WriteBytes   	bl_STM32_SPI_WriteBytes
#define bl_ReadBytes   	bl_STM32_SPI_ReadBytes
#define bl_OpenBus 		bl_STM32_SPI_Open
#define bl_ActivePinState 	bl_STM32_SPI_ActivePinState
#define bl_Disable_Bus_Interrupt	bl_STM32_SPI_DisableInterrupt
#define bl_Enable_Bus_Interrupt bl_STM32_SPI_EnableInterrupt


#define ERR_UNEXPECTED 99
#define ERR_NO_ERROR 0
#define DATA_BUFFER_SIZE 1024


void bl_ALFAT_OpenBus(void);
int bl_EncodeStatusCode(unsigned char data[]);
int bl_EncodeErrorCode(unsigned char *data, int index);
int bl_ALFAT_GetVersion(int * major, int *minor, int * build);
int bl_ALFAT_InitializeMedia(unsigned char *drv);
int bl_ALFAT_CloseFile(int handle);
int bl_ALFAT_OpenFile(unsigned char handle,unsigned char* filename, unsigned char *mode);
int bl_ALFAT_SendCommand(unsigned char *cmd);
int bl_ALFAT_ReadFile(int handle,unsigned char *buffer,int length, unsigned char *filler);
int bl_ALFAT_ReadStatus(unsigned char statbuf[]);
int bl_ALFAT_FlushBanner(void);
