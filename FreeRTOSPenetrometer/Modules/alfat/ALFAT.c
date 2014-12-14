/**
 *****************************************************************************
 * File: ALFAT.c
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
#include "FreeRTOS.h"
#include "ALFAT.h"
#include "ALFAT_SPI.h"
#include <stdbool.h>
#include "Semaphores.h"

#define ALFAT_COMMAND_LENGTH 	(256)
// alfat_delay function.
//extern void alfat_delay(int32_t ms);
uint8_t data_buffer[DATA_BUFFER_SIZE];
uint8_t command[ALFAT_COMMAND_LENGTH];

//////////////////////////////////////////////////
////////// Covert a interger to hex string ///////
//////////////////////////////////////////////////
//void mcs_int_to_asciihex( uint32_t number, uint8_t number_str[])
//{
//	uint8_t hex[16] = "0123456789ABCDEF";
//	int32_t i=0;
//	int32_t j=0;
//	int32_t shift=0;
//	uint8_t c=0;
//	uint32_t index = 0;
//	uint8_t start =0;
//	memset(number_str,0,8);
//	number_str[j]='0';
//	for (i=0; i<7; i++)
//	{
//		 index = (uint32_t)(((uint32_t)((uint32_t)(number) & ((uint32_t)(0xF000000>>shift)))) >> ((uint32_t)(24-shift)));
//		 c =  hex[index];
//		 if (c!='0') start++;
//		 if (c!='0' || start!=0)
//		 {
//		 	number_str[j]=c;
//			j++;
//		 }
//		 shift+=4;
//	}
//}
void ResetCmd(uint8_t *cmd)
{
	uint16_t i;

	for(i=0; i< ALFAT_COMMAND_LENGTH; i++){
		cmd[i] = 0;
	}
	//memset(cmd,0,ALFAT_COMMAND_LENGTH);
}
void AddCmd(uint8_t *cmd, int8_t *s)
{
	mcs_strcat((char*)cmd,(char*)s);
}
void SendCmd(uint8_t *cmd)
{
	WriteBytes(cmd, 0,strlen((char*)cmd));
}



/////// Encode status code ///////////
/* This funtion will encode "$00\n"*/
////////////////////////////////////
int32_t EncodeStatusCode(uint8_t data[])
{
	int32_t temp;
	if(data[0] != '$')
		return ERR_UNEXPECTED;
	temp=data[1]-'0';
	temp=(temp*16)+(data[2]-'0');
	return temp;
}


/////// Encode error code ///////////
/* This funtion will encdoe "!00\n"*/
////////////////////////////////////
int32_t EncodeErrorCode(uint8_t data[], int32_t index)
{
	int32_t temp;
	if(data[index] != '!')
		return ERR_UNEXPECTED;
	temp=data[index+1]-'0';
	temp=(temp*16)+(data[index+2]-'0');
	return temp;
}


/////// Get Version ///////////
// Send :"V\r"               //
// Return "vx.x.x\n!00\n     //
///////////////////////////////
int32_t ALFAT_GetVersion(int32_t major[], int32_t minor[], int32_t build[])
{

	ResetCmd(command);
	AddCmd(command,(int8_t*)"V\r");
	// Wait if ALFAT is not ready for new command
	while(ActivePinState())
	{
		alfat_delay(USER_TIME_DELAY);
		ReadBytes(data_buffer, 0,(DATA_BUFFER_SIZE));
	}
	SendCmd(command);
	// alfat_delay a litle bit
	alfat_delay(USER_TIME_DELAY);
	// Prepare buffer to save read data
	memset(data_buffer,0	,DATA_BUFFER_SIZE);
	// Read and save data
	ReadBytes(data_buffer, 0,DATA_BUFFER_SIZE);
	// Process version number vx.x.x
	if (data_buffer[0] != 'v')
		return ERR_UNEXPECTED;
	*major = data_buffer[1]-'0';
	*minor = data_buffer[3]-'0';
	*build = data_buffer[5]-'0';
	// Process error code
	return EncodeErrorCode(data_buffer,7);
}
////// Initialize a Media drive //////////
// Command: I<SP>X:<LF>                 //
// Return : !00<LF>						//
// X: Media drive such as U0, U1, M     //
//////////////////////////////////////////
int32_t ALFAT_InitializeMedia(uint8_t drv[])
{
	uint32_t num_read=0;
	ResetCmd(command);
	AddCmd(command,(int8_t*)"I ");
	AddCmd(command,(int8_t*)drv);
	AddCmd(command,(int8_t*)"\r");
	// Wait while ALFAT is not ready for new command
	while(ActivePinState())
	{
		alfat_delay(USER_TIME_DELAY);
		ReadBytes(data_buffer, num_read,(DATA_BUFFER_SIZE-num_read));
	}
	SendCmd(command);
	// alfat_delay a litle bit
	alfat_delay(USER_TIME_DELAY);
	// Wait if active pin is high
	memset(data_buffer,0	,DATA_BUFFER_SIZE);
	// Because initialize a media driver maybe takes more one second. We should wait untill
	// finishing initialize by check active pin.
	while(ActivePinState())
	{
		alfat_delay(USER_TIME_DELAY);
		num_read +=ReadBytes(data_buffer, num_read,(DATA_BUFFER_SIZE-num_read));
	}
	if (num_read<4)
		num_read +=ReadBytes(data_buffer, num_read,(DATA_BUFFER_SIZE-num_read));
	// Process error code
	return EncodeErrorCode(data_buffer,0); // "!00\n"

}

////// Open Handle ////////////////
// Command: O<SP>nM>full path<LF>//
// Return:  !00<LF>              //
///////////////////////////////////
int32_t ALFAT_OpenFile(uint8_t handle,uint8_t filename[], uint8_t mode[])
{
	uint32_t num_read=0;
	char handle_hex[8];

	/* Take semaphore */
	if(take_bsemaphores(ALFAT_BSEMAPHORE_ID, DEFAULT_BSEMAPHORE_DELAY) == pdPASS){


		// Conert handle from INT to HEX string
		mcs_int_to_asciihex(handle,handle_hex);
		ResetCmd(command);
		AddCmd(command,(int8_t*)"O ");
		AddCmd(command,(int8_t*)handle_hex);
		AddCmd(command,(int8_t*)mode);
		AddCmd(command,(int8_t*)">");
		AddCmd(command,(int8_t*)filename);
		AddCmd(command,(int8_t*)"\r");
		// Wait while ALFAT is not ready for new command
		while(ActivePinState())
		{
			alfat_delay(USER_TIME_DELAY);
			ReadBytes(data_buffer, num_read,(DATA_BUFFER_SIZE-num_read));
		}
		SendCmd(command);
		// alfat_delay a litle bit
		alfat_delay(USER_TIME_DELAY);
		// Wait if ALFAT is active
		while( ActivePinState())
		{
			alfat_delay(USER_TIME_DELAY);
			num_read +=ReadBytes(data_buffer, num_read,(DATA_BUFFER_SIZE-num_read));
		}
		// After delay a short time, ActivePinState() will be high with SPI but maybe it is not high with USART
		// So we have to check num_read
		if (num_read<4)
			num_read +=ReadBytes(data_buffer, num_read,(DATA_BUFFER_SIZE-num_read));

		if(give_bsemaphores(ALFAT_BSEMAPHORE_ID) != pdPASS){ }
	}
	// Process error code
	return EncodeErrorCode(data_buffer,0);

}
////// Write to a file  /////////////
// Command: W<SP>n>ssssssss<LF>    //
// Or     : L<SP>n>ssssssss<LF>    //
// Return 1: !00<LF>               //
// User sends data (ssssssss bytes)//
// Return 2: $aaaaaaaa<LF>!00<LF>  //
/////////////////////////////////////
int32_t ALFAT_WriteFile(uint8_t handle,uint8_t content[], uint32_t length)
{
	char handle_hex[8];
	char length_hex[8];
	// Test with 8*1024 * NUM_BLOCK if use 'L' command
	// Test with 4*1024 * NUM_BLOCK if use 'M' command
	//int32_t length = (strlen((char*)content))*NUM_BLOCK;
	//int32_t i =0;
	if(take_bsemaphores(ALFAT_BSEMAPHORE_ID, DEFAULT_BSEMAPHORE_DELAY) == pdPASS){
		// Convert handle from INT to HEX string
		mcs_int_to_asciihex(handle,handle_hex);
		// Convert length from INT to HEX string
		mcs_int_to_asciihex(length,length_hex);
		// Resetcommand
		ResetCmd(command);
		AddCmd(command, (int8_t*)"W ");
		AddCmd(command,(int8_t*)handle_hex);
		AddCmd(command,(int8_t*)">");
		AddCmd(command,	(int8_t*)length_hex);
		AddCmd(command,(int8_t*)"\r");
		// Wait for ALFAT is ready for new command
		while(ActivePinState())
		{
			alfat_delay(USER_TIME_DELAY);
			ReadBytes(data_buffer, 0,(DATA_BUFFER_SIZE));
		}
		// Send command
		SendCmd(command);
		memset(data_buffer,0,DATA_BUFFER_SIZE);
		// alfat_delay a short time to make sure command was sent to ALFAT and ALFAT see that command
		alfat_delay(USER_TIME_DELAY);
		/// Get first "!00\n" if ALFAT accept this command
		ReadBytes(data_buffer, 0,4);
		// Check Error code
		if(give_bsemaphores(ALFAT_BSEMAPHORE_ID) != pdPASS){ }
	}
	if (EncodeErrorCode(data_buffer,0)!=0)
		return ERR_UNEXPECTED;
	if(take_bsemaphores(ALFAT_BSEMAPHORE_ID, DEFAULT_BSEMAPHORE_DELAY) == pdPASS){


		// Write data to file
		//for (i=0; i<NUM_BLOCK; i++)
		//{
		//	WriteBytes(content,0, (length/NUM_BLOCK));
		//
		//}
		WriteBytes(content,0, (length));

		// Should delay a short time or
		// check busy pin to make sure ALFAT is done saving data and send back "$aaaaaaaa\n"
		alfat_delay(USER_TIME_DELAY);
		// Read for "$aaaaaaaa\n" - length is 10
		memset(data_buffer,0,DATA_BUFFER_SIZE);
		ReadBytes(data_buffer,0,10);
		// Read "!00\n"
		ReadBytes(data_buffer,10,4);
		// Check Error code
		if(give_bsemaphores(ALFAT_BSEMAPHORE_ID) != pdPASS){ }
	}
	return 	EncodeErrorCode(data_buffer,10);

}
///// Read data from File /////////////////////////////////////////////
// Command: R<SP>nM>ssssssss<LF>                                     //
// Return : !00<LF>ssssssss Bytes are returned $aaaaaaaa<LF>!00<LF>  //
///////////////////////////////////////////////////////////////////////
int32_t ALFAT_ReadFile(int32_t handle,uint8_t buffer[],int32_t length, uint8_t filler[])
{
	char handle_hex[8] ;
	char length_hex[8] ;
	uint32_t num_read=0;
	if(take_bsemaphores(ALFAT_BSEMAPHORE_ID, DEFAULT_BSEMAPHORE_DELAY) == pdPASS){
		// Conert handle from INT to HEX string
		mcs_int_to_asciihex(handle,handle_hex);
		// Conert length from INT to HEX string
		mcs_int_to_asciihex(length,length_hex);
		ResetCmd(command);
		AddCmd(command, (int8_t*)"R ");
		AddCmd(command,(int8_t*)handle_hex);
		AddCmd(command,(int8_t*)filler);
		AddCmd(command,(int8_t*)">");
		AddCmd(command,(int8_t*)length_hex);
		AddCmd(command,(int8_t*)"\r");
		// Wait while ALFAT is not ready for new command
		while(ActivePinState())
		{
			alfat_delay(USER_TIME_DELAY);
			ReadBytes(data_buffer, 0,(DATA_BUFFER_SIZE));
		}
		SendCmd(command);
		// alfat_delay a short time
		alfat_delay(USER_TIME_DELAY);
		// Read "!00\n"
		ReadBytes(data_buffer,0,4);


		if(give_bsemaphores(ALFAT_BSEMAPHORE_ID) != pdPASS){ }
	}
	// Check Error code
	if (EncodeErrorCode(data_buffer,0)!=0)
		return ERR_UNEXPECTED;
	if(take_bsemaphores(ALFAT_BSEMAPHORE_ID, DEFAULT_BSEMAPHORE_DELAY) == pdPASS){
		// Read data and fill to buffer
		// Make sure that we did read enough data in case we need a big data and ALFAT need time to load it from media drive.
		while(num_read<length)
		{
			num_read +=ReadBytes(buffer, 0,length);
		}
		// Read for "$aaaaaaaa\n"
		ReadBytes(data_buffer,0,10);
		// Read "!00\n"
		ReadBytes(data_buffer,10,4);
		if(give_bsemaphores(ALFAT_BSEMAPHORE_ID) != pdPASS){ }
	}
	// Check Error code
	return 	EncodeErrorCode(data_buffer,10);

}
///// Close a Handle //////////
// Command: C<SP>n<LF>       //
// Return : !00<LF>          //
///////////////////////////////
int32_t ALFAT_CloseFile(int32_t handle)
{
	char handle_hex[8];
	if(take_bsemaphores(ALFAT_BSEMAPHORE_ID, DEFAULT_BSEMAPHORE_DELAY) == pdPASS){
		// Conert handle from INT to HEX string
		mcs_int_to_asciihex(handle,handle_hex);
		ResetCmd(command);
		AddCmd(command, (int8_t*)"C ");
		AddCmd(command, (int8_t*)handle_hex);
		AddCmd(command, (int8_t*)"\r");
		// Wait while ALFAT is not ready for new command
		while(ActivePinState())
		{
			alfat_delay(USER_TIME_DELAY);
			ReadBytes(data_buffer, 0,(DATA_BUFFER_SIZE));
		}
		SendCmd(command);
		// alfat_delay a short time
		alfat_delay(USER_TIME_DELAY);
		// Wait if active pin is high
		ReadBytes(data_buffer, 0,(DATA_BUFFER_SIZE));
		// Get Error code
		if(give_bsemaphores(ALFAT_BSEMAPHORE_ID) != pdPASS){ }
	}
	return 	EncodeErrorCode(data_buffer,0); // "!00\n"

}


///// read status //////////
// Command: J<LF>       //
// Return : !00<LF>          //
///////////////////////////////
int32_t ALFAT_ReadStatus(uint8_t statbuf[])
{
	//uint16_t index = 0;

	if(take_bsemaphores(ALFAT_BSEMAPHORE_ID, DEFAULT_BSEMAPHORE_DELAY) == pdPASS){
		ResetCmd(command);
		AddCmd(command, (int8_t*)"J");
		AddCmd(command, (int8_t*)"\r");
		// Wait while ALFAT is not ready for new command
		while(ActivePinState())
		{
			alfat_delay(USER_TIME_DELAY);
			ReadBytes(data_buffer, 0,(DATA_BUFFER_SIZE));
		}
		SendCmd(command);
		// alfat_delay a short time
		alfat_delay(USER_TIME_DELAY);

		// Read "!00\n"
		ReadBytes(data_buffer,0,4);

		// Check Error code
		if(give_bsemaphores(ALFAT_BSEMAPHORE_ID) != pdPASS){ }
	}
	if (EncodeErrorCode(data_buffer,0)!=0)
		return ERR_UNEXPECTED;

	if(take_bsemaphores(ALFAT_BSEMAPHORE_ID, DEFAULT_BSEMAPHORE_DELAY) == pdPASS){

		//read 3 bytes for status
		ReadBytes(statbuf, 0, 3);


		// Read "!00\n"
		ReadBytes(data_buffer,4,4);
		if(give_bsemaphores(ALFAT_BSEMAPHORE_ID) != pdPASS){ }
	}
	// Check Error code
	return 	EncodeErrorCode(data_buffer,0);

}



//int32_t ALFAT_FileTell(uint8_t handle)
//{
//	//uint16_t index = 0;
//	uint16_t file_len = 0;
//
//	char handle_hex[2];
//	mcs_int_to_asciihex(handle_hex,handle_hex);
//
//	ResetCmd(command);
//	AddCmd(command, (int8_t*)"Y ");
//	AddCmd(command, handle_hex);
//	AddCmd(command, (int8_t*)"\r");
//	 // Wait while ALFAT is not ready for new command
//	 while(ActivePinState())
//	 {
//		alfat_delay(USER_TIME_DELAY);
//		ReadBytes(data_buffer, 0,(DATA_BUFFER_SIZE));
//	 }
//	SendCmd(command);
//	// alfat_delay a short time
//	alfat_delay(USER_TIME_DELAY);
//
//	// Read "!00\n"
//	ReadBytes(data_buffer,0,4);
//
//	// Check Error code
//	if (EncodeErrorCode(data_buffer,0)!=0)
//		return ERR_UNEXPECTED;
//
//
//	//read 3 bytes for status
//	ReadBytes(&filelen, 0, 9);
//
//
//	// Read "!00\n"
//	ReadBytes(data_buffer,4,4);
//	// Check Error code
//	return 	EncodeErrorCode(data_buffer,4);
//
//}


/*  $ssssssss<LF>
 *
 *
 */
void EncodeFileSize(char* file_len, uint32_t* file_size)
{
	/*error in parsing */
	if(file_len[0] != '$'){
		*file_size = 0;
	}

	*file_size = mcs_asciihex_to_int(&file_len[1], 8);


}

int32_t ALFAT_FIND_FILE(char path[], uint32_t* file_size)
{
	//uint16_t index = 0;
	char file_len[10] = {0};


	if(take_bsemaphores(ALFAT_BSEMAPHORE_ID, DEFAULT_BSEMAPHORE_DELAY) == pdPASS){
		ResetCmd(command);
		AddCmd(command, (int8_t*)"? ");
		AddCmd(command, (int8_t*)path);
		AddCmd(command, (int8_t*)"\r");
		// Wait while ALFAT is not ready for new command
		while(ActivePinState())
		{
			alfat_delay(USER_TIME_DELAY);
			ReadBytes(data_buffer, 0,(DATA_BUFFER_SIZE));
		}
		SendCmd(command);
		// alfat_delay a short time
		alfat_delay(USER_TIME_DELAY);

		// Read "!00\n"
		ReadBytes(data_buffer,0,4);
		// Check Error code
		if(give_bsemaphores(ALFAT_BSEMAPHORE_ID) != pdPASS){ }
	}
	if (EncodeErrorCode(data_buffer,0)!=0)
		return ERR_UNEXPECTED;

	if(take_bsemaphores(ALFAT_BSEMAPHORE_ID, DEFAULT_BSEMAPHORE_DELAY) == pdPASS){

		//read 10 bytes for length
		ReadBytes((uint8_t*)file_len, 0, 10);
		EncodeFileSize(file_len, file_size);


		//read 4 bytes for attributes
		ReadBytes(data_buffer, 4, 4);

		//read 21 bytes for time
		ReadBytes(data_buffer, 8, 21);


		// Read "!00\n"
		ReadBytes(data_buffer,29,4);
		if(give_bsemaphores(ALFAT_BSEMAPHORE_ID) != pdPASS){ }
	}
	// Check Error code
	return 	EncodeErrorCode(data_buffer,29);
}



////// Delete File ////////////////
// Command: D<SP>full path<LF>//
// Return:  !00<LF>              //
///////////////////////////////////
int32_t ALFAT_DeleteFile(uint8_t filename[])
{
	uint32_t num_read=0;
	if(take_bsemaphores(ALFAT_BSEMAPHORE_ID, DEFAULT_BSEMAPHORE_DELAY) == pdPASS){
		ResetCmd(command);
		AddCmd(command,(int8_t*)"D ");
		AddCmd(command,(int8_t*)filename);
		AddCmd(command,(int8_t*)"\r");
		// Wait while ALFAT is not ready for new command
		while(ActivePinState())
		{
			alfat_delay(USER_TIME_DELAY);
			ReadBytes(data_buffer, num_read,(DATA_BUFFER_SIZE-num_read));
		}
		SendCmd(command);
		// alfat_delay a litle bit
		alfat_delay(USER_TIME_DELAY);

		num_read +=ReadBytes(data_buffer, 0,4);
		if(give_bsemaphores(ALFAT_BSEMAPHORE_ID) != pdPASS){ }
	}
	// Process error code
	return EncodeErrorCode(data_buffer,0);

}



/////// FlushBanner ///////////
int32_t ALFAT_FlushBanner(void)
{
	uint32_t error_flag = pdFAIL;
	if(take_bsemaphores(ALFAT_BSEMAPHORE_ID, DEFAULT_BSEMAPHORE_DELAY) == pdPASS){

		// Wait if ALFAT is not ready for new command
		while(ActivePinState())
		{
			alfat_delay(1000);
			ReadBytes(data_buffer, 0,(DATA_BUFFER_SIZE));
			error_flag = pdPASS;
		}

		if(give_bsemaphores(ALFAT_BSEMAPHORE_ID) != pdPASS){ }
	}
	return error_flag;
}



///// Copy data from/to File /////////////////////////////////////////////
// Command: M<SP>HANDLE_SRC<SP>INDEX<SP>HANDLE_DES<SP>LENGTH<LF>
// Return : !00<LF>$xxxx<LF>!00<LF>  //
///////////////////////////////////////////////////////////////////////
int32_t ALFAT_CopyFile(int32_t handle_src,int32_t handle_des,int32_t index, int32_t length)
{
	char handle_src_hex[8] ;
	char handle_des_hex[8] ;
	char length_hex[8];
	char index_hex[8] ;
	if(take_bsemaphores(ALFAT_BSEMAPHORE_ID, DEFAULT_BSEMAPHORE_DELAY) == pdPASS){

		// Conert from INT to HEX string
		mcs_int_to_asciihex(handle_src,handle_src_hex);
		mcs_int_to_asciihex(handle_des,handle_des_hex);
		mcs_int_to_asciihex(length,length_hex);
		mcs_int_to_asciihex(index,index_hex);

		/* get command ready */
		ResetCmd(command);
		AddCmd(command, (int8_t*)"M ");
		AddCmd(command,(int8_t*)handle_src_hex);
		AddCmd(command, (int8_t*)" ");
		AddCmd(command,(int8_t*)index_hex);
		AddCmd(command, (int8_t*)" ");
		AddCmd(command,(int8_t*)handle_des_hex);
		AddCmd(command, (int8_t*)" ");
		AddCmd(command,(int8_t*)length_hex);
		AddCmd(command,(int8_t*)"\r");

		/* Wait while ALFAT is not ready for new command */
		while(ActivePinState()){
			alfat_delay(USER_TIME_DELAY);
			ReadBytes(data_buffer, 0,(DATA_BUFFER_SIZE));
		}

		/* send command */
		SendCmd(command);

		/* alfat_delay a short time */
		alfat_delay(USER_TIME_DELAY);

		/* Read "!00\n" */
		ReadBytes(data_buffer,0,4);
		if(give_bsemaphores(ALFAT_BSEMAPHORE_ID) != pdPASS){ }
	}

	/* Check Error code */
	if (EncodeErrorCode(data_buffer,0)!=0){
		return ERR_UNEXPECTED;
	}
	if(take_bsemaphores(ALFAT_BSEMAPHORE_ID, DEFAULT_BSEMAPHORE_DELAY) == pdPASS){

		// Read for "$xxxx\n"
		ReadBytes(data_buffer,0,5);

		// Read "!00\n"
		ReadBytes(data_buffer,5,4);
		if(give_bsemaphores(ALFAT_BSEMAPHORE_ID) != pdPASS){ }
	}
	// Check Error code
	return 	EncodeErrorCode(data_buffer,5);

}




