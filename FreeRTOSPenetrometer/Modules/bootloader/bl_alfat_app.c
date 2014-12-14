#define BL_ALFAT_APP_C

#include "bl_alfat_app.h"
#include "bl_spi.h"
#include "bl_ALFAT_SPI.h"
#include "bl_ALFAT.h"
#include "bootloader.h"

//#pragma DATA_SECTION( statbuf, ".fapi" );
static unsigned char statbuf[3]={0};

#pragma DATA_SECTION( mountcmdUSB, ".boot" );
static const unsigned char mountcmdUSB[4] = {'U', '0', ':',0x00};
#pragma DATA_SECTION( readmode, ".boot" );
static const unsigned char readmode[2] = {'R',0x00};
#pragma DATA_SECTION( bfilenameUSB, ".boot" );
static const unsigned char bfilenameUSB[20] = {'U','0',':',0x5c,'b','o','o','t',0x5c,'b','o','o','t','.','b','i','n',0x00};
#pragma DATA_SECTION( bootfhUSB, ".boot" );
static const unsigned int bootfhUSB = 1;

static unsigned int bl_mount(unsigned char mountcmd[])
{
	unsigned int status =0;

	//read status
	if( bl_ALFAT_ReadStatus(statbuf) != 0){
		return BOOT_FAIL;
	}
	status = bl_EncodeStatusCode(statbuf);

	/* mount USB */
	if(mountcmd[0]=='U'){
		//if USB0 is attached
		if( ((status>>5)&(0x00000001)) != 0x00000001 ){
			return BOOT_FAIL;
		}

		//check if USB0 needs to be mounted
		if( ((status>>2)&(0x00000001)) == 0x00000000 ){
			if (bl_ALFAT_InitializeMedia(mountcmd) != 0){
				return BOOT_FAIL;
			}
		}
	}
	return BOOT_PASS;
}


unsigned int bl_mount_usb(void)
{
	if(bl_mount((unsigned char*)mountcmdUSB) != BOOT_PASS){
		return BOOT_FAIL;
	}
	return BOOT_PASS;
}

unsigned int bl_open_bootfile(void)
{
	//open file
	if (bl_ALFAT_OpenFile(bootfhUSB, (unsigned char*)bfilenameUSB, (unsigned char*)readmode) != 0){
		return BOOT_FAIL;
	}
	return BOOT_PASS;
}

unsigned int bl_close_bootfile(void)
{
	//close file
	if(bl_ALFAT_CloseFile(bootfhUSB) != 0){
		return BOOT_FAIL;
	}

	return BOOT_PASS;
}

unsigned int bl_alfat_init(void)
{
	unsigned int error_flag = BOOT_FAIL;
	unsigned char statbuf[2];

	/* Read out Banner */
	error_flag = bl_ALFAT_FlushBanner();
	bl_ALFAT_ReadStatus(&statbuf[0]);

	return error_flag;

}

