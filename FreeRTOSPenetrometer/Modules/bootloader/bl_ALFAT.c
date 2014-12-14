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

#include "bl_ALFAT.h"
#include "bootloader.h"
#include "bl_ALFAT_SPI.h"

// Delay function.
#define COMMAND_SIZE 256

extern void bl_Delay(unsigned int ms);


//#pragma DATA_SECTION( data_buffer, ".fapi" );
static unsigned char data_buffer[DATA_BUFFER_SIZE]={0};
//#pragma DATA_SECTION( command, ".fapi" );
static unsigned char command[COMMAND_SIZE];
#pragma DATA_SECTION( bl_hex, ".boot" );
static const unsigned char bl_hex[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

//////////////////////////////////////////////// 
//////// Covert a interger to hex string ///////
////////////////////////////////////////////////
void bl_ConvertIntToHex( unsigned int number, unsigned char number_str[])
{
	//unsigned char hex[16] = "0123456789ABCDEF";
	int i=0;
	int j=0;
	int shift=0;
	unsigned char c=0;
	unsigned int index = 0;
	unsigned char start =0;

	for(i=0; i<8; i++){
		number_str[i] = 0;
	}
	//memset(number_str,0,8);

	number_str[0]='0';
	j=0;
	for (i=0; i<7; i++)
	{
		 index = (unsigned int)(((unsigned int)((unsigned int)(number) & ((unsigned int)(0xF000000>>shift)))) >> ((unsigned int)(24-shift)));
		 c =  bl_hex[index];
		 if (c!='0') start++;
		 if (c!='0' || start!=0)
		 {
		 	number_str[j]=c;
			j++;
		 }
		 shift+=4;
	}
}
void bl_ResetCmd(unsigned char cmd[])
{
	unsigned int i;

	for(i=0; i<COMMAND_SIZE; i++){
		cmd[i] = 0;
	}
	//memset(cmd,0,256);
}

void bl_strcat(char* dest, char* src)
{
	unsigned int i;
	unsigned int destptr=0;

	/* find end of string */
	for(i=0; i<COMMAND_SIZE; i++){
		if(dest[i]==0){
			break;
		}
		else{
			destptr++;
		}
	}

	/*append src to dest string */
	for(i=0; i<COMMAND_SIZE; i++){
		if(src[i] == 0x00){
			break;
		}
		else{
			dest[destptr]=src[i];
			destptr++;
		}
	}

}


void bl_AddCmd(unsigned char *cmd, signed char *s)
{
	bl_strcat((char*)cmd,(char*)s);
}

unsigned int bl_strlen(unsigned char cmd[])
{
	unsigned int i;
	unsigned int counter;

	counter=0;
	for(i=0; i<COMMAND_SIZE; i++){
		counter++;
		if(cmd[i] == 0x0D ){
			break;
		}
	}

	return counter;
}

void bl_SendCmd(unsigned char cmd[])
{
	bl_WriteBytes(cmd, 0, bl_strlen(cmd));
}



/////// Encode status code ///////////
/* This funtion will encode "$00\n"*/
////////////////////////////////////
int bl_EncodeStatusCode(unsigned char data[])
{
	int temp;
	if(data[0] != '$')
		return ERR_UNEXPECTED;
	temp=data[1]-'0';
	temp=(temp*16)+(data[2]-'0');
	return temp;
}


/////// Encode error code ///////////
/* This funtion will encdoe "!00\n"*/
////////////////////////////////////
int bl_EncodeErrorCode(unsigned char data[], int index)
{
	int temp;
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
int bl_ALFAT_GetVersion(int major[], int minor[], int build[])
{
	unsigned int i;
	
	bl_ResetCmd(command);
	bl_AddCmd(command,(signed char*)"V\r");
	// Wait if ALFAT is not ready for new command
	while(bl_ActivePinState())
	{
		bl_Delay(USER_TIME_DELAY);
		bl_ReadBytes(data_buffer, 0,(DATA_BUFFER_SIZE));
	}
	bl_SendCmd(command);
	// Delay a litle bit
	bl_Delay(USER_TIME_DELAY);
	// Prepare buffer to save read data
	for(i=0; i<DATA_BUFFER_SIZE; i++){
		data_buffer[i]=0;
	}
	//memset(data_buffer,0	,DATA_BUFFER_SIZE);
	// Read and save data
	bl_ReadBytes(data_buffer, 0,DATA_BUFFER_SIZE);
	// Process version number vx.x.x
	if (data_buffer[0] != 'v')
	  return ERR_UNEXPECTED;
	*major = data_buffer[1]-'0';
	*minor = data_buffer[3]-'0';
	*build = data_buffer[5]-'0';
	// Process error code
	return bl_EncodeErrorCode(data_buffer,7);
}
////// Initialize a Media drive //////////
// Command: I<SP>X:<LF>                 //
// Return : !00<LF>						//
// X: Media drive such as U0, U1, M     //
//////////////////////////////////////////
int bl_ALFAT_InitializeMedia(unsigned char drv[])
{
	 unsigned int num_read=0;
	 unsigned int i;

	 bl_ResetCmd(command);
	 bl_AddCmd(command,(signed char*)"I ");
	 bl_AddCmd(command,(signed char*)drv);
	 bl_AddCmd(command,(signed char*)"\r");
	 // Wait while ALFAT is not ready for new command
	 while(bl_ActivePinState())
	 {
		 bl_Delay(USER_TIME_DELAY);
		 bl_ReadBytes(data_buffer, num_read,(DATA_BUFFER_SIZE-num_read));
	 }
	 bl_SendCmd(command);
	 // Delay a litle bit
	 bl_Delay(USER_TIME_DELAY);
	 // Wait if active pin is high
	 for(i=0; i<DATA_BUFFER_SIZE; i++){
		 data_buffer[i]=0;
	 }
	 //memset(data_buffer,0	,DATA_BUFFER_SIZE);
	 // Because initialize a media driver maybe takes more one second. We should wait untill 
	 // finishing initialize by check active pin.
	 while(bl_ActivePinState())
	 {
		 bl_Delay(USER_TIME_DELAY);
		num_read += bl_ReadBytes(data_buffer, num_read,(DATA_BUFFER_SIZE-num_read));
	 }
	 if (num_read<4)
		num_read += bl_ReadBytes(data_buffer, num_read,(DATA_BUFFER_SIZE-num_read));
	 // Process error code
	 return bl_EncodeErrorCode(data_buffer,0); // "!00\n"

}		

////// Open Handle ////////////////
// Command: O<SP>nM>full path<LF>//
// Return:  !00<LF>              //
/////////////////////////////////// 
int bl_ALFAT_OpenFile(unsigned char handle,unsigned char filename[], unsigned char mode[])
{
	unsigned int num_read=0;
	unsigned char handle_hex[8];
	// Conert handle from INT to HEX string 
	bl_ConvertIntToHex(handle,handle_hex);
	bl_ResetCmd(command);
	bl_AddCmd(command,(signed char*)"O ");
	bl_AddCmd(command,(signed char*)handle_hex);
	bl_AddCmd(command,(signed char*)mode);
	bl_AddCmd(command,(signed char*)">");
	bl_AddCmd(command,(signed char*)filename);
	bl_AddCmd(command,(signed char*)"\r");
	 // Wait while ALFAT is not ready for new command
	 while(bl_ActivePinState())
	 {
		 bl_Delay(USER_TIME_DELAY);
		 bl_ReadBytes(data_buffer, num_read,(DATA_BUFFER_SIZE-num_read));
	 }
	 bl_SendCmd(command);
	// Delay a litle bit
	 bl_Delay(USER_TIME_DELAY);
	// Wait if ALFAT is active
	while( bl_ActivePinState())
	{
		bl_Delay(USER_TIME_DELAY);
		num_read += bl_ReadBytes(data_buffer, num_read,(DATA_BUFFER_SIZE-num_read));
	}
	// After delay a short time, ActivePinState() will be high with SPI but maybe it is not high with USART
	// So we have to check num_read
	if (num_read<4)
		num_read += bl_ReadBytes(data_buffer, num_read,(DATA_BUFFER_SIZE-num_read));
	// Process error code
	return bl_EncodeErrorCode(data_buffer,0);

}

///// Read data from File /////////////////////////////////////////////
// Command: R<SP>nM>ssssssss<LF>                                     //
// Return : !00<LF>ssssssss Bytes are returned $aaaaaaaa<LF>!00<LF>  //
///////////////////////////////////////////////////////////////////////								
int bl_ALFAT_ReadFile(int handle,unsigned char buffer[],int length, unsigned char filler[])
{
	unsigned char handle_hex[8] ;
	unsigned char length_hex[8] ;
	unsigned int num_read=0;
	// Conert handle from INT to HEX string 
	bl_ConvertIntToHex(handle,handle_hex);
	// Conert length from INT to HEX string 
	bl_ConvertIntToHex(length,length_hex);
	bl_ResetCmd(command);
	bl_AddCmd(command, (signed char*)"R ");
	bl_AddCmd(command,(signed char*)handle_hex);
	bl_AddCmd(command,(signed char*)filler);
	bl_AddCmd(command,(signed char*)">");
	bl_AddCmd(command,(signed char*)length_hex);
	bl_AddCmd(command,(signed char*)"\r");
	 // Wait while ALFAT is not ready for new command
	 while(bl_ActivePinState())
	 {
		 bl_Delay(USER_TIME_DELAY);
		 bl_ReadBytes(data_buffer, 0,(DATA_BUFFER_SIZE));
	 }
	 bl_SendCmd(command);
	// Delay a short time
	 bl_Delay(USER_TIME_DELAY);
	// Read "!00\n"
	 bl_ReadBytes(data_buffer,0,4);
	// Check Error code
	if (bl_EncodeErrorCode(data_buffer,0)!=0)
		return ERR_UNEXPECTED; 
	// Read data and fill to buffer
	// Make sure that we did read enough data in case we need a big data and ALFAT need time to load it from media drive.
	while(num_read<length) 
	{
		num_read += bl_ReadBytes(buffer, 0,length);
	}
	// Read for "$aaaaaaaa\n"
	bl_ReadBytes(data_buffer,0,10);
	// Read "!00\n"
	bl_ReadBytes(data_buffer,10,4);
	// Check Error code
	return 	bl_EncodeErrorCode(data_buffer,10);

}
///// Close a Handle //////////
// Command: C<SP>n<LF>       //
// Return : !00<LF>          //
///////////////////////////////
int bl_ALFAT_CloseFile(int handle)
{
	unsigned char handle_hex[8];
	// Conert handle from INT to HEX string  
	bl_ConvertIntToHex(handle,handle_hex);
	bl_ResetCmd(command);
	bl_AddCmd(command, (signed char*)"C ");
	bl_AddCmd(command, (signed char*)handle_hex);
	bl_AddCmd(command, (signed char*)"\r");
	 // Wait while ALFAT is not ready for new command
	 while(bl_ActivePinState())
	 {
		 bl_Delay(USER_TIME_DELAY);
		 bl_ReadBytes(data_buffer, 0,(DATA_BUFFER_SIZE));
	 }
	 bl_SendCmd(command);
	// Delay a short time
	 bl_Delay(USER_TIME_DELAY);
	// Wait if active pin is high
	 bl_ReadBytes(data_buffer, 0,(DATA_BUFFER_SIZE));
	// Get Error code
	return 	bl_EncodeErrorCode(data_buffer,0); // "!00\n"

}


///// read status //////////
// Command: J<LF>       //
// Return : !00<LF>          //
///////////////////////////////
int bl_ALFAT_ReadStatus(unsigned char statbuf[])
{
	//uint16_t index = 0;

	bl_ResetCmd(command);
	bl_AddCmd(command, (signed char*)"J");
	bl_AddCmd(command, (signed char*)"\r");
	 // Wait while ALFAT is not ready for new command
	 while(bl_ActivePinState())
	 {
		 bl_Delay(USER_TIME_DELAY);
		 bl_ReadBytes(data_buffer, 0,(DATA_BUFFER_SIZE));
	 }
	 bl_SendCmd(command);
	// Delay a short time
	 bl_Delay(USER_TIME_DELAY);

	// Read "!00\n"
	 bl_ReadBytes(data_buffer,0,4);

	// Check Error code
	if (bl_EncodeErrorCode(data_buffer,0)!=0)
		return ERR_UNEXPECTED;


	//read 3 bytes for status
	bl_ReadBytes(statbuf, 0, 3);


	// Read "!00\n"
	bl_ReadBytes(data_buffer,4,4);
	// Check Error code
	return bl_EncodeErrorCode(data_buffer,0);

}






/////// FlushBanner ///////////
int bl_ALFAT_FlushBanner(void)
{
	unsigned int error_flag = BOOT_FAIL;

	// Wait if ALFAT is not ready for new command
	while(bl_ActivePinState())
	{
		bl_Delay(1000);
		bl_ReadBytes(data_buffer, 0,(DATA_BUFFER_SIZE));
		error_flag = BOOT_PASS;
	}

	return error_flag;
}





