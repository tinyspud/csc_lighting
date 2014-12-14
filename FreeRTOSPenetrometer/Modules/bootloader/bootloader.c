/*
 * bootloader.c
 *
 *  Created on: Feb 22, 2014
 *      Author: swoosh
 */

#include "bootloader.h"
#include "bl_alfat_app.h"
#include "bl_ALFAT.h"
#include "bl_spi.h"
#include "bl_ALFAT_SPI.h"
#include "bl_flash.h"

#include "bl_gio.h"
#include "bl_het.h"
//#include "reg_system.h"

//#define BL_DEBUG
#ifdef BL_DEBUG
#include "sci_common.h"
#include "sci.h"
#endif

#define FIRMWARE_VERSION 1
#define BOOTFILE_BUF_SIZE 			(512)
#define APPLICATION_START_ADDRESS 	(0x00000000)
#define BOOT_KEY_ADDRESS		  	(0x002C0000)
#define BANK0 0
#define BANK1 1



typedef volatile struct systemBase1
{
    unsigned int SYSPC1;                 /* 0x0000 */
    unsigned int SYSPC2;                 /* 0x0004 */
    unsigned int SYSPC3;                 /* 0x0008 */
    unsigned int SYSPC4;                 /* 0x000C */
    unsigned int SYSPC5;                 /* 0x0010 */
    unsigned int SYSPC6;                 /* 0x0014 */
    unsigned int SYSPC7;                 /* 0x0018 */
    unsigned int SYSPC8;                 /* 0x001C */
    unsigned int SYSPC9;                 /* 0x0020 */
    unsigned int SSWPLL1;                /* 0x0024 */
    unsigned int SSWPLL2;                /* 0x0028 */
    unsigned int SSWPLL3;                /* 0x002C */
    unsigned int CSDIS;                  /* 0x0030 */
    unsigned int CSDISSET;               /* 0x0034 */
    unsigned int CSDISCLR;               /* 0x0038 */
    unsigned int CDDIS;                  /* 0x003C */
    unsigned int CDDISSET;               /* 0x0040 */
    unsigned int CDDISCLR;               /* 0x0044 */
    unsigned int GHVSRC;                 /* 0x0048 */
    unsigned int VCLKASRC;               /* 0x004C */
    unsigned int RCLKSRC;                /* 0x0050 */
    unsigned int CSVSTAT;                /* 0x0054 */
    unsigned int MSTGCR;                 /* 0x0058 */
    unsigned int MINITGCR;               /* 0x005C */
    unsigned int MSINENA;                /* 0x0060 */
    unsigned int MSTFAIL;                /* 0x0064 */
    unsigned int MSTCGSTAT;              /* 0x0068 */
    unsigned int MINISTAT;               /* 0x006C */
    unsigned int PLLCTL1;                /* 0x0070 */
    unsigned int PLLCTL2;                /* 0x0074 */
    unsigned int UERFLAG;                /* 0x0078 */
    unsigned int DIEIDL;                 /* 0x007C */
    unsigned int DIEIDH;                 /* 0x0080 */
    unsigned int VRCTL;                  /* 0x0084 */
    unsigned int LPOMONCTL;              /* 0x0088 */
    unsigned int CLKTEST;                /* 0x008C */
    unsigned int DFTCTRLREG1;            /* 0x0090 */
    unsigned int DFTCTRLREG2;            /* 0x0094 */
    unsigned int   rsvd1;                /* 0x0098 */
    unsigned int   rsvd2;                /* 0x009C */
    unsigned int GPREG1;                 /* 0x00A0 */
    unsigned int BTRMSEL;                /* 0x00A4 */
    unsigned int IMPFASTS;               /* 0x00A8 */
    unsigned int IMPFTADD;               /* 0x00AC */
    unsigned int SSISR1;                 /* 0x00B0 */
    unsigned int SSISR2;                 /* 0x00B4 */
    unsigned int SSISR3;                 /* 0x00B8 */
    unsigned int SSISR4;                 /* 0x00BC */
    unsigned int RAMGCR;                 /* 0x00C0 */
    unsigned int BMMCR1;                 /* 0x00C4 */
    unsigned int BMMCR2;                 /* 0x00C8 */
    unsigned int MMUGCR;                 /* 0x00CC */
    unsigned int CLKCNTL;                /* 0x00D0 */
    unsigned int ECPCNTL;                /* 0x00D4 */
    unsigned int DSPGCR;                 /* 0x00D8 */
    unsigned int DEVCR1;                 /* 0x00DC */
    unsigned int SYSECR;                 /* 0x00E0 */
    unsigned int SYSESR;                 /* 0x00E4 */
    unsigned int SYSTASR;                /* 0x00E8 */
    unsigned int GBLSTAT;                /* 0x00EC */
    unsigned int DEV;                    /* 0x00F0 */
    unsigned int SSIVEC;                 /* 0x00F4 */
    unsigned int SSIF;                   /* 0x00F8 */
} bl_systemBASE1_t;

#define bl_systemREG1 ((bl_systemBASE1_t *)0xFFFFFF00U)


#pragma DATA_SECTION( g_bootfile_buf, ".fapi" );
static unsigned char g_bootfile_buf[BOOTFILE_BUF_SIZE] ={0};


#pragma DATA_SECTION( g_bootfile_filler, ".boot" );
static const unsigned char g_bootfile_filler[2] = {'^',0x00};
#pragma DATA_SECTION( bootfhUSB, ".boot" );
static const unsigned int bootfhUSB = 1;



union bl_long_32u{
	unsigned char b[4];
	unsigned long l;
};

unsigned int get_bootloader_magickey(void)
{
	unsigned int* magickey_ptr;

	magickey_ptr = (unsigned int*)BOOT_KEY_ADDRESS;

	return *magickey_ptr;
}


unsigned char set_bootloader_magickey(unsigned int key)
{

	unsigned char status;

	/*erase sector */
	status = Fapi_BlockErase( BANK1, BOOT_KEY_ADDRESS, 4);
	if(status != 0){
		return BOOT_FAIL;
	}

	/*write to flash*/
	status = Fapi_BlockProgram(BANK1, BOOT_KEY_ADDRESS, (unsigned int)&key, 4);
	if(status != 0){
		return BOOT_FAIL;
	}

	return BOOT_PASS;
}




void bl_alfat_spi_init()
{
	/*set up all spi ports except for SCS[0] and SIMO*/
	bl_spiInit();

	/*setup reset pin direction */
	bl_hetPORT1->DIR |= (1U << 27);
	/*put device in reset */
	bl_hetPORT1 ->DCLR = 1U << 27;
	//gioSetBit(hetPORT1, 27, 0);


	/*set direction */
	bl_gioPORTA->DIR |= (1U << 4);
	//gioPORTA->DIR |= BIT4;



	/* Enable power to 3.3V */
	bl_gioPORTA->DSET = 1U << 4;
	//gioSetBit(gioPORTA, 4, 1);
	/*Enable ALFAT */
	bl_gioPORTA->DSET = 1U << 0;
	//gioSetBit(gioPORTA, 0, 1);
	/* Enable USB power for ALFAT (active low) */
	bl_gioPORTA->DCLR = 1U << 2;
	//gioSetBit(gioPORTA, 2, 0);


	//release from reset
	bl_hetPORT1->DSET = 1U << 27;
	//gioSetBit(hetPORT1, 27, 1);



	/* delay to allow ALFAT to get into correct mode */
	bl_Delay(1000);



	/* Activate SIMO and SCS[0]*/
	bl_spiREG3->PC0  |= (1U << 10U);  /* SIMO */
}





unsigned char run_bootloader(void)
{
	union bl_long_32u file_size;
	unsigned int i,k;
	unsigned char calculated_checksum = 0;
	unsigned char version_size = 0;
	unsigned char version = 0;
	unsigned int file_pointer = 0;
	unsigned int bytes_read = 0;
	unsigned int flash_address_ptr =0;
	unsigned int status = 1;


#ifdef BL_DEBUG
	unsigned char* debug_address_ptr =0;
	unsigned int m;
#endif

	/*execute bootloader */
	if(get_bootloader_magickey() == UPDATE_FIRMWARE_FLAG){
		/* clear magic key*/
		set_bootloader_magickey(0x00000000);


		//_disable_IRQ();
		//_disable_FIQ();

		/*init I/Os for ALFAT */
		/* need to modify for new board */
		//bootloader_hetInit();
		bl_gioInit();

		bl_hetInit();

		/*use special bootloader alfat spi init */
		bl_alfat_spi_init();

		/*flush out banner if cold start */
		bl_alfat_init();


#ifdef BL_DEBUG
		sciInit();
#endif



		/*mount USB*/
		if(bl_mount_usb() != BOOT_PASS){
			return BOOT_FAIL;
		}

		/*open bootfile */
		if (bl_open_bootfile() != BOOT_PASS){
			return BOOT_FAIL;
		}

		/*read version size*/
		if (bl_ALFAT_ReadFile(bootfhUSB, &g_bootfile_buf[0], 1, (unsigned char*)&g_bootfile_filler[0]) != 0){
			return BOOT_FAIL;
		}
		version_size = g_bootfile_buf[0];

		/*read version TODO: handle more than one byte for version*/
		if (bl_ALFAT_ReadFile(bootfhUSB, &g_bootfile_buf[0], version_size, (unsigned char*)&g_bootfile_filler[0]) != 0){
			return BOOT_FAIL;
		}
		version = (g_bootfile_buf[0] - 0x30);
		/*make sure we are not downgrading */
		if(version < FIRMWARE_VERSION){
			/*TODO: implement force updates even though it is a downgrade */
			return BOOT_FAIL;
		}

		/*read file size TODO: add support for 64 bits file sizes*/
		if (bl_ALFAT_ReadFile(bootfhUSB, &g_bootfile_buf[0], 8, (unsigned char*)&g_bootfile_filler[0]) != 0){
			return BOOT_FAIL;
		}
		for(i=0;i<4;i++){
			file_size.b[i] = g_bootfile_buf[i];
		}


		status = Fapi_BlockErase( BANK0, 0x00000000, file_size.l);
		if(status != 0){
			return BOOT_FAIL;
		}




		//Fapi_flushPipeline();

		/*get checksum ready */
		calculated_checksum = 0x00;

		/*get file pointer ready */
		file_pointer = 0;
		flash_address_ptr = APPLICATION_START_ADDRESS;

#ifdef BL_DEBUG
		debug_address_ptr =0;
#endif

		/*read through file*/
		for(i=0; i<file_size.l; i+=BOOTFILE_BUF_SIZE){
			/*read 128 bytes at a time*/
			if (bl_ALFAT_ReadFile(bootfhUSB, &g_bootfile_buf[0], BOOTFILE_BUF_SIZE, (unsigned char*)&g_bootfile_filler[0]) != 0){
				return BOOT_FAIL;
			}




			/*write to flash*/
			status = Fapi_BlockProgram(BANK0, flash_address_ptr, (unsigned int)&g_bootfile_buf[0], BOOTFILE_BUF_SIZE);
			if(status != 0){
				return BOOT_FAIL;
			}

			/*update file_pointer */
			file_pointer += BOOTFILE_BUF_SIZE;
			flash_address_ptr += BOOTFILE_BUF_SIZE;


			/*check if we reached the end */
			if(file_pointer >= file_size.l){
				/*adjust bytes read*/
				bytes_read = (BOOTFILE_BUF_SIZE -(file_pointer-file_size.l));
				/*run checksum calculations */
				for(k=0; k<bytes_read; k++){
					calculated_checksum ^= g_bootfile_buf[k];
#ifdef BL_DEBUG
					if(g_bootfile_buf[k] != *debug_address_ptr){
						return BOOT_FAIL;
					}
					debug_address_ptr++;
#endif
				}



				/*compare checksums and return */
				if(calculated_checksum == g_bootfile_buf[k]){
					set_bootloader_magickey(0x00000000);

					if(bl_close_bootfile() != BOOT_PASS){
						return BOOT_FAIL;
					}

					/*reset device */
					bl_systemREG1->SYSECR = (0x10) << 14;
					//jumpaddress = 0x00000000;//(uint32_t)APPLICATION_START_ADDRESS;
					//((void (*)(void))jumpaddress)();

					return BOOT_PASS;
				}
				else{
					if(bl_close_bootfile() != BOOT_PASS){
						return BOOT_FAIL;
					}
					return BOOT_FAIL;
				}
			}

			else{
				/*update bytes read*/
				bytes_read = BOOTFILE_BUF_SIZE;
				/*run checksum calculations */
				for(k=0; k<bytes_read; k++){
					calculated_checksum ^= g_bootfile_buf[k];

#ifdef BL_DEBUG
					if(g_bootfile_buf[k] != *debug_address_ptr){
						return BOOT_FAIL;
					}
					debug_address_ptr++;
#endif
				}


//				/* write to flash */
//				status = Fapi_BlockProgram( BANK0, flash_address_ptr, (unsigned int)&g_bootfile_buf[0], BOOTFILE_BUF_SIZE);
//
//				if(status != 0){
//					flash_address_ptr += BOOTFILE_BUF_SIZE;
//				}
//				else{
//					return BOOT_FAIL;
//				}


			}

		}
	}

	return BOOT_PASS;
}
