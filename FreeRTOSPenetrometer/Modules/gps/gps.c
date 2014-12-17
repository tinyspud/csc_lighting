/*
 * gps.c
 *
 *  Created on: Dec 28, 2013
 *      Author: skatsu
 */

/************************************************************
 * NMEA syntax parser - c file
 *
 * Please see header file for more details
 *
 ************************************************************/

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "os_timer.h"
#include "os_queue.h"

#include "reg_sci.h"
#include "common.h"
#include "Tasks.h"
#include "sci.h"
#include "spi.h"
#include "gps.h"
#include "het.h"
#include "gio.h"


#define UART	(scilinREG)


#define WDCNTMAX	6	//maximum word size for parsing
#define STRCNTMAX	100	//maximum string size for parsing - might need to be longer
//static const uint8_t g_at_gpgsa_string[5]={'G','P','G','S','A'};
static const uint8_t g_at_gpgll_string[5] = { 'G', 'P', 'G', 'L', 'L' };
uint8_t at_PSRF100[25] = "$PSRF100,1,38400,8,1,0";
static uint8_t g_rx_buffer_for_gps_msg[STRCNTMAX];
static TimerHandle_t xGPSTimer;
static uint32_t g_gps_timer_expired_flag = pdFALSE;
static GPS_BAUD g_current_gps_baud = GPS_9600;

typedef enum {
	RMC_START,
	RMC_MID,
	RMC_UTCH,
	RMC_UTCM,
	RMC_UTCS,
	RMC_STAT,
	RMC_LATD,
	RMC_LATM,
	RMC_NWIN,
	RMC_LOND,
	RMC_LONM,
	RMC_EWIN,
	RMC_SPEED,
	RMC_COURSE,
	RMC_DATEDD,
	RMC_DATEMM,
	RMC_DATEYY,
	RMC_MODE,
	RMC_END
} RMCPARSE_STATES;

typedef enum {
	START,
	MID,
	LATD,
	LATM,
	NWIN,
	LOND,
	LONM,
	EWIN,
	UTCH,
	UTCM,
	UTCS,
	STAT,
	MODE,
	END
} GLLPARSE_STATES;

typedef enum {
	CHECK_MCS_9600, SET_GPS_38400, SET_MCS_38400, CHECK_MCS_38400, PROCESS
} GPSBAUD_STATES;



void print_gps_data_struct(GPSDATA_S* gps_rx_struct)
{
//	if(gps_rx_struct->Status == 'A'){
//		printf_ui_uart("Latitude: %d\r\n", gps_rx_struct->Latt.Degrees);
//		printf_ui_uart("Longitude: %d\r\n", gps_rx_struct->Long.Degrees);
//		printf_ui_uart("Hours: %d\r\n", gps_rx_struct->Time.hour);
//		printf_ui_uart("Minutes: %d\r\n", gps_rx_struct->Time.min);
//		printf_ui_uart("Day: %d\r\n", gps_rx_struct->Date.day);
//		printf_ui_uart("Month: %d\r\n", gps_rx_struct->Date.month);
//		printf_ui_uart("Year: %d\r\n", gps_rx_struct->Date.year);
//
//	}
//	else{
//		printf_ui_uart("Not enough satellites \r\n");
//	}
}





/* send gps return type characters */
void send_new_line_string_for_gps(void) {
	send_byte_on_uart('\r');
	send_byte_on_uart('\n');
}

/* send a string out over gps uart port */
void send_string_to_gps(uint8_t *strin, uint16_t len) {
	uint16 i;

	for (i = 0; i < len; i++) {
		send_byte_on_uart(strin[i]);
		//UART_putChar(UART, strin[i]);
	}

}

/* append hex as an ascii */
void append_as_hex_string(uint8_t* strin, uint8_t num) {

	int len = strlen((const char*) strin);
	int i = len;
	for (i = len; i < len + 2; i++) {
		if ((num & 0xF0) < 0xA0) {
			strin[i] = (uint8_t) ((num >> 4) + 48);
		} else {
			strin[i] = (uint8_t) ((num >> 4) + 55);
		}
		num <<= 4;
	}
}

/* generate and append checksum to a given string input*/
void gen_and_append_checksum(uint8_t* strin) {
	int len = strlen((const char*) strin);

	uint8_t checksum;
	uint16_t i;

	checksum = 0;
	for (i = 0 + 1; i < len; i++) {
		if (strin[i] == '*')
			break;
		checksum ^= strin[i];
	}

	/* Add checksum characters to the end of strin */
	strin[len] = '*';
	append_as_hex_string(&strin[0], checksum);
}

/* send nme message to query gps module */
uint16_t send_nmea_query_msg(NMEAType querytype) {
	uint8_t at_PSRF103[19] = "$PSRF103,N,1,0,1";

	/* put in query type */
	at_PSRF103[9] = (uint8_t) (((int) querytype) + 48);
	/* append checksum */
	gen_and_append_checksum(&at_PSRF103[0]);
	/* send command */
	send_string_to_gps(&at_PSRF103[0], 19);
	/* send return carriage */
	send_new_line_string_for_gps();

	return pdPASS ;
}

/* send nmea message to alter baud rate */
uint16_t send_nmea_setmaxbaud_msg(void) {

	if (g_current_gps_baud == GPS_9600) {

		/* append checksum */
		gen_and_append_checksum(&at_PSRF100[0]);
		/* send command */
		send_string_to_gps(&at_PSRF100[0], 25);
		/* send return carriage */
		send_new_line_string_for_gps();

		g_current_gps_baud = GPS_38400;
		return pdPASS ;
	} else {
		return pdFAIL ;
	}
}

///* convert ascii type of gps messages to ints */
//int16_t mcs_asciihex_to_int(char* instr, uint8_t len) {
//	uint16_t val = 0;
//	uint8_t i;
//
//	for (i = 0; i < len; i++) {
//
//		/* make sure ascii values are in range */
//		if ((instr[i] > '9') || (instr[i] < '0')) {
//			return -1;
//		} else {
//			val += ((instr[i] - '0') * mcs_pow(10, (len - i - 1)));
//		}
//
//	}
//
//	return val;
//}



/* parse rmc type gps messages */
uint32_t parse_rmc_msg(GPSDATA_S* rmc, uint8_t *instr) {

	RMCPARSE_STATES state;
	char wdbuf[7];
	uint8_t chksum;
	uint8_t wdcnt;
	uint8_t i;

	wdcnt = 0;
	chksum = 0;

	//	invalidate_GPSDATA_S_data(rmc);

	/* check if data is valid first */
	if (instr[18] != 'A') {
		rmc->Status = instr[18];
		return pdFAIL ;
	}

	state = RMC_START;
	/* loop through each character in the string */
	for (i = 0; i < STRCNTMAX; i++) {

		/* increment word counter */
		wdbuf[wdcnt] = instr[i];
		if (wdcnt > WDCNTMAX) {
			wdcnt = 0;
		} else {
			wdcnt++;
		}

		/* perform check sum */
		if ((instr[i] != '$') && (instr[i] != '*')) {
			chksum ^= instr[i];
		}

		switch (state) {
		case RMC_START: {
			if (instr[i] == '$') {
				state = RMC_MID;
			}
			break;
		}
		/* stop when comma is found */
		case RMC_MID: {
			if (instr[i] == ',') {
				state = RMC_UTCH;
				wdcnt = 0;
			}
			break;
		}
		/* start parsing UTC */
		case RMC_UTCH: {
			if (wdcnt == 2) {
				state = RMC_UTCM;
				rmc->Time.hour = mcs_asciiint_to_int(&wdbuf[0], 2);
				wdcnt = 0;
			}
			break;
		}
		case RMC_UTCM: {
			if (wdcnt == 2) {
				state = RMC_UTCS;
				rmc->Time.min = mcs_asciiint_to_int(&wdbuf[0], 2);
				wdcnt = 0;
			}
			break;
		}
		case RMC_UTCS: {
			if (instr[i] == ',') {
				state = RMC_STAT;
				rmc->Time.sec = mcs_asciifloat_to_float(&wdbuf[0], 6);
				wdcnt = 0;
			}
			break;
		}
		/*parse status */
		case RMC_STAT: {
			if (instr[i] == ',') {
				state = RMC_LATD;
				rmc->Status = wdbuf[0];
				wdcnt = 0;
			}
			break;
		}
		/*parse latitude */
		case RMC_LATD: {
			if (wdcnt == 2) {
				state = RMC_LATM;
				rmc->Latt.Degrees = mcs_asciiint_to_int(&wdbuf[0], 2);
				wdcnt = 0;
			}
			break;
		}
		case RMC_LATM: {
			if (instr[i] == ',') {
				state = RMC_NWIN;
				rmc->Latt.Minutes = mcs_asciifloat_to_float(&wdbuf[0], 7);
				wdcnt = 0;
			}
			break;
		}
		case RMC_NWIN: {
			if (instr[i] == ',') {
				state = RMC_LOND;
				rmc->Latt.NSIndicator = wdbuf[0];
				wdcnt = 0;
			}

			break;
		}
		/* parse longitude */
		case RMC_LOND: {
			if (wdcnt == 3) {
				state = RMC_LONM;
				rmc->Long.Degrees = mcs_asciiint_to_int(&wdbuf[0], 3);
				wdcnt = 0;
			}
			break;
		}
		case RMC_LONM: {
			if (instr[i] == ',') {
				state = RMC_EWIN;
				rmc->Long.Minutes = mcs_asciifloat_to_float(&wdbuf[0], 7);
				wdcnt = 0;
			}
			break;
		}
		case RMC_EWIN: {
			if (instr[i] == ',') {
				state = RMC_SPEED;
				rmc->Long.EWIndicator = wdbuf[0];
				wdcnt = 0;
			}

			break;
		}
		/*parse speed */
		case RMC_SPEED: {
			if (instr[i] == ',') {
				state = RMC_COURSE;
				rmc->Speed = mcs_asciifloat_to_float(&wdbuf[0], 4);
				wdcnt = 0;
			}

			break;
		}
		/*parse course */
		case RMC_COURSE: {
			if (instr[i] == ',') {
				state = RMC_DATEDD;
				rmc->Course = mcs_asciifloat_to_float(&wdbuf[0], wdcnt);
				wdcnt = 0;
			}

			break;
		}
		/*parse date */
		case RMC_DATEDD: {
			if (wdcnt == 2) {
				state = RMC_DATEMM;
				rmc->Date.day = mcs_asciiint_to_int(&wdbuf[0], 2);
				wdcnt = 0;
			}
			break;
		}
		case RMC_DATEMM: {
			if (wdcnt == 2) {
				state = RMC_DATEYY;
				rmc->Date.month = mcs_asciiint_to_int(&wdbuf[0], 2);
				wdcnt = 0;
			}
			break;
		}
		case RMC_DATEYY: {
			if (wdcnt == 2) {
				state = RMC_MODE;
				rmc->Date.year = mcs_asciiint_to_int(&wdbuf[0], 2) + 2000;
				wdcnt = 0;
			}
			break;
		}

		case RMC_MODE: {
			if (instr[i] == '*') {
				state = RMC_END;
				rmc->Mode = wdbuf[0];

				/* verify checksum */
				wdbuf[0] = instr[++i];
				wdbuf[1] = instr[++i];
				if (chksum == mcs_asciihex_to_int(&wdbuf[0],2)) {
					return pdPASS ;
				} else {
					return pdFAIL ;
				}
			}
			break;
		} //end case
		} //end switch
	} //end for
	return pdFAIL ;
} //end function

/* parse gll type gps messages */
uint32_t parse_gll_msg(GPSDATA_S* gll, uint8_t *instr) {

	GLLPARSE_STATES state;
	char wdbuf[7];
	uint8_t chksum;
	uint8_t wdcnt;
	uint8_t i;

	wdcnt = 0;
	chksum = 0;

	//	invalidate_GPSDATA_S_data(gll);

	/* check if data is valid first */
	if (instr[22] == 'V') {
		gll->Status = 'V';
		return pdFAIL ;
	}

	state = START;
	/* loop through each character in the string */
	for (i = 0; i < STRCNTMAX; i++) {

		/* increment word counter */
		wdbuf[wdcnt] = instr[i];
		if (wdcnt > WDCNTMAX) {
			wdcnt = 0;
		} else {
			wdcnt++;
		}

		/* perform check sum */
		if ((instr[i] != '$') && (instr[i] != '*')) {
			chksum ^= instr[i];
		}

		switch (state) {
		case START: {
			if (instr[i] == '$') {
				state = MID;
			}
			break;
		}

		case MID: {
			if (instr[i] == ',') {
				state = LATD;
				wdcnt = 0;
			}
			break;
		}

		case LATD: {
			if (wdcnt == 2) {
				state = LATM;
				gll->Latt.Degrees = mcs_asciiint_to_int(&wdbuf[0], 2);
				wdcnt = 0;
			}
			break;
		}

		case LATM: {
			if (instr[i] == ',') {
				state = NWIN;
				gll->Latt.Minutes = mcs_asciifloat_to_float(&wdbuf[0], 7);
				wdcnt = 0;
			}
			break;
		}
		case NWIN: {
			if (instr[i] == ',') {
				state = LOND;
				gll->Latt.NSIndicator = wdbuf[0];
				wdcnt = 0;
			}

			break;
		}

		case LOND: {
			if (wdcnt == 3) {
				state = LONM;
				gll->Long.Degrees = mcs_asciiint_to_int(&wdbuf[0], 3);
				wdcnt = 0;
			}
			break;
		}

		case LONM: {
			if (instr[i] == ',') {
				state = EWIN;
				gll->Long.Minutes = mcs_asciifloat_to_float(&wdbuf[0], 7);
				wdcnt = 0;
			}
			break;
		}

		case EWIN: {
			if (instr[i] == ',') {
				state = UTCH;
				gll->Long.EWIndicator = wdbuf[0];
				wdcnt = 0;
			}

			break;
		}
		case UTCH: {
			if (wdcnt == 2) {
				state = UTCM;
				gll->Time.hour = mcs_asciiint_to_int(&wdbuf[0], 2);
				wdcnt = 0;
			}
			break;
		}
		case UTCM: {
			if (wdcnt == 2) {
				state = UTCS;
				gll->Time.min = mcs_asciiint_to_int(&wdbuf[0], 2);
				wdcnt = 0;
			}
			break;
		}
		case UTCS: {
			if (instr[i] == ',') {
				state = STAT;
				gll->Time.sec = mcs_asciifloat_to_float(&wdbuf[0], 6);
				wdcnt = 0;
			}
			break;
		}
		case STAT: {
			if (instr[i] == ',') {
				state = MODE;
				gll->Status = wdbuf[0];
				wdcnt = 0;
			}
			break;
		}
		case MODE: {
			if (instr[i] == '*') {
				state = END;
				gll->Mode = wdbuf[0];

				/* verify checksum */
				wdbuf[0] = instr[++i];
				wdbuf[1] = instr[++i];
				if (chksum == mcs_asciihex_to_int(&wdbuf[0],2)) {
					return pdPASS ;
				} else {
					return pdFAIL ;
				}
			}
			break;
		} //end case
		} //end switch
	} //end for
	return pdFAIL ;
} //end function

/* disable all gps messages from coming in initalliy */
uint32_t disable_gps_msg(NMEAType querytype) {
	uint8_t atPSRF103[19] = "$PSRF103,N,0,0,1";

	/* put in query type */
	atPSRF103[9] = (uint8_t) (((uint8_t) querytype) + 48);
	/* append checksum */
	gen_and_append_checksum(&atPSRF103[0]);
	/* send command */
	send_string_to_gps(&atPSRF103[0], 19);
	/* send return carriage */
	send_new_line_string_for_gps();

	return pdPASS ;
}

/* flush incoming GPS UART stream for a given timeout */
static uint32_t flush_gps_responses(const uint32_t timeout) {

	portCHAR cByteRxed;
	uint32_t error_flag = pdPASS;

	/*set and start timer */
	g_gps_timer_expired_flag = FALSE_FLAG;
	if (xTimerStart(xGPSTimer, portMAX_DELAY) != pdPASS ) {
		error_flag = pdFAIL;
	}
	if ((error_flag == pdPASS) && (xTimerChangePeriod(xGPSTimer, timeout, portMAX_DELAY) != pdPASS)) {
		error_flag = pdFAIL;
	}
	if ((error_flag == pdPASS) && (xTimerStart(xGPSTimer, portMAX_DELAY) != pdPASS )) {
		error_flag = pdFAIL;
	}

	for (;;) {

		/* timed out waiting for this response */
		if (g_gps_timer_expired_flag == TRUE_FLAG) {
			error_flag = pdPASS;
			break;
		}

		//wait detect error or ok response
		if (pdTRUE == xSerialGetChar(&cByteRxed, ((portTickType) 0x07d0))) {
			UART_putChar(UART, cByteRxed);
		}
	}

	return error_flag;
}

/* parse incoming GPS UART stream query response */
uint16_t receive_gps_response(GPSDATA_S *rmcmsg, const uint32_t timeout,
		uint32_t enableprint) {
	uint16_t i;
	portCHAR cByteRxed;
	uint32_t error_flag = pdPASS;

	/*set and start timer */
	g_gps_timer_expired_flag = FALSE_FLAG;
	if (xTimerChangePeriod(xGPSTimer, timeout, portMAX_DELAY) != pdPASS ) {
		error_flag = pdFAIL;
	}
	if (xTimerStart(xGPSTimer, portMAX_DELAY) != pdPASS ) {
		error_flag = pdFAIL;
	}

	for (i = 0; i < STRCNTMAX; i++) {

		/* timed out waiting for this response */
		if (g_gps_timer_expired_flag == TRUE_FLAG) {
			error_flag = pdFAIL;
			break;
		}

		//wait detect error or ok response
		if (pdTRUE == xSerialGetChar(&cByteRxed, ((portTickType) 0x07d0))) {

			//print out characters
			if (enableprint == 1) {
				UART_putChar(UART, cByteRxed);
			}

			//break if return character found
			if ((cByteRxed == '\r') || (cByteRxed == '\n')) {
				/* if message is valid */
				error_flag = parse_rmc_msg(rmcmsg, &g_rx_buffer_for_gps_msg[0]);

				if (enableprint == 1) {
					UART_putChar(UART, '\r');
					UART_putChar(UART, '\n');
				}

				break;
			}
			g_rx_buffer_for_gps_msg[i] = cByteRxed;

		}
	}

	/* See if the error_flag indicates a failure, in which case invalidate the data */
	if(error_flag == pdFAIL)
		invalidate_GPSDATA_S_data(rmcmsg);

	return error_flag;

}

/* search for specified string on incoming GPS UART stream, break if found */
static uint8_t locate_string_in_gps_msg(const uint8_t* inA, uint8_t len,
		uint32_t timeout) {
	portCHAR cByteRxed;
	uint8_t counter = 0;
	uint32_t error_flag = pdPASS;

	/*set and start timer */
	g_gps_timer_expired_flag = FALSE_FLAG;
	if (xTimerChangePeriod(xGPSTimer, timeout, portMAX_DELAY) != pdPASS ) {
		error_flag = pdFAIL;
	}
	if (xTimerStart(xGPSTimer, portMAX_DELAY) != pdPASS ) {
		error_flag = pdFAIL;
	}

	for (;;) {

		/* timed out waiting for this response */
		if (g_gps_timer_expired_flag == TRUE_FLAG) {
			error_flag = pdFAIL;
			break;
		}

		if (pdTRUE == xSerialGetChar(&cByteRxed, ((portTickType) (0.5 * configTICK_RATE_HZ)))) {

			/*debug printout */
			UART_putChar(UART, cByteRxed);
			if (cByteRxed == inA[counter]) {
				counter++;
			} else {
				counter = 0;
			}
			if (counter == len) {
				error_flag = pdPASS;
				break;
			}
		}
	}

	return error_flag;
}

/* GPS timer callback function */
void prvGPSTimerCallback(TimerHandle_t xExpiredTimer) {
	g_gps_timer_expired_flag = TRUE_FLAG;
}

/*turn device on, get out of reset */
void power_on_gps(void) {
	/*delay for one second and then raise GPS_EN */
	g_gps_timer_expired_flag = FALSE_FLAG;
	if (xTimerChangePeriod(xGPSTimer, (1 * SYS_TICKS_IN_1_SEC), portMAX_DELAY) == pdPASS ) {
		if (xTimerStart(xGPSTimer, portMAX_DELAY) == pdPASS ) {
			for (;;) {
				if (g_gps_timer_expired_flag == TRUE_FLAG) {
					gioSetBit(GPS_ON_OFF_PORT, GPS_ON_OFF_PIN, 1);
					break;
				}
			}
		}
	}

	/*delay for 200 ms and then lower GPS_EN */
	if (xTimerStart(xGPSTimer, portMAX_DELAY) == pdPASS ) {
		for (;;) {
			if (g_gps_timer_expired_flag == TRUE_FLAG) {
				gioSetBit(GPS_ON_OFF_PORT, GPS_ON_OFF_PIN, 0);
				break;
			}
		}

	}

}

void reset_gps(void) {
	g_gps_timer_expired_flag = FALSE_FLAG;
	if (xTimerChangePeriod(xGPSTimer, (1 * SYS_TICKS_IN_1_SEC), portMAX_DELAY) == pdPASS ) {
		if (xTimerStart(xGPSTimer, portMAX_DELAY) == pdPASS ) {

		}
	}
	for (;;) {
		if (g_gps_timer_expired_flag == TRUE_FLAG) {
			break;
		}
	}
	/*delay for one second and then raise GPS_EN */
	g_gps_timer_expired_flag = FALSE_FLAG;
	if (xTimerChangePeriod(xGPSTimer, (1 * SYS_TICKS_IN_1_SEC), portMAX_DELAY) == pdPASS ) {
		if (xTimerStart(xGPSTimer, portMAX_DELAY) == pdPASS ) {
			for (;;) {
				if (g_gps_timer_expired_flag == TRUE_FLAG) {
					gioSetBit(GPS_ON_OFF_PORT, GPS_ON_OFF_PIN, 1);
					break;
				}
			}
		}
	}

	/*delay for 200 ms and then lower GPS_EN */
	if (xTimerStart(xGPSTimer, portMAX_DELAY) == pdPASS ) {
		for (;;) {
			if (g_gps_timer_expired_flag == TRUE_FLAG) {
				gioSetBit(GPS_ON_OFF_PORT, GPS_ON_OFF_PIN, 0);
				break;
			}
		}

	}
}

/* intialize gps module by disabling messages and flushing pipe*/
uint32_t init_gps(void) {
	//GPSBAUD_STATES gps_baud_state= CHECK_MCS_9600;

	/* Create generic GPS timer */
	xGPSTimer = xTimerCreate(
			"GPS generic Timer", /* Unique name */
				(3 * SYS_TICKS_IN_1_SEC), /* Period (# ticks) */
				pdFALSE, /* Auto reload timer? */
				"GPSTimer", /* Unique ID (so callback can link to several timers) */
				prvGPSTimerCallback /* Callback function (see above) */
		);
	if (xGPSTimer == ((TimerHandle_t)NULL_PTR) ) {
		return pdFAIL ;
	}

	/*make sure GPS is powered on */
	power_on_gps();

	/*Delay for one second after starting GPS */
	g_gps_timer_expired_flag = FALSE_FLAG;
	if (xTimerChangePeriod(xGPSTimer, (1 * SYS_TICKS_IN_1_SEC), portMAX_DELAY) == pdPASS )
		if (xTimerStart(xGPSTimer, portMAX_DELAY) == pdPASS ) { }

	for (;;) {
		if (g_gps_timer_expired_flag == TRUE_FLAG) {
			break;
		}
	}

	/* set all rates to zero */
	disable_gps_msg(RMC);
	disable_gps_msg(GLL);
	disable_gps_msg(GGA);
	disable_gps_msg(GSA);
	disable_gps_msg(GSV);
	disable_gps_msg(VTG);

	/* flush receive buffer after turning off messages */
	flush_gps_responses((3 * SYS_TICKS_IN_1_SEC));

	/* make sure uart is silent */
	if (locate_string_in_gps_msg(&g_at_gpgll_string[0], 5, 5000) == pdPASS ) {
		return pdFAIL ;
	}
	/* send a test command to make sure it receives it */
	else {
		send_nmea_query_msg(GLL);
		if (locate_string_in_gps_msg(&g_at_gpgll_string[0], 5, 3000) == pdPASS ) {
			return pdPASS ;
		} else {
			return pdFAIL ;
		}
	}

}

//BaseType_t gps_receive_data_message(queues_gps_output_t* p_gps_msg,
//		portTickType recv_delay) {
////	xQueueHandle queue = NULL;
//
//	/* Get handle to logging queue */
////	queue = queues_get_queue_handle(QUEUES_APP_GPS_OUTPUT);
//
//	/* Validate queue handle */
////	return xQueueReceive(queue, p_gps_msg, recv_delay);
//}

/* Copy data from pointer source to fill out destination */
void copy_GPSDATA_S_data(GPSDATA_S* destination, GPSDATA_S* source) {
	destination->Course = source->Course;
	destination->Mode = source->Mode;
	destination->Speed = source->Speed;
	destination->Status = source->Status;

	/* The contents */
	destination->Date.day = source->Date.day;
	destination->Date.month = source->Date.month;
	destination->Date.year = source->Date.year;

	destination->Latt.Degrees = source->Latt.Degrees;
	destination->Latt.Minutes = source->Latt.Minutes;
	destination->Latt.NSIndicator = source->Latt.NSIndicator;

	destination->Long.Degrees = source->Long.Degrees;
	destination->Long.Minutes = source->Long.Minutes;
	destination->Long.EWIndicator = source->Long.EWIndicator;

	destination->Time.hour = source->Time.hour;
	destination->Time.min = source->Time.min;
	destination->Time.sec = source->Time.sec;
}

/* Invalidte all data stored within destination */
void invalidate_GPSDATA_S_data(GPSDATA_S* destination) {
	destination->Course = GPS_INVALID_COURSE;
	destination->Mode = GPS_INVALID_MODE;
	destination->Speed = GPS_INVALID_SPEED;
	destination->Status = GPS_INVALID_STATUS;

	/* The contents */
	destination->Date.day = GPS_INVALID_DATE_DAY;
	destination->Date.month = GPS_INVALID_DATE_MONTH;
	destination->Date.year = GPS_INVALID_DATE_YEAR;

	destination->Latt.Degrees = GPS_INVALID_LATT_DEGREES;
	destination->Latt.Minutes = GPS_INVALID_LATT_MINUTES;
	destination->Latt.NSIndicator = GPS_INVALID_LATT_NSINDICATOR;

	destination->Long.Degrees = GPS_INVALID_LONG_DEGREES;
	destination->Long.Minutes = GPS_INVALID_LONG_MINUTES;
	destination->Long.EWIndicator = GPS_INVALID_LONG_EWINDICATOR;

	destination->Time.hour = GPS_INVALID_TIME_HOUR;
	destination->Time.min = GPS_INVALID_TIME_MIN;
	destination->Time.sec = GPS_INVALID_TIME_SEC;
}

boolean is_gps_good(GPSDATA_S gpsdat){
	return (gpsdat.Status != GPS_INVALID_STATUS) &&
			((gpsdat.Status != 0) && (gpsdat.Status == 'A'));
}

/* Recommended length of string is at least 13
 *
 * */
int generate_gps_string_Long(GPSDATA_S gpsdat, char* strptr, int strptr_len){
	/* Generate a generic GPS string */
	int i = 0;
	int j = 0;
	int k = 0;
	uint16_t scratch16 = 0;
	float scratchf = 0;
	int a = 0;
	float b = 0;

	char dat[GPS_RECOMMENDED_LONG_STR] = { 'L', 'L', 'L', 176, ' ', 'M', 'M', '.', 'm', 'm', 'm', 'm', 39, ' ', 'x' };

	if(gpsdat.Long.Degrees < 100)
		j = 1;

	scratch16 = gpsdat.Long.Degrees;
	/* Fill in dat[] */
	int figs = 3;
	for(k = 0; k < figs; k++){
		a = pow10uint16((figs - 1) - k);
		dat[k] = (uint8_t)(
				((scratch16 - (scratch16 % a)) % (a * 10)) / a
		) + 0x30;
	}

	scratchf = round_to_nearest_whole(gpsdat.Long.Minutes * 10000) / 10000;
	figs = 2;
	for(k = 0; k < figs; k++){
		b = pow10float((figs - 1) - k);
		dat[k + 5] = get_1s_character(scratchf / b);
	}

	figs = 4;
	for(k = 0; k < figs; k++){
		b = pow10float((k + 1) * -1);
		dat[k + 8] = get_1s_character(scratchf / b);
	}

	dat[GPS_RECOMMENDED_LONG_STR - 1] = gpsdat.Long.EWIndicator;

	while((i < strptr_len) && (j < GPS_RECOMMENDED_LONG_STR)){
		/* Generate a string with the coordinate data for the longitude */
		strptr[i] = dat[j];

		i++;
		j++;
	}

	return i;
}

int generate_gps_string_UTC(GPSDATA_S gpsdat, char* strptr, int strptr_len){
	/* Generate a generic GPS string */
	int i = 0;
	int j = 0;
	int k = 0;
	uint8_t scratch8 = 0;
	int a = 0;

	char dat[] = { 'X', 'X', ':', 'X', 'X', };

	scratch8 = gpsdat.Time.hour;
	/* Fill in dat[] */
	int figs = 2;
	for(k = 0; k < figs; k++){
		a = pow10uint16((figs - 1) - k);
		dat[k] = (uint8_t)(
				((scratch8 - (scratch8 % a)) % (a * 10)) / a
		) + 0x30;
	}

	scratch8 = gpsdat.Time.min;
	figs = 2;
	for(k = 0; k < figs; k++){
		a = pow10uint16((figs - 1) - k);
		dat[k + 3] = (uint8_t)(
				((scratch8 - (scratch8 % a)) % (a * 10)) / a
		) + 0x30;
	}

	while((i < strptr_len) && (j < GPS_RECOMMENDED_LONG_STR)){
		/* Generate a string with the coordinate data for the longitude */
		strptr[i] = dat[j];

		i++;
		j++;
	}

	return i;
}

/* Recommended length of string is at least 13
 *
 * */
int generate_gps_string_Latt(GPSDATA_S gpsdat, char* strptr, int strptr_len){
	/* Generate a generic GPS string */
	int i = 0;
	int j = 0;
	int k = 0;
	uint16_t scratch16 = 0;
	float scratchf = 0;
	int a = 0;
	float b = 0;

	char dat[GPS_RECOMMENDED_LATT_STR] = { 'L', 'L', 'L', 176, ' ', 'M', 'M', '.', 'm', 'm', 'm', 'm', 39, ' ', 'x' };

	if(gpsdat.Latt.Degrees < 100)
		j = 1;

	scratch16 = gpsdat.Latt.Degrees;
	/* Fill in dat[] */
	int figs = 3;
	for(k = 0; k < figs; k++){
		a = pow10uint16((figs - 1) - k);
		dat[k] = (uint8_t)(
				((scratch16 - (scratch16 % a)) % (a * 10)) / a
		) + 0x30;
	}

	scratchf = round_to_nearest_whole(gpsdat.Latt.Minutes * 10000) / 10000;
	figs = 2;
	for(k = 0; k < figs; k++){
		b = pow10float((figs - 1) - k);
		dat[k + 5] = get_1s_character(scratchf / b);
	}

	figs = 4;
	for(k = 0; k < figs; k++){
		b = pow10float((k + 1) * -1);
		dat[k + 8] = get_1s_character(scratchf / b);
	}

	dat[GPS_RECOMMENDED_LONG_STR - 1] = gpsdat.Latt.NSIndicator;

	while((i < strptr_len) && (j < GPS_RECOMMENDED_LONG_STR)){
		/* Generate a string with the coordinate data for the longitude */
		strptr[i] = dat[j];

		i++;
		j++;
	}

	return i;
}

int generate_gps_string_DATE(GPSDATA_S gpsdat, char* strptr, int strptr_len){
	/* Generate a generic GPS string */
	int i = 0;
	int j = 0;
	int k = 0;
	uint8_t scratch8 = 0;
	int a = 0;

	char dat[] = { 'D', 'D', 'M', 'M', 'M', 'Y', 'Y', 'Y', 'Y' };

	scratch8 = gpsdat.Date.day;
	/* Fill in dat[] */
	int figs = 2;
	for(k = 0; k < figs; k++){
		a = pow10uint16((figs - 1) - k);
		dat[k] = (uint8_t)(
				((scratch8 - (scratch8 % a)) % (a * 10)) / a
		) + 0x30;
	}

	get_month_string(gpsdat.Date, &dat[2]);

	uint16_t yr = ((uint16_t)gpsdat.Date.year) + 2000;
	/* Fill in dat[] */
	figs = 4;
	for(k = 0; k < figs; k++){
		a = pow10uint16((figs - 1) - k);
		dat[k + 5] = (uint8_t)(
				((yr - (yr % a)) % (a * 10)) / a
		) + 0x30;
	}

	while((i < strptr_len) && (j < GPS_RECOMMENDED_DATE_STR)){
		/* Generate a string with the coordinate data for the longitude */
		strptr[i] = dat[j];

		i++;
		j++;
	}

	return i;
}

void get_month_string(DATE dt, char* strptr){
	if(dt.month > 0 && dt.month <= 12){
		switch(dt.month){
		case 1:
			strptr[0] = 'J';
			strptr[1] = 'A';
			strptr[2] = 'N';
			break;
		case 2:
			strptr[0] = 'F';
			strptr[1] = 'E';
			strptr[2] = 'B';
			break;
		case 3:
			strptr[0] = 'M';
			strptr[1] = 'A';
			strptr[2] = 'R';
			break;
		case 4:
			strptr[0] = 'A';
			strptr[1] = 'P';
			strptr[2] = 'R';
			break;
		case 5:
			strptr[0] = 'M';
			strptr[1] = 'A';
			strptr[2] = 'Y';
			break;
		case 6:
			strptr[0] = 'J';
			strptr[1] = 'U';
			strptr[2] = 'N';
			break;
		case 7:
			strptr[0] = 'J';
			strptr[1] = 'U';
			strptr[2] = 'L';
			break;
		case 8:
			strptr[0] = 'A';
			strptr[1] = 'U';
			strptr[2] = 'G';
			break;
		case 9:
			strptr[0] = 'S';
			strptr[1] = 'P';
			strptr[2] = 'T';
			break;
		case 10:
			strptr[0] = 'O';
			strptr[1] = 'C';
			strptr[2] = 'T';
			break;
		case 11:
			strptr[0] = 'N';
			strptr[1] = 'O';
			strptr[2] = 'V';
			break;
		case 12:
			strptr[0] = 'D';
			strptr[1] = 'E';
			strptr[2] = 'C';
			break;
		default:
			break;
		}
	}
}





