#define ALFAT_APP_C


//include this first for printfs
#include "spi.h"
#include "ALFAT_SPI.h"
#include "ALFAT.h"
#include "mcs_time.h"
#include "LogTask.h"
#include "alfat_app.h"

typedef enum {
	USB_IDLE = 0,
	USB_FAILURE,
	USB_NOT_ATTACHED,
	USB_ATTACHED,
	USB_MOUNTED
} USB_STATE;

typedef enum {
	SD_IDLE = 0,
	SD_FAILURE,
	SD_NOT_ATTACHED,
	SD_ATTACHED,
	SD_PROTECTED,
	SD_MOUNTED
} SD_STATE;

typedef enum {
	ALFAT_FILE_CLOSED,
	ALFAT_FILE_OPENED_RW,
	ALFAT_FILE_OPENED_RD,
	ALFAT_FILE_OPENED_AP
}ALFAT_File_State_t;


#define INIT_RETRIES 6
#define SIZE_OF_ALFAT_BUF 	512


static uint8_t g_alfat_buf[SIZE_OF_ALFAT_BUF] = {0};
static uint16_t g_alfat_buf_counter = 0;
static uint16_t g_log_file_size_counter = 0;

uint8_t g_sd_configbuf[SIZE_OF_SD_CONFIG_BUF] = {0};//{'M','C','S',' ','E','D','I','T','E','D',0x00};
uint8_t g_sd_config_write_buf[SIZE_OF_SD_CONFIG_BUF] = {0};

static const uint8_t g_mountcmdUSB[4] = {'U', '0', ':',0x00};
static const uint8_t g_mountcmdSD[3] = {'M',':',0x00};
static const uint8_t g_filler[2] = {'^',0x00};
static uint8_t g_statbuf[3]={0};
const uint8_t g_readmode[2] = {'R',0x00};
const uint8_t g_writemode[2] = {'W',0x00};
const uint8_t g_appendmode[2] = {'A',0x00};


static USB_STATE g_usb_state = USB_IDLE;
static ALFAT_File_State_t g_usb_logfile_state = ALFAT_FILE_CLOSED;
static ALFAT_File_State_t g_usb_cfgfile_state = ALFAT_FILE_CLOSED;

static SD_STATE g_sd_state = SD_IDLE;
static ALFAT_File_State_t g_sd_logfile_state = ALFAT_FILE_CLOSED;
static ALFAT_File_State_t g_sd_cfgfile_state = ALFAT_FILE_CLOSED;
static ALFAT_File_State_t g_sd_smsfile_state = ALFAT_FILE_CLOSED;

const uint8_t g_lfilenameUSB[20] = {'U','0',':',0x5c,'l','o','g','s',0x5c,'l','o','g','s','.','t','x','t',0x00};
const uint8_t g_lfilenameSD[19] = {'M',':',0x5c,'l','o','g','s',0x5c,'l','o','g','s','.','t','x','t',0x00};

const uint8_t g_cfilenameUSB[20] = {'U','0',':',0x5c,'c','f','g','s',0x5c,'c','f','g','s','.','t','x','t',0x00};
const uint8_t g_cfilenameSD[19] = {'M',':',0x5c,'c','f','g','s',0x5c,'c','f','g','s','.','t','x','t',0x00};

#define SMS_FILE_NAME_LENGTH	37
#define SMS_FILE_NAME_DATE_START	7
#define SMS_FILE_NAME_NUMBER_START	20
static uint8_t g_smsfilenameSD[SMS_FILE_NAME_LENGTH] = {'M',':',0x5c,'S','M','S',0x5c,'Y','Y','M','M','D','D','H','H','m','m','s','s','_','N','N','N','N','N','N','N','N','N','N','N','N','.','S','M','S',0x00};





uint32_t update_sd_state(void)
{
	//SD_STATUS sd_state = SD_NOT_ATTACHED;
	uint32_t status = 0;

	//read status
	if( ALFAT_ReadStatus(g_statbuf) != 0){
		g_sd_state = SD_FAILURE;
		return pdFAIL;
	}
	status = EncodeStatusCode(g_statbuf);

	//if SD is attached
	if( ((status)&(0x00000001)) != 0x00000001 ){
		g_sd_state = SD_NOT_ATTACHED;
	}
	else{
		g_sd_state= SD_ATTACHED;
	}

	//check if SD is protected
	if( ((status>>1)&(0x00000001)) != 0x00000000 ){
		g_sd_state = SD_PROTECTED;
	}

	return pdPASS;
}


uint16_t initialize_sd(void)
{
	uint16_t error_state = pdFAIL;
	uint16_t i;

	for(i=0; i<INIT_RETRIES; i++){

		switch(g_sd_state){
		case SD_IDLE:
			update_sd_state();
			break;

		case SD_FAILURE:
			update_sd_state();
			break;

		case SD_NOT_ATTACHED:
			update_sd_state();
			break;

		case SD_ATTACHED:
			if (ALFAT_InitializeMedia((uint8_t*) g_mountcmdSD) == ERR_NO_ERROR){
				g_sd_state = SD_MOUNTED;
				g_sd_logfile_state = ALFAT_FILE_CLOSED;
			}
			else{
				update_sd_state();
			}
			break;

		case SD_MOUNTED:
			error_state = pdPASS;
			i= INIT_RETRIES;
			break;

		default:
			break;
		}
	}

	return error_state;
}


int16_t get_size_of_log_message(uint8_t writebuf[])
{
	uint16_t arr_ptr = 0;
	uint16_t i;

	for(i=0; i<SIZE_OF_LOG_WRITE_BUF; i++ ){
		if(writebuf[i] == 0x00){
			break;
		}
		arr_ptr++;
	}

	if(arr_ptr > SIZE_OF_LOG_WRITE_BUF ){
		return -1;
	}
	else{
		return arr_ptr;
	}


}




uint16_t set_sys_configs(void)
{
	/*get configs here */
	g_sd_configbuf[0] = 'M';
	g_sd_configbuf[1] = 'C';
	g_sd_configbuf[2] = 'S';
	g_sd_configbuf[3] = ' ';
	g_sd_configbuf[4] = 'T';
	g_sd_configbuf[5] = 'E';
	g_sd_configbuf[6] = 'S';
	g_sd_configbuf[7] = 'T';
	g_sd_configbuf[8] = '\r';
	g_sd_configbuf[9] = '\n';


	if (ALFAT_WriteFile(FH_CFG_SD, &g_sd_configbuf[0], SIZE_OF_SD_CONFIG_BUF) != ERR_NO_ERROR){
		return pdFAIL;
	}

	return pdPASS;
}


uint8_t compare(uint8_t a[], uint8_t b[])
{
	int c = 0;

	while( a[c] == b[c] )
	{
		if( a[c] == '\0' &&  b[c] == '\0' )
			return 1;
		c++;
	}

	return 0;
}




uint32_t close_sd_cfg_file(void)
{
	uint32_t error_state = pdFAIL;

	/* make sure log file is opened */
	if( g_sd_cfgfile_state != ALFAT_FILE_CLOSED){

		if(ALFAT_CloseFile(FH_CFG_SD) != ERR_NO_ERROR){
			error_state = pdFAIL;
		}
		else{
			g_sd_cfgfile_state = ALFAT_FILE_CLOSED;
			error_state = pdPASS;
		}
	}

	return error_state;
}



//
//uint16_t get_sys_configs(void)
//{
//	uint8_t value[MAX_KEYVALUE_LENGTH]={'\0'};
//	uint8_t key[MAX_KEYVALUE_LENGTH]={'\0'};
//	CONFIG_STATES parse_state;
//	uint16_t i,v,k,j, x, y;
//	uint8_t input_char;
//
//
//	/* make sure media is initialized */
//	initialize_sd();
//	/* make sure file is closed */
//	if(g_sd_cfgfile_state != ALFAT_FILE_CLOSED){
//		/* close file */
//		close_sd_cfg_file();
//	}
//	/* open file for read mode */
//	if (ALFAT_OpenFile(FH_CFG_SD, (uint8_t*)g_cfilenameSD,  (uint8_t*)g_readmode) == ERR_NO_ERROR){
//		g_sd_cfgfile_state = ALFAT_FILE_OPENED_RD;
//
//		/* read contents of config file and place in g_sd_configbuf */
//		if (ALFAT_ReadFile(FH_CFG_SD, &g_sd_configbuf[0], SIZE_OF_SD_CONFIG_BUF, (uint8_t*)&g_filler[0]) != ERR_NO_ERROR){
//			return pdFAIL;
//		}
//		close_sd_cfg_file();
//
//	}
//	else{
//		return pdFAIL;
//	}
//
//
//	/* parse config file, simple parser */
//	parse_state = CFG_COMMAND;
//	v=0;
//	k=0;
//	for(i=0; i<SIZE_OF_SD_CONFIG_BUF; i++ ){
//
//		input_char = g_sd_configbuf[i];
//
//
//		switch(parse_state){
//
//		case CFG_COMMAND:
//			/* if end of file is detected */
//			if(input_char == '^'){
//				return pdPASS;
//			}
//			/* if space is detected, parse current command */
//			else if(input_char == '='){
//				parse_state = CFG_VALUE;
//
//			}
//			else{
//				key[k++] = input_char;
//			}
//
//			break;
//
//
//
//		case CFG_VALUE:
//			/* if end of file is detected */
//			if(input_char == '^'){
//				return pdPASS;
//			}
//			/* if first return is detected, parse current value */
//			/* this is where applications must input their call back functions */
//			else if(input_char == 0x0D){
//				if(compare(key, (uint8_t*)KEY_CAN_PORT)){
//					//get value
//					if(compare(value, (uint8_t*)"CAN1")){
//						printf_ui_uart("Loaded CAN1 port\r\n");
//						set_can_port(CAN1_PORT);
//					}
//					else if(compare(value, (uint8_t*)"CAN2")){
//						printf_ui_uart("Loaded CAN2 port\r\n");
//						set_can_port(CAN2_PORT);
//					}
//					else if(compare(value, (uint8_t*)"CAN3")){
//						printf_ui_uart("Loaded CAN3 port\r\n");
//						set_can_port(CAN3_PORT);
//					}
//					/* insert your restore routine here
//						else if(compare(value, (uint8_t*)"USER_KEY")){
//							printf_ui_uart("Loaded USER KEY\r\n");
//							user_restore_routine(USER_VALUE);
//						}*/
//				}
//
//				else if(compare(key, (uint8_t*)KEY_HOSTNUMBER)){
//					printf_ui_uart("Loaded HOST Number\r\n");
//					/* Set the host number */
//					for(y = 0, x = 0; (x < MAX_KEYVALUE_LENGTH) & (y < SMS_TELNUMBER_LENGTH); x++)
//					{
//						if(Is_Number(value[x]))
//							g_sms_telnumber[y++] = value[x];
//					}
//				}
//				else if(compare(key, (uint8_t*)KEY_NICKNAME)){
//					printf_ui_uart("Loaded MCS Nickname\r\n");
//					/* Set the host number */
//					for(y = 0, x = 0; (x < MAX_KEYVALUE_LENGTH) & (y < MAX_KEYVALUE_LENGTH); x++)
//					{
//						/* Use graphics rendering to determine if the character is printable */
//						if(get_char_graphic_pointer(value[x]) != 0)
//							g_mcs_nickname[y++] = value[x];
//					}
//				}
//				else if(compare(key, (uint8_t*)KEY_LANGUAGE)){
//					printf_ui_uart("Loaded Default language\r\n");
//				}
//				else if(compare(key, (uint8_t*)KEY_VERSION)){
//					printf_ui_uart("Loaded Version\r\n");
//				}
//
//
//			}
//
//			/* if second return is detected jump to command state */
//			else if(input_char == 0x0A){
//				k=0;
//				v=0;
//				for(j=0; j<MAX_KEYVALUE_LENGTH; j++){
//					key[j]='\0';
//					value[j]='\0';
//				}
//
//				parse_state = CFG_COMMAND;
//			}
//			/* store value */
//			else{
//				value[v++] = input_char;
//			}
//			break;
//
//		}
//
//
//	}
//
//
//	return pdPASS;
//}
//
//
//
//
//uint16_t modify_sys_configs( uint8_t new_key[], uint8_t new_value[])
//{
//	uint8_t key[MAX_KEYVALUE_LENGTH]={'\0'};
//	MODIFY_CONFIG_STATES parse_state;
//	uint16_t k,i,j;
//	uint16_t buf;
//	uint8_t found_key;
//	uint8_t input_char;
//
//
//	/* make sure media is initialized */
//	initialize_sd();
//	/* make sure file is closed */
//	if(g_sd_cfgfile_state != ALFAT_FILE_CLOSED){
//		/* close file */
//		close_sd_cfg_file();
//	}
//	/* open file for read mode */
//	if (ALFAT_OpenFile(FH_CFG_SD, (uint8_t*)g_cfilenameSD,  (uint8_t*)g_readmode) == ERR_NO_ERROR){
//		g_sd_cfgfile_state = ALFAT_FILE_OPENED_RD;
//
//		/* read contents of config file and place in g_sd_configbuf */
//		if (ALFAT_ReadFile(FH_CFG_SD, &g_sd_configbuf[0], SIZE_OF_SD_CONFIG_BUF, (uint8_t*)&g_filler[0]) != ERR_NO_ERROR){
//			return pdFAIL;
//		}
//		close_sd_cfg_file();
//
//	}
//
//
//
//	parse_state = STORE_COMMAND;
//	k=0;
//	buf = 0;
//	found_key = 0;
//	input_char = 0;
//	for(i=0; i<SIZE_OF_SD_CONFIG_BUF; i++ ){
//
//		switch(parse_state){
//
//		case STORE_COMMAND:
//
//			/* store each byte read into a buffer to write back to flash later*/
//			input_char = g_sd_configbuf[i];
//
//			/* if end of file is detected */
//			if(input_char == '^'){
//
//				/* if no keys are found, then append key and value */
//				if(found_key != 1){
//
//					/* add key */
//					for(j=0; j<MAX_KEYVALUE_LENGTH; j++){
//						if(new_key[j] != '\0'){
//							g_sd_config_write_buf[buf++] = new_key[j];
//
//						}
//						else{
//							break;
//						}
//					}
//
//					g_sd_config_write_buf[buf++] = '=';
//
//
//					for(j=0; j<MAX_KEYVALUE_LENGTH; j++){
//						if(new_value[j] != '\0'){
//							g_sd_config_write_buf[buf] = new_value[j];
//							buf++;
//						}
//						else{
//							break;
//						}
//					}
//
//					g_sd_config_write_buf[buf++] = 0x0D;
//					g_sd_config_write_buf[buf++] = 0x0A;
//
//				}
//
//				/* append terminating character */
//				g_sd_config_write_buf[buf++] = input_char;
//
//
//				/* make sure media is initialized */
//				initialize_sd();
//				/* make sure file is closed first */
//				if(g_sd_cfgfile_state != ALFAT_FILE_CLOSED){
//					close_sd_cfg_file();
//				}
//				/*open file for write mode */
//				if (ALFAT_OpenFile(FH_CFG_SD, (uint8_t*)g_cfilenameSD,  (uint8_t*)g_writemode) == ERR_NO_ERROR){
//					g_sd_cfgfile_state = ALFAT_FILE_OPENED_RW;
//
//					//write to ALFAT
//					if (ALFAT_WriteFile(FH_CFG_SD, &g_sd_config_write_buf[0], buf) != ERR_NO_ERROR){
//						return pdFAIL;
//					}
//					close_sd_cfg_file();
//
//				}
//
//
//				return pdPASS;
//			}
//			/* if space is detected, parse current command */
//			else if(input_char == '='){
//				g_sd_config_write_buf[buf++] = input_char;
//				parse_state = STORE_VALUE;
//
//				/* compare if key matches */
//				if(compare(key, new_key)){
//					found_key = 1;
//					parse_state = MODIFY_VALUE;
//				}
//
//			}
//			else{
//				g_sd_config_write_buf[buf++] = input_char;
//				key[k++] = input_char;
//			}
//
//			break;
//
//		case STORE_VALUE:
//			/* store each byte read into a buffer to write back to flash later */
//			input_char = g_sd_configbuf[i];
//			g_sd_config_write_buf[buf++] = input_char;
//
//			/* if new line is detected, parse current value */
//			if(input_char== 0x0A){
//
//				/* reset and go back to command state */
//				k=0;
//				for(j=0; j<MAX_KEYVALUE_LENGTH; j++){
//					key[j]='\0';
//				}
//
//				parse_state = STORE_COMMAND;
//
//			}
//
//			break;
//
//		case READ_VALUE:
//			/* read each byte read into a buffer to write back to flash later */
//			input_char = g_sd_configbuf[i];
//
//			/* if new line is detected, parse current value */
//			if(input_char== 0x0A){
//				g_sd_config_write_buf[buf++] = 0x0D;
//				g_sd_config_write_buf[buf++] = input_char;
//
//				/* reset and go back to command state */
//				k=0;
//				for(j=0; j<MAX_KEYVALUE_LENGTH; j++){
//					key[j]='\0';
//				}
//				parse_state = STORE_COMMAND;
//			}
//
//			break;
//
//			/* modify the current key's value by saving it into the buffer */
//		case MODIFY_VALUE:
//
//			for(j=0; j<MAX_KEYVALUE_LENGTH; j++){
//				if(new_value[j] != '\0'){
//					g_sd_config_write_buf[buf++] = new_value[j];
//				}
//				else{
//					break;
//				}
//			}
//
//			/* read past current value */
//			parse_state = READ_VALUE;
//			break;
//
//		}
//
//
//	}
//
//	return pdFAIL;
//}





/* used for testing purposes only */
uint16_t read_sd_config_file(void)
{
	uint16_t error_state = pdFAIL;

	/* make sure media is initialized */
	initialize_sd();


	/* make sure file is closed*/
	if(g_sd_cfgfile_state != ALFAT_FILE_CLOSED){
		/* close file */
		close_sd_cfg_file();
	}

	/* open file for read mode */
	if (ALFAT_OpenFile(FH_CFG_SD, (uint8_t*)g_cfilenameSD,  (uint8_t*)g_readmode) == ERR_NO_ERROR){
		g_sd_cfgfile_state = ALFAT_FILE_OPENED_RD;

		/*get config data */
		error_state = get_sys_configs();
		close_sd_cfg_file();
	}



	return error_state;
}

/* used for testing purposes only */
uint16_t write_sd_config_file()
{
	uint16_t error_state = pdFAIL;

	initialize_sd();


	/* open file if it is closed */
	if(g_sd_cfgfile_state == ALFAT_FILE_CLOSED){

		/* open file and then execute */
		if (ALFAT_OpenFile(FH_CFG_SD, (uint8_t*)g_cfilenameSD,  (uint8_t*)g_writemode) == ERR_NO_ERROR){
			g_sd_cfgfile_state = ALFAT_FILE_OPENED_RW;

			//if(modify_sys_configs(new_key, new_value) == pdPASS){
			if(modify_sys_configs((uint8_t*)"VERSION", (uint8_t*)"1.234") == pdPASS){
				error_state = pdPASS;
			}
		}

	}
	/* if file is already opened, then exectute */
	else{
		if(set_sys_configs() == pdPASS){
			error_state = pdPASS;
		}
	}

	close_sd_cfg_file();

	return error_state;
}

uint16_t write_fixed_len_buff_to_FH(ALFAT_HANDLES filehandle, uint8_t write_buf[], int buff_len)
{
	/* Assumes the file handle's already been opened */
	return (ALFAT_WriteFile((uint8_t) filehandle, write_buf, buff_len) == ERR_NO_ERROR) ? pdPASS : pdFAIL;
}



uint16_t write_log_to_buf(uint8_t write_buf[])
{
	uint16_t k=0;
	uint16_t size_of_log_message = 0;



	size_of_log_message = get_size_of_log_message(write_buf);

	/* make sure size does not exceed buffer*/
	if( (g_alfat_buf_counter + size_of_log_message) < SIZE_OF_ALFAT_BUF ){

		/*copy into buffer */
		for(k=0; k<size_of_log_message; k++){
			g_alfat_buf[g_alfat_buf_counter + k] = write_buf[k];
		}

		/*update counters */
		g_alfat_buf_counter += size_of_log_message;
		g_log_file_size_counter += size_of_log_message;

		return pdPASS;

	}
	/*write to SD to free up buffer */
	else{
		if (ALFAT_WriteFile(FH_LOG_SD, g_alfat_buf, g_alfat_buf_counter) == ERR_NO_ERROR){

			/*reset alfat buf counter since we just cleared buffer */
			g_alfat_buf_counter = 0;

			/*copy into buffer */
			for(k=0; k<size_of_log_message; k++){
				g_alfat_buf[g_alfat_buf_counter + k] = write_buf[k];
			}
			/*update counters */
			g_alfat_buf_counter += size_of_log_message;
			g_log_file_size_counter += size_of_log_message;

			return pdPASS;
		}
		else{
			/* if write failed, update state and try again */
			//update_sd_state();
			return pdFAIL;
		}
	}
}

uint32_t append_sd_log_file(uint8_t write_buf[])
{
	uint16_t error_state = pdFAIL;

	initialize_sd();


	/* open file if it is closed */
	if(g_sd_logfile_state == ALFAT_FILE_CLOSED){
		/* if it is closed, then open file for append mode */
		if (ALFAT_OpenFile(FH_LOG_SD, (uint8_t*)g_lfilenameSD,  (uint8_t*)g_appendmode) == ERR_NO_ERROR){
			g_sd_logfile_state = ALFAT_FILE_OPENED_AP;
			if(write_log_to_buf(write_buf) == pdPASS){
				error_state = pdPASS;
			}
		}
	}
	else{
		if(write_log_to_buf(write_buf) == pdPASS){
			error_state = pdPASS;
		}

	}

	return error_state;
}



uint32_t write_sms_sd_file(receive_sms_data_struct_t * ptr2sms_data)
{
	uint16_t error_state = pdFAIL;
	uint8_t write_buf[SMS_TXT_LENGTH] = { 0 };
	int i = 0;

	/* Init the SD card */
	initialize_sd();

	/* Write the file name */
	/* M:\SMS\YYMMDDHHmmss_NNNNNNNNNNNN.SMS\0 */
	mcs_int_to_asciiint((char*)&g_smsfilenameSD[SMS_FILE_NAME_DATE_START + 0], 2, (ptr2sms_data->sms_rx_time.years) % 100);
	mcs_int_to_asciiint((char*)&g_smsfilenameSD[SMS_FILE_NAME_DATE_START + 2], 2, (ptr2sms_data->sms_rx_time.months) % 100);
	mcs_int_to_asciiint((char*)&g_smsfilenameSD[SMS_FILE_NAME_DATE_START + 4], 2, (ptr2sms_data->sms_rx_time.days) % 100);
	mcs_int_to_asciiint((char*)&g_smsfilenameSD[SMS_FILE_NAME_DATE_START + 6], 2, (ptr2sms_data->sms_rx_time.hours) % 100);
	mcs_int_to_asciiint((char*)&g_smsfilenameSD[SMS_FILE_NAME_DATE_START + 8], 2, (ptr2sms_data->sms_rx_time.mins) % 100);
	mcs_int_to_asciiint((char*)&g_smsfilenameSD[SMS_FILE_NAME_DATE_START + 10], 2, (ptr2sms_data->sms_rx_time.secs) % 100);

	/* Need to convert last character from a null to a '-' */
	g_smsfilenameSD[SMS_FILE_NAME_DATE_START + 12] = '-';

//#error BIG BUG IN HERE SOMEWHERE

	/* Write the file data */
	for(i = 0; i < SMS_TELNUMBER_LENGTH; i++)
	{
		g_smsfilenameSD[i + SMS_FILE_NAME_NUMBER_START] =
				((ptr2sms_data->sms_telnumber[i] >= (uint8_t)'0') && (ptr2sms_data->sms_telnumber[i] <= (uint8_t)'9')) ?
						(ptr2sms_data->sms_telnumber[i]) :
						'-';
	}

	/* Open the file */
	if(g_sd_smsfile_state == ALFAT_FILE_CLOSED){
		/* Open the file */
		if (ALFAT_OpenFile(FH_SMS_SD, g_smsfilenameSD,  (uint8_t*)g_writemode) == ERR_NO_ERROR){
			g_sd_smsfile_state = ALFAT_FILE_OPENED_RW;
		}
	}

	if(g_sd_smsfile_state == ALFAT_FILE_OPENED_RW){
		/* Copy the contents into the file */
		for(i = 0; i < SMS_TXT_LENGTH; i++)
			write_buf[i] = ptr2sms_data->sms_txt[i];

		/* Write data to file */
		write_fixed_len_buff_to_FH(FH_SMS_SD, write_buf, i);

		/* Close the file handle */
		ALFAT_CloseFile(FH_SMS_SD);
		g_sd_smsfile_state = ALFAT_FILE_CLOSED;
	}

	return error_state;
}


uint32_t write_buffer(void)
{
	if (ALFAT_WriteFile(FH_LOG_SD, g_alfat_buf, g_alfat_buf_counter) == ERR_NO_ERROR){
		/*reset alfat buf counter since we just cleared buffer */
		g_alfat_buf_counter = 0;
		return pdPASS;
	}
	else{
		return pdFAIL;
	}
}




uint32_t close_sd_log_file(void)
{
	uint32_t error_state = pdFAIL;
	initialize_sd();

	/* make sure log file is opened */
	if( g_sd_logfile_state != ALFAT_FILE_CLOSED){

		/* write remaining bytes in buffer before closing */
		if(write_buffer() == pdFAIL){
			return pdFAIL;
		}

		if(ALFAT_CloseFile(FH_LOG_SD) == ERR_NO_ERROR){
			g_sd_logfile_state = ALFAT_FILE_CLOSED;
			error_state = pdPASS;
		}
	}
	/*file is already closed */
	else{
		error_state = pdPASS;
	}

	return error_state;
}

uint32_t delete_sd_log_file()
{

	uint32_t error_state = pdFAIL;
	initialize_sd();


	/* make sure log file is closed */
	if( g_sd_logfile_state == ALFAT_FILE_CLOSED){
		if (ALFAT_DeleteFile((uint8_t*)g_lfilenameSD) == ERR_NO_ERROR){
			error_state = pdPASS;
		}
	}

	/* close file and try again */
	else{
		if(ALFAT_CloseFile(FH_LOG_SD) == ERR_NO_ERROR){
			g_sd_logfile_state = ALFAT_FILE_CLOSED;
			if (ALFAT_DeleteFile((uint8_t*)g_lfilenameSD) == ERR_NO_ERROR){
				error_state = pdPASS;
			}

		}

	}

	return error_state;

}

/*
 * *******************************************************************************************************************************************
 * USB FUNCTIONS
 * ******************************************************************************************************************************************
 */




uint32_t update_usb_state(void)
{
	//USB_STATUS g_usb_state = USB_NOT_ATTACHED;
	uint32_t status =0;

	//read status
	if( ALFAT_ReadStatus(g_statbuf) != 0){
		g_usb_state = USB_FAILURE;
		return pdFAIL;
	}
	status = EncodeStatusCode(g_statbuf);

	//if USB0 is attached
	if( ((status>>5)&(0x00000001)) == 0x00000001 ){
		g_usb_state = USB_ATTACHED;
		/*check if USB0 is mounted */
		if( ((status>>2)&(0x00000001)) != 0x00000000 ){
			g_usb_state = USB_MOUNTED;
		}
	}
	else{
		g_usb_state = USB_NOT_ATTACHED;
	}

	return pdPASS;;
}

uint16_t initialize_usb(void)
{
	uint16_t error_state = pdFAIL;
	uint16_t i;

	for(i=0; i<INIT_RETRIES; i++){

		switch(g_usb_state){
		case USB_IDLE:
			update_usb_state();
			break;

		case USB_FAILURE:
			update_usb_state();
			break;

		case USB_NOT_ATTACHED:
			update_usb_state();
			break;

		case USB_ATTACHED:
			if (ALFAT_InitializeMedia((uint8_t*) g_mountcmdUSB) == ERR_NO_ERROR){
				g_usb_state = USB_MOUNTED;
				g_usb_logfile_state = ALFAT_FILE_CLOSED;
				i= INIT_RETRIES;
				error_state = pdPASS;
			}
			else{
				update_usb_state();
			}
			break;

		case USB_MOUNTED:
			i= INIT_RETRIES;
			break;

		default:
			break;
		}
	}

	return error_state;
}

uint32_t close_usb_cfg_file(void)
{
	uint32_t error_state = pdFAIL;

	initialize_usb();

	/* make sure log file is opened */
	if( g_usb_cfgfile_state != ALFAT_FILE_CLOSED){

		if(ALFAT_CloseFile(FH_CFG_USB) == ERR_NO_ERROR){
			g_usb_cfgfile_state = ALFAT_FILE_CLOSED;
			error_state = pdPASS;
		}
		/* if failed, then reset state */
		else{
			g_usb_state = USB_IDLE;
			g_usb_logfile_state = ALFAT_FILE_CLOSED;
		}
	}
	else{
		error_state = pdPASS;
	}

	return error_state;

}

uint32_t close_usb_log_file(void)
{
	uint32_t error_state = pdFAIL;
	initialize_usb();

	/* make sure log file is opened */
	if( g_usb_logfile_state != ALFAT_FILE_CLOSED){

		if(ALFAT_CloseFile(FH_LOG_USB) == ERR_NO_ERROR){
			g_usb_logfile_state = ALFAT_FILE_CLOSED;
			error_state = pdPASS;
		}
		else{
			g_usb_state = USB_IDLE;
			g_usb_cfgfile_state = ALFAT_FILE_CLOSED;
		}
	}
	else{
		error_state = pdPASS;
	}

	return error_state;

}


uint32_t delete_usb_log_file()
{
	uint32_t error_state = pdFAIL;
	initialize_usb();

	/* make sure log file is closed */
	if( g_usb_logfile_state == ALFAT_FILE_CLOSED){
		if (ALFAT_DeleteFile((uint8_t*)g_lfilenameUSB) == ERR_NO_ERROR){
			error_state = pdPASS;
		}
		else{
			g_usb_state = USB_IDLE;
			g_usb_logfile_state = ALFAT_FILE_CLOSED;
		}
	}

	/* close file and try again */
	else{
		if(ALFAT_CloseFile(FH_LOG_USB) == ERR_NO_ERROR){
			g_usb_logfile_state = ALFAT_FILE_CLOSED;
			if (ALFAT_DeleteFile((uint8_t*)g_lfilenameUSB) == ERR_NO_ERROR){
				error_state = pdPASS;
			}
			else{
				g_usb_state = USB_IDLE;
				g_usb_logfile_state = ALFAT_FILE_CLOSED;
			}
		}
	}

	return error_state;
}

uint32_t get_file_size(uint8_t handle)
{

	uint32_t file_size;

	switch(handle){
	case FH_LOG_USB:
		initialize_usb();
		if( ALFAT_FIND_FILE((char*)g_lfilenameUSB, &file_size) != ERR_NO_ERROR){
			file_size = 0;
			g_usb_state = USB_IDLE;
			g_usb_logfile_state = ALFAT_FILE_CLOSED;
		}
		break;

	case FH_CFG_USB:
		initialize_usb();
		if( ALFAT_FIND_FILE((char*)g_cfilenameUSB, &file_size) != ERR_NO_ERROR){
			file_size = 0;
			g_usb_state = USB_IDLE;
			g_usb_logfile_state = ALFAT_FILE_CLOSED;
		}
		break;

	case FH_LOG_SD:
		initialize_sd();
		if( ALFAT_FIND_FILE((char*)g_lfilenameSD, &file_size) != ERR_NO_ERROR){
			file_size = 0;
		}
		break;

	case FH_CFG_SD:
		initialize_sd();
		if (ALFAT_FIND_FILE((char*)g_cfilenameSD, &file_size) != ERR_NO_ERROR){
			file_size = 0;
		}
		break;

	default:
		file_size = 0;
		break;
	}
	return file_size;
}

uint32_t copy_sd_to_usb(uint8_t usbhandle, uint8_t sdhandle)
{
	uint32_t error_state = pdFAIL;
	uint32_t sd_file_size = 0;

	initialize_usb();
	initialize_sd();

	int32_t err_code = 0;

	if(usbhandle == FH_LOG_USB){
		close_usb_log_file();

		err_code = ALFAT_OpenFile(FH_LOG_USB, (uint8_t*)g_lfilenameUSB,  (uint8_t*)g_writemode);
		if (err_code == ERR_NO_ERROR){
			g_usb_logfile_state = ALFAT_FILE_OPENED_RW;
			error_state = pdPASS;
		}
		else{
			g_usb_state = USB_IDLE;
			g_usb_logfile_state = ALFAT_FILE_CLOSED;
		}
	}
	else if(usbhandle == FH_CFG_USB){

		close_usb_cfg_file();

		err_code = ALFAT_OpenFile(FH_CFG_USB, (uint8_t*)g_cfilenameUSB,  (uint8_t*)g_writemode);
		if (err_code == ERR_NO_ERROR){
			g_usb_cfgfile_state = ALFAT_FILE_OPENED_RW;
			error_state = pdPASS;
		}
		else{
			g_usb_state = USB_IDLE;
			g_usb_cfgfile_state = ALFAT_FILE_CLOSED;
		}
	}




	if(sdhandle == FH_LOG_SD){
		close_sd_log_file();
		sd_file_size = get_file_size(FH_LOG_SD);

		err_code = ALFAT_OpenFile(FH_LOG_SD, (uint8_t*)g_lfilenameSD,  (uint8_t*)g_readmode);
		if (err_code == ERR_NO_ERROR){
			g_sd_logfile_state = ALFAT_FILE_OPENED_RD;
			error_state = pdPASS;
		}
	}
	else if(sdhandle == FH_CFG_SD){
		close_sd_cfg_file();
		sd_file_size = get_file_size(FH_CFG_SD);

		err_code = ALFAT_OpenFile(FH_CFG_SD, (uint8_t*)g_cfilenameSD,  (uint8_t*)g_readmode);
		if (err_code == ERR_NO_ERROR){
			g_sd_cfgfile_state = ALFAT_FILE_OPENED_RD;
			error_state = pdPASS;
		}

	}




	if( (sdhandle == FH_LOG_SD) && (usbhandle == FH_LOG_USB) ){
		/* copy file */
		err_code = ALFAT_CopyFile(FH_LOG_SD, FH_LOG_USB, 0, sd_file_size);
		if (err_code  == 0) {
			error_state = pdFAIL;
		}

		/* close files */
		close_sd_log_file();
		close_usb_log_file();


	}
	else if( (sdhandle == FH_CFG_SD) && (usbhandle == FH_CFG_USB) ){
		/* copy file */
		err_code = ALFAT_CopyFile(FH_CFG_SD, FH_CFG_USB, 0, sd_file_size);
		if (err_code == 0) {
			error_state = pdFAIL;
		}

		/* close files */
		close_sd_cfg_file();
		close_usb_cfg_file();
	}
	else{
		error_state = pdFAIL;
	}


	return error_state;

}



uint32_t alfat_init(void)
{
	uint32_t error_flag = pdFAIL;
	uint8_t statbuf[2];

	/* Read out Banner */
	error_flag = ALFAT_FlushBanner();
	ALFAT_ReadStatus(&statbuf[0]);

	return error_flag;

}


int16_t check_size_of_log_string(char* message)
{
	uint16_t counter =0;
	uint16_t i;

	for(i=0; i<MAX_SIZE_OF_MESSAGE; i++){
		if(message[i] == 0x00){
			return counter;
		}
		counter++;
	}

	return -1;
}

