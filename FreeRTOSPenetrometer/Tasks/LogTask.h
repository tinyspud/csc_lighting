/*
 * Log.h
 *
 *  Created on: Jan 17, 2014
 *      Author: rsnoo_000
 */

#ifndef LOG_H_
#define LOG_H_

/* SafeRTOS Includes */
#include "FreeRTOS.h"
//#include <stdint.h>

/* Log task callback */
void log_task( void* p_params );


/*define module error IDs */
#define GPS_LOG_ID			(1)
#define GSM_LOG_ID			(2)
#define PRINTER_LOG_ID		(3)
#define DISPLAY_LOG_ID		(4)
#define KEYPAD_LOG_ID		(5)
#define KEYSWITCH_LOG_ID	(6)
#define WDT_LOG_ID			(7)
#define ESTOP_LOG_ID		(8)
#define POWER_LOG_ID		(9)
#define USB_LOG_ID			(10)
#define SD_LOG_ID			(11)
#define SONALERT_LOG_ID		(12)
#define COM_LOG_ID			(13)


/* functionality error IDs */
#define BOOTLOADER_LOG_ID		(13)
#define SYSTEM_LOG_ID			(14)
#define TIMER_LOG_ID			(15)



//typedef enum {
//	ALFAT_UNINIT,   /* 0: needs to be init */
//	ALFAT_IDLE,		/* 1: ready and idle */
//	ALFAT_BUSY, 	/* 2: busy processing */
//	ALFAT_SUCCESS, 	/* 3: last command successful */
//	ALFAT_FAIL 	/* 4: last command failure */
//} ALFAT_STATE;


/* Log a string by length */
//portBaseType log_message( char* p_msg, uint8_t msg_len );
//void process_log_message(uint16_t enable_printing, char path[], char state[], char message[], uint32_t arg);


void release_log_state();

#endif /* LOG_H_ */
