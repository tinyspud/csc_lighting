/*
 * bootloader.h
 *
 *  Created on: Feb 22, 2014
 *      Author: swoosh
 */

#ifndef BOOTLOADER_H_
#define BOOTLOADER_H_




#define BOOT_FAIL (0x04)
#define BOOT_PASS (0x08)

#define UPDATE_FIRMWARE_FLAG 		(0xABACADAF)


unsigned char run_bootloader(void);
unsigned char set_bootloader_magickey(unsigned int key);

#endif /* BOOTLOADER_H_ */
