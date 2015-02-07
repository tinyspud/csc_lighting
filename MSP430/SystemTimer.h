/*
 *
 *
 *
 *
 * */


#ifndef __SYSTEMTIMING_H
#define __SYSTEMTIMING_H




#define MAX_NUM_FCNS	20

typedef void (*queueableFunctionPointer)(void);


#define SYS_TIMER_ONE_MSEC	(4)
#define SYS_TIMER_QUARTER_SEC	(1024)
#define SYS_TIMER_HALF_SEC	(2048)
#define SYS_TIMER_ONE_SEC	(4096)


#define SYS_TIMER_DELAY_N_SEC(n)	(uint32_t)(n * SYS_TIMER_ONE_SEC)


void init_system_timer();

uint16_t get_current_tick();

uint32_t GetCurrentLongTick();

void TryRegisterWithSysTimer(queueableFunctionPointer, uint32_t);

void CheckAndServiceFunctionPointers();

uint8_t ShouldSortFunctionPointers();

void SortFunctionPointers();

uint32_t TickToSec(uint32_t);

void start_system_timer();

#endif


