#ifndef __SYSTEM_TIMER_H
#define __SYSTEM_TIMER_H

#include <stdint.h>

#define MAX_NUM_FCNS	20

typedef void (*queueableFunctionPointer)(void);

#define LFCLK_FREQUENCY           (32768UL)                                 /**< LFCLK frequency in Hertz, constant. */
#define RTC_FREQUENCY             (8UL)                                     /**< Required RTC working clock RTC_FREQUENCY Hertz. Changable. */
#define COMPARE_COUNTERTIME       (3UL)                                     /**< Get Compare event COMPARE_TIME seconds after the counter starts from 0. */
#define COUNTER_PRESCALER         ((LFCLK_FREQUENCY / RTC_FREQUENCY) - 1)   /* f = LFCLK/(prescaler + 1) */

#define SYS_TIMER_ONE_MSEC	(4)
#define SYS_TIMER_QUARTER_SEC	(1024)
#define SYS_TIMER_HALF_SEC	(2048)
#define SYS_TIMER_ONE_SEC	(4096)

//#define LED_PWM_10KHZ_CC_VAL	(16000000/(1 << (NRF_TIMER1->PRESCALER)) / 10000)
#define LED_PWM_10KHZ_CC_VAL	(16000000/10000)


#define SYS_TIMER_DELAY_N_SEC(n)	(uint32_t)(n * SYS_TIMER_ONE_SEC)


void init_system_timer(void);

uint16_t get_current_tick(void);

uint32_t GetCurrentLongTick(void);

void TryRegisterWithSysTimer(queueableFunctionPointer, uint32_t);

void CheckAndServiceFunctionPointers(void);

uint8_t ShouldSortFunctionPointers(void);

void SortFunctionPointers(void);

uint32_t TickToSec(uint32_t);

void start_system_timer(void);


void system_timer_init(void);

void start_system_timer(void);

#endif
