#include <stdint.h>
#include "LogTask.h"
#include "mcs_time.h"
#include "alfat_app.h"
#include "SerializedInfo.h"
#include "GitParams.h"


static clock_struct_t g_log_clock;
static char g_log_mesg_buf[SIZE_OF_LOG_WRITE_BUF] = {0};
static canopen_app_state_t g_pump_state;



#define WRITE_GIT_PARAM(x) 	for(j = 0, i = 0, writenull = 0; j < SIZE_OF_LOG_WRITE_BUF; i++, j++){\
		if(writenull == 0){\
			if(x[i] == 0x00)\
			writenull = 1;\
		}\
		g_log_mesg_buf[j] = (writenull == 0) ? (x[i]) : (0x00);\
}\
log_message((char*)g_log_mesg_buf, SIZE_OF_LOG_WRITE_BUF);



void write_compile_data(){

	/* Write all of the compile params into the log file */
	char _buildpath[] = BUILD_PATH;
	char _buildsystem[] = BUILD_SYSTEM;
	char _branchname[] = BRANCH_NAME;
	char _buildusername[] = BUILD_USER_NAME;
	char _builddomainname[] = BUILD_USER_DOMAIN;
	char _commitsha[] = COMMIT_SHA;
	char _lastcommittime[] = LAST_COMMIT_TIME_UTC;
	char _compiletime[] = COMPILE_TIME_UTC;

#ifndef HAS_GIT_PARAMS
#error GIT Parameters not present (Contact C. Chock chris@beaumontdesign.com)
#endif


	int i = 0, j = 0, writenull = 0;
	/*clear buffer */
	for(i=0; i<SIZE_OF_LOG_WRITE_BUF; i++){
		g_log_mesg_buf[i] = 0;
	}

	WRITE_GIT_PARAM(_buildpath);
	/* Transmit buffer out */
	WRITE_GIT_PARAM(_buildsystem);

	WRITE_GIT_PARAM(_buildusername);

	WRITE_GIT_PARAM(_builddomainname);

	WRITE_GIT_PARAM(_branchname);

	WRITE_GIT_PARAM(_commitsha);

	WRITE_GIT_PARAM(_lastcommittime);

	WRITE_GIT_PARAM(_compiletime);

	WRITE_GIT_PARAM(DieIDStringNullTerminated);

}


uint16_t process_log_message(uint16_t enable_printing, char* path, char* message, uint32_t arg)
{
	int16_t sizeofmessage =0;
	int16_t tempsize =0;
	char tempbuf[10];
	uint16_t i;

	/*make sure message size is not too large for our buffers */
	tempsize = check_size_of_log_string(path);
	if(tempsize!= -1){
		sizeofmessage += tempsize;
	}
	else{
		return pdFAIL;
	}
	tempsize = check_size_of_log_string(message);
	if(tempsize!= -1){
		sizeofmessage += tempsize;
	}
	else{
		return pdFAIL;
	}

	/* if size of path and message plus 30 bytes overhead exceeds max buffer size, then error */
	if(sizeofmessage +30 < SIZE_OF_LOG_WRITE_BUF){
		get_system_clock(&g_log_clock);
		canopen_app_get_current_state(&g_pump_state);

		/*clear buffer */
		for(i=0; i<SIZE_OF_LOG_WRITE_BUF; i++){
			g_log_mesg_buf[i] = 0;
		}

		mcs_int_to_asciiint (tempbuf, 2, g_log_clock.years);
		mcs_strcat(g_log_mesg_buf, tempbuf);
		mcs_strcat(g_log_mesg_buf, "/");
		mcs_int_to_asciiint (tempbuf, 2, g_log_clock.months);
		mcs_strcat(g_log_mesg_buf, tempbuf);
		mcs_strcat(g_log_mesg_buf, "/");
		mcs_int_to_asciiint (tempbuf, 2, g_log_clock.days);
		mcs_strcat(g_log_mesg_buf, tempbuf);
		mcs_strcat(g_log_mesg_buf, " ");
		mcs_int_to_asciiint (tempbuf, 2, g_log_clock.hours);
		mcs_strcat(g_log_mesg_buf, tempbuf);
		mcs_strcat(g_log_mesg_buf, ":");
		mcs_int_to_asciiint (tempbuf, 2, g_log_clock.mins);
		mcs_strcat(g_log_mesg_buf, tempbuf);
		mcs_strcat(g_log_mesg_buf, ":");
		mcs_int_to_asciiint (tempbuf, 2, g_log_clock.secs);
		mcs_strcat(g_log_mesg_buf, tempbuf);
		mcs_strcat(g_log_mesg_buf, ", ");
		mcs_int_to_asciiint (tempbuf, 1, g_pump_state.currstate);
		mcs_strcat(g_log_mesg_buf, tempbuf);
		mcs_strcat(g_log_mesg_buf, ", ");
		mcs_strcat(g_log_mesg_buf, path);
		mcs_strcat(g_log_mesg_buf, ", ");
		mcs_strcat(g_log_mesg_buf, message);
		mcs_strcat(g_log_mesg_buf, "=");
		mcs_int_to_asciiint (tempbuf, 4, arg);
		mcs_strcat(g_log_mesg_buf, tempbuf);
		mcs_strcat(g_log_mesg_buf, "\r\n");

		if(enable_printing == TRUE_FLAG){
			printf_ui_uart(g_log_mesg_buf);
		}

		log_message((char*)g_log_mesg_buf, SIZE_OF_LOG_WRITE_BUF);

		return pdPASS;
	}
	else{
		return pdFAIL;
	}


}




