/*
*/

#ifndef ALFAT_APP_H
#define ALFAT_APP_H



#define MAX_SIZE_OF_MESSAGE  90
#define SIZE_OF_SD_CONFIG_BUF  512
#define SIZE_OF_LOG_WRITE_BUF  254

extern const uint8_t g_readmode[2];
extern const uint8_t g_writemode[2];
extern const uint8_t g_appendmode[2];

extern uint8_t g_sd_configbuf[SIZE_OF_SD_CONFIG_BUF];


extern const uint8_t g_lfilenameUSB[20];
extern const uint8_t g_lfilenameSD[19];

extern const uint8_t g_cfilenameUSB[20];
extern const uint8_t g_cfilenameSD[19];




/* File handles - 16 max */
typedef enum {
	/* Log file */
	FH_LOG_SD = 0x00,
	FH_LOG_USB = 0x01,
	/* Config file */
	FH_CFG_SD = 0x02,
	FH_CFG_USB = 0x03,
	/* SMS Log files */
	FH_SMS_SD = 0x04,
	FH_SMS_USB = 0x05,
	/* CAN Log files */
	FH_CAN_SD = 0x06,
	FH_CAN_USB = 0x07,
	/* Time */
	FH_TIME_SD = 0x08,
	/* Unused */
	FH_UNUSED_9 = 0x09,
	FH_UNUSED_A = 0x0A,
	FH_UNUSED_B = 0x0B,
	FH_UNUSED_C = 0x0C,
	FH_UNUSED_D = 0x0D,
	FH_UNUSED_E = 0x0E,
	FH_UNUSED_F = 0x0F
} ALFAT_HANDLES;


typedef enum {
	CFG_COMMAND = 1,
	CFG_VALUE
} CONFIG_STATES;

typedef enum {
	STORE_COMMAND = 1,
	STORE_VALUE,
	READ_VALUE,
	MODIFY_VALUE
} MODIFY_CONFIG_STATES;

#define KEY_HOSTNUMBER	"HOST_NUMBER"
#define KEY_LANGUAGE	"LANGUAGE"
#define KEY_VERSION		"VERSION"
#define KEY_CAN_PORT	"CAN_PORT"
#define KEY_NICKNAME	"MCS_NICKNAME"

//uint32_t vreadBootFile(void);
uint32_t vreadLogUSBFile(void);
uint32_t vreadLogSDFile(void);

uint32_t copy_sd_to_usb(uint8_t usbhandle, uint8_t sdhandle);
uint32_t alfat_init(void);
uint32_t append_usb_log_file(uint8_t write_buf[]);
uint32_t append_sd_log_file(uint8_t write_buf[]);
uint32_t delete_usb_log_file(void);
uint32_t delete_sd_log_file(void);
uint32_t close_usb_log_file(void);
uint32_t close_sd_log_file(void);

uint16_t read_sd_config_file(void);
uint16_t write_sd_config_file();

uint32_t get_file_size(uint8_t handle);

uint16_t process_log_message(uint16_t enable_printing, char* path, char* message, uint32_t arg);

int16_t check_size_of_log_string(char*);

uint16_t write_fixed_len_buff_to_FH(ALFAT_HANDLES filehandle, uint8_t write_buf[], int buff_len);

/*******************************************************************
* NAME :            uint16_t get_sys_configs(void)
*
* DESCRIPTION :     Allows an application to retrieve configuration data to the SD card for persistent
*                   storage. This function must be modified to call each application's restore data.
*
*
* INPUTS :
*       PARAMETERS:
*          N/A
*
* OUTPUTS :
*       RETURN :
*            Type:   uint16_t               Error code:
*                    SUCCESS	            pdPASS
*                    FAILURE	            pdFAIL
*
* File format:
* KEY=value(\r\n)
* value is everything between = and \r; max length is MAX_KEYVALUE_LENGTH for both key and value
* Character '^' denotes end of file
*
*********************************************************************/
uint16_t get_sys_configs(void);


/*******************************************************************
* NAME :            uint16_t modify_sys_configs(uint8_t new_key[], uint8_t new_value[])
*
* DESCRIPTION :     Allows an application to store configuration data to the SD card for persistent
*                   storage.  A function must be provided in the startup routine to restore configuration
*                   data.  By calling this function with a key-value pair, it will store it into the configuraiton
*                   file on the sd card. If the key exists, it will modify it with the new value.  If the key does
*                   not exist, it will append the new key-value pair.  The format is based on a stripped down
*                   .ini format.
*                   Example of key-value pairs are:  VERSION=1.00, LANGUAGE=ENGLISH, TIMEZONE=-10
*
* EXAMPLE: modify_sys_configs((uint8_t*)"CAN_PORT", (uint8_t*)"CAN1");
*
*
* INPUTS :
*       PARAMETERS:
*          uint8_t[]    new_key              string name of key
*          uint8_t[]    new_value            array of new values
*
* OUTPUTS :
*       RETURN :
*            Type:   uint16_t               Error code:
*                    SUCCESS	            pdPASS
*                    FAILURE	            pdFAIL
*
*********************************************************************/
uint16_t modify_sys_configs( uint8_t new_key[], uint8_t new_value[]);

#endif

