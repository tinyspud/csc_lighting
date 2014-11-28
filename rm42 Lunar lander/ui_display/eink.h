/*
 * eink.h
 *
 *  Created on: Jan 2, 2014
 *      Author: skatsu
 */

#ifndef EINK_H_
#define EINK_H_

#include "spi.h"

//#define DEBUG_DISPLAY_CS_TIMING
#define DEBUG_DISPLAY_COMMAND

//measured at 10.6usec.  check with actually processor setting
#define EINK_CS_DELAY 140U

/*  */
#define RSP_GOOD			0x9000U
#define RSP_BAD_LEN			0x6700U
#define RSP_INVALID_LE		0x6C00
#define RSP_BAD_P1_2		0x6A00
#define RSP_NOT_SUPPORTED	0x6D00

#define MAX_PACKETS 60
#define MAX_BYTES_PER_PACKET 250

#define BYTES_IN_1_LINE 50
#define LINES_ON_SCREEN 300
#define BYTES_IN_1_LINE_PACKET BYTES_IN_1_LINE + 4
#define BYTES_IN_RETURN_MAX	255

#define ONE_TEXT_LINE 50

/* Value to wait until the timeout */
#define EINK_WAIT_TO_TIMEOUT 500



/* Init states */
#define EINK_UNINIT		0
#define EINK_GETDEV		1
#define EINK_LOADHD		3
#define EINK_LOADLN		4
#define EINK_REFRESH	5
#define EINK_WAITING	6
#define EINK_LOADED		7

#define EINK_WAIT_PERIOD 20
#define MAX_LOAD_ATTEMPTS 10

#define EINK_DPI	113

/* GPIO Ports Needed */
#define EINK_DISPLAY_EN_PORT 	spiPORT1
#define EINK_BUSY_PORT 			spiPORT1
#define EINK_CS_PORT			spiPORT1

/* GPIO pin number */
#define EINK_DISPLAY_EN_PIN 	1U /* spi1 EN - set to port out */
#define EINK_BUSY_PIN			3U /* spi1 CS3 - set to port in */
#define EINK_CS_PIN				2U /* spi1 CS0 - set to port out */


/* information for epd header arrays */
typedef struct epdArrayData
{
	int num_rows;
	int num_bytes_per_row;
}epdArrayData_t;

static bool __display_black_on_white;
static uint16 dummysenddata[BYTES_IN_1_LINE_PACKET];
static uint16 returnValue[BYTES_IN_RETURN_MAX];
static uint16 waiter;

/* scratch_screen is memory copy of what's queued to be transmitted to the
 * display.  This is stored in the non-bit-flipped version */
extern uint8 scratch_screen[LINES_ON_SCREEN][BYTES_IN_1_LINE];
static int l_current_row;

/* SPI port config data */
spiDAT1_t dataconfig1_t;

void init_display_buffers(void);

void uploadImageLine_post_bitflip(spiDAT1_t, uint16 *);

void uploadImageLine_pre_bitflip(spiDAT1_t, uint8 *);

void wait_to_not_busy(void);

void ui_display_get_current_screen(void);

void uploadImageFromHeader(spiDAT1_t, uint16 *);

void splash_sample(void);

void uploadArray(spiDAT1_t, uint8 *, int);

uint16 do_bit_flip_for_epd(uint8);

uint8 undo_bit_flip_for_epd(uint16);

void load_one_line(spiDAT1_t, uint8 *, int);

void ui_display_set_black_on_white();

void ui_display_set_white_on_black();

void display_cs_on(void);

void display_cs_off(void);

void display_en_on(void);

void display_en_off(void);

void blackout_screen(void);

void whiteout_screen(void);

int manage_eink(int);

void clear_scratch_screen_line(int);

void clear_scratch_screen(void);

void force_clear_scratch_screen(void);

static void waitDisplayDrver(uint32 time);

uint32 getCmdResponse(spiDAT1_t dataconfig1_t);

void displayUpdate(spiDAT1_t dataconfig1_t);

void uploadImageData(spiDAT1_t dataconfig1_t,uint16 *displayBuf);

void displayResetPointer(spiDAT1_t dataconfig1_t);

void uploadEpdHeader(spiDAT1_t dataconfig1_t);

uint32 getDevInfo(spiDAT1_t dataconfig1_t);

boolean is_whitespace(char);

boolean check_if_busy(void);

boolean timeout_to_not_busy(boolean);

boolean is_scratch_on_screen(void);

#endif /* EINK_H_ */
