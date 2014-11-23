/*
 * gps.h
 *
 *  Created on: Dec 28, 2013
 *      Author: skatsu
 */

#ifndef GPS_H_
#define GPS_H_

/************************************************************
 * NMEA syntax parser
 *
 * This file will read in strings of NMEA format and parse
 * them into understandable information
 *
 ************************************************************/

#include <string.h>
#include <limits.h>
#include "sys_common.h"

// Read NMEA lines, parse them and pack them into an object that can make sense of everything
/*
 GGA     Time, position and fix type data
 GLL     Latitude, longitude, UTC time of position fix and status
 GSA     GPS receiver operating mode, satellites used in the position solution, and DOP values
 GSV     Number of GPS satellites in view satellite ID numbers, elevation, azimuth, & SNR values
 MSS     Signal-to-noise ratio, signal strength, frequency, and bit rate from a radio-beacon receiver
 RMC     Time, date, position, course and speed data
 VTG     Course and speed information relative to the ground
 ZDA     PPS timing message (synchronized to PPS)
 150     OK to send message
 151     GPS Data and Extended Ephemeris Mask
 152     Extended Ephemeris Integrity
 154     Extended Ephemeris ACK
 */



#define UART			scilinREG
#define GPS_BAUD_RATE	9600


/* GPS State */
typedef enum {
	OK,			/* 0: OK detected on last response */
	TIMEOUT, 	/* 2: Timed out on last response */
	ERROR,
	BUSY		/* 3: Waiting for GSM response */
} GPS_RESPONSE_STATE;




#ifndef NMEAType
typedef enum {
	GGA = 0,
	GLL = 1,
	GSA = 2,
	GSV = 3,
	MSS = 6,
	RMC = 4,
	VTG = 5,
	ZDA = 7,
	INVALID = 99
} NMEAType;
#endif


#ifndef Latitude
typedef struct {
	uint16_t Degrees;
	float Minutes;
	uint8_t NSIndicator;
} Latitude;
#endif

#ifndef Longitude
typedef struct {
	uint16_t Degrees;
	float Minutes;
	uint8_t EWIndicator;
} Longitude;
#endif



typedef enum {
	AUTONOMOUS = 0, DGPS = 1, DR = 2
} GPSMode;

typedef struct{
	uint8_t hour;
	uint8_t min;
	float 	sec;
}UTC;

typedef struct{
	uint8_t day;
	uint8_t month;
	uint16_t year;
}DATE;


typedef struct {
	Latitude Latt;
	Longitude Long;
	UTC Time;
	uint8_t Status;
	uint8_t Mode;
	DATE Date;
	float Speed;
	float Course;
} GPSDATA_S;




typedef enum {
	GPS_9600 = 0,
	GPS_38400,
	GPS_57600
} GPS_BAUD;

/* Define the invalid values for GPSDATA_S */
#define GPS_INVALID_COURSE	0
#define GPS_INVALID_MODE	'N' /* Heading */
#define GPS_INVALID_SPEED	0
#define GPS_INVALID_STATUS	'V'

#define GPS_INVALID_DATE_DAY	0
#define GPS_INVALID_DATE_MONTH	0
#define GPS_INVALID_DATE_YEAR	0

#define GPS_INVALID_LATT_DEGREES		91
#define GPS_INVALID_LATT_MINUTES		61
#define GPS_INVALID_LATT_NSINDICATOR	0

#define GPS_INVALID_LONG_DEGREES		181
#define GPS_INVALID_LONG_MINUTES		61
#define GPS_INVALID_LONG_EWINDICATOR	0

#define GPS_INVALID_TIME_HOUR	25
#define GPS_INVALID_TIME_MIN	61
#define GPS_INVALID_TIME_SEC	61

#define GPS_RECOMMENDED_LONG_STR	15
#define GPS_RECOMMENDED_LATT_STR	15
#define GPS_RECOMMENDED_DATE_STR	9


#define GPS_1PPS_PORT		hetPORT
#define GPS_LOCKIND_PORT	hetPORT
#define GPS_RFPWRUP_PORT	hetPORT
#define GPS_ON_OFF_PORT		hetPORT

#define GPS_1PPS_PIN	14
#define GPS_LOCKIND_PIN	16
#define GPS_RFPWRUP_PIN	18
#define GPS_ON_OFF_PIN	22


/************************************************************
 * int ParseLongLatt(char* strin, LongLat *ll)
 * strin 	| String to parse ("$GPGLL,2503.6319,N,12136.0099,E,053740.000,A,A*52")
 * *ll 		| LongLatt struct
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * int 		| 0: Completed no problem, ll packed successfully
 * 			| 1: Completed with problems, don't trust ll
 ************************************************************/
uint32_t parse_gll_msg(GPSDATA_S* gll, uint8_t *instr);
uint32_t parse_rmc_msg(GPSDATA_S* rmc, uint8_t *instr);


/************************************************************
 * string send_nmea_query_msg(NMEAType)
 ************************************************************/
uint16_t send_nmea_query_msg(NMEAType);




//uint8_t mcs_asciihex_to_int(uint8_t*);


uint32_t init_gps(void);

uint16_t receive_gps_response(GPSDATA_S *gllmsg, const uint32_t timeout, uint32_t enableprint);

void copy_GPSDATA_S_data(GPSDATA_S*, GPSDATA_S*);

void invalidate_GPSDATA_S_data(GPSDATA_S*);

boolean is_gps_good(GPSDATA_S);

int generate_gps_string_Long(GPSDATA_S, char*, int);

int generate_gps_string_Latt(GPSDATA_S, char*, int);

int generate_gps_string_UTC(GPSDATA_S, char*, int);

int generate_gps_string_DATE(GPSDATA_S, char*, int);

void get_month_string(DATE, char*);


void print_gps_data_struct(GPSDATA_S* gps_rx_struct);

#endif /* GPS_H_ */
