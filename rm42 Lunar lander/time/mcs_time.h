/*
 * mcs_time.h
 *
 *  Created on: Jan 29, 2014
 *      Author: swoosh
 */

#ifndef MCS_TIME_H_
#define MCS_TIME_H_

/*
 * .NET uses a 64-bit time value encoded with a 64-bit unsigned integer representing
 * ticks since midnight, janurary 1, 0001 and ends at  23:59:59.9999999, December 31, 9999
 *
 * 1 tick = 100 nanoseconds
 *
 * 2 most significant bits reprenst the kind, namely local, UTC or unspecified
 * */
#define DATETIME_MAX	((uint64_t) 3155378975999999999)
#define DATETIME_MIN	((uint64_t) 0)
#define DATETIME_TICK_MASK	((uint64_t) 0x3FFFFFFFFFFFFFFF)
#define DATETIME_KIND_UTC	((uint64_t) 0x4000000000000000)
#define DATETIME_KIND_LOC	((uint64_t) 0x8000000000000000)
#define DATETIME_KIND_UNS	((uint64_t) 0x0000000000000000)
#define DATETIME_KIND_MASK	((uint64_t) 0xC000000000000000)

//#define DATETIME_2014_01_01_MIDNIGHT	((uint64_t) 635241312000000000)
#define DATETIME_1984_01_01_MIDNIGHT	((uint64_t) 625773600000000000)
#define DATETIME_1_DAY		((uint64_t) 864000000000)
#define DATETIME_TICKS_IN_1_MSEC	((uint64_t) 10000)
#define DATETIME_TICKS_IN_1_SEC	((uint64_t) 10000000)


typedef struct {
	uint16_t years;
	uint8_t months;
	uint8_t days;
	uint32_t hours;
	uint32_t mins;
	uint32_t secs;

} MCS_CLOCK_S;



void get_system_clock(MCS_CLOCK_S* mcs_clock);
void set_system_clock(MCS_CLOCK_S mcs_clock);
uint32_t init_system_clock(void);

extern uint32_t ticks_since_last_sync;

/* for use in CANopen which is number of days since Jan 1, 1984 and milliseconds since
 * midnight */
uint16_t get_days_since_jan_1_1984(MCS_CLOCK_S);
uint32_t get_miliseconds_since_midnight(MCS_CLOCK_S);


/* For binary logging timestamp */
uint64_t GendotNETDateTime(MCS_CLOCK_S);

#endif /* MCS_TIME_H_ */
