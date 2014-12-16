/*
 * eink.h
 *
 *  Created on: Jan 2, 2014
 *      Author: skatsu
 */

#ifndef EINK_H_
#define EINK_H_

#include "FreeRTOS.h"
#include "spi.h"


//#define USING_2_0_INCH_EPAPER
#define USING_2_7_INCH_EPAPER

//measured at 10.6usec.  check with actually processor setting
/* CS delay needs to be >= 10 us */
/* Delay is measured in ticks of 10 us so waiting 2 ticks is sufficient */
#define EINK_CS_DELAY 		2

#define EINK_REG_HEADER		0x70
#define EINK_REG_READ		0x73
#define EINK_REG_WRITE		0x72
#define EINK_REG_COG_ID		0x71

#define EINK_COMMAND_OVERHEAD	1

#define EINK_REG_IDX_1_LEN	8
#define EINK_REG_IDX_2_LEN	1
#define EINK_REG_IDX_3_LEN	1
#define EINK_REG_IDX_4_LEN	1
#define EINK_REG_IDX_5_LEN	1
#define EINK_REG_IDX_7_LEN	1
#define EINK_REG_IDX_8_LEN	1
#define EINK_REG_IDX_9_LEN	1
#define EINK_REG_IDX_A_LEN	111
#define EINK_REG_IDX_B_LEN	1
#define EINK_REG_IDX_F_LEN	1

#define EINK_2_0_INCH_COG_ID	0x00, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xE0, 0x00
#define EINK_2_7_INCH_COG_ID	0x00, 0x00, 0x00, 0x7F, 0xFF, 0xFE, 0x00, 0x00

#define EINK_REG_IDX_SCREEN_SIZE		0x01
#define EINK_REG_IDX_OE_CTRL			0x02
#define EINK_REG_IDX_DRIVER_LATCH		0x03
#define EINK_REG_IDX_POWER_SETTING_1	0x04
#define EINK_REG_IDX_CHARGE_PUMP_CTRL	0x05
#define EINK_REG_IDX_PWR_OSC_SETTING	0x07
#define EINK_REG_IDX_POWER_SETTING_2	0x08
#define EINK_REG_IDX_VCOM_LEVEL			0x09
#define EINK_REG_IDX_WRITE_LINE			0x0A
#define EINK_REG_IDX_POWER_SAVING_MODE	0x0B
#define EINK_REG_IDX_SELF_CHECK			0x0F

/* OE commands (0x02) */
#define EINK_OE_CMD_DISABLE	0x40
#define EINK_OE_CMD_DRIVE	0x07

/* Self check register */
#define EINK_SELF_CHECK_BREAKAGE_MASK	0x80
#define EINK_SELF_CHECK_DCDC_MASK		0x40

/* Power saving mode */
#define EINK_PWR_SAVING_MODE_ENABLE		0x02

/* Power oscillator setting */
#define EINK_PWR_OSC_HIGH_POWER			0xD1

/* Power setting 2 */
#define EINK_PWR_2_CMD					0x02

/* Vcom level */
#define EINK_VCOM_CMD					0xC2

/* Power setting 1 */
#define EINK_PWR_1_CMD					0x03

/* Latch */
#define EINK_DRIVER_LATCH_ON			0x01
#define EINK_DRIVER_LATCH_OFF			0x00

/* Charge pump control */
#define EINK_CHARGE_PUMP_POS_V			0x01
#define EINK_CHARGE_PUMP_NEG_V			0x03
#define EINK_CHARGE_PUMP_VCOM_ON		0x0F

/* Border byte */
#define EINK_BORDER_BYTE_VAL			0x00


#define MAX_TRIES_TO_USE_CHARGE_PUMP	4

#define MAX_PACKETS 60
#define MAX_BYTES_PER_REGIDX	111
#define MAX_BYTES_RECEIVED_PACKET	(2 + 1)
#define MAX_BYTES_COMMAND_PACKET	2
#define MAX_BYTES_PER_PACKET	(EINK_COMMAND_OVERHEAD + MAX_BYTES_PER_REGIDX)
#define EINK_COG_ID_PACKET_LEN	2

#define EINK_VERSION		0x02
#define EINK_VERSION_MASK	0x0F

/* Pixel color - only 2 bits active */
typedef enum PixelColor{
	PixelBlack = 0x03,		/* Black: 0b11 */
	PixelWhite = 0x02,		/* White: 0b10 */
	PixelNothing = 0x00,	/* Nothing: 0b00 */
}PixelColor_t;

/* Scan line - only 2 bits active */
typedef enum ScanLine{
	ScanActiveLine = 0x03,		/* Write this line: 0b11 */
	ScanInactiveLine = 0x00,	/* Not writing this line: 0b00 */
}ScanLine_t;


#ifdef USING_2_0_INCH_EPAPER
#define DISPLAY_WIDTH_PIXELS	100
#define DISPLAY_HEIGHT_PIXELS	96
#define BYTES_IN_RETURN_MAX	255
#define EINK_SCREEN_BANNER	EINK_2_0_INCH_COG_ID
#define EINK_DPI	111

#else
#ifdef USING_2_7_INCH_EPAPER

#define DISPLAY_WIDTH_PIXELS	264
#define DISPLAY_HEIGHT_PIXELS	176
#define BYTES_IN_RETURN_MAX	255
#define EINK_SCREEN_BANNER	EINK_2_7_INCH_COG_ID
#define EINK_DPI	117

#endif
#endif

#define NUM_BYTES_BORDER		1

#define BYTES_IN_1_LINE 		(DISPLAY_WIDTH_PIXELS / 8)
#define NUM_DATA_BYTES_FIRST	(DISPLAY_WIDTH_PIXELS / 8)
#define NUM_DATA_BYTES_SECOND	NUM_DATA_BYTES_FIRST
#define NUM_SCAN_BYTES			(LINES_ON_SCREEN / 4)
#define BYTES_IN_1_LINE_TO_EPD	(NUM_BYTES_BORDER + NUM_DATA_BYTES_FIRST + NUM_SCAN_BYTES + NUM_DATA_BYTES_SECOND)

/* Legacy support macro */
#define LINES_ON_SCREEN			(DISPLAY_HEIGHT_PIXELS)

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

typedef enum einkstate{
	EinkUninitialized,
	EinkPoweringOn,
	EinkInitializingCOGDriver,
	EinkIdle,
	EinkLoading,
	EinkError
}einkstate_t;

#define EINK_WAIT_PERIOD 20
#define MAX_LOAD_ATTEMPTS 10

/* GPIO Ports Needed */
#define EINK_SPIPORT 			spiREG1

#define EINK_BUSY_PORT 			gioPORTA
#define EINK_CS_PORT			spiPORT1
#define EINK_RESET_PORT			gioPORTA
#define EINK_PANEL_ON_PORT		gioPORTA
#define EINK_DISCHARGE_PORT		gioPORTA
#define EINK_BORDER_CTRL_PORT	gioPORTA


/* GPIO pin number */
#define EINK_BUSY_PIN			0U /* spi1 CS3 - set to port in */
#define EINK_CS_PIN				0U /* HET1 pin 17 - set to port out */
#define EINK_RESET_PIN			1U
#define EINK_PANEL_ON_PIN		6U
#define EINK_DISCHARGE_PIN		7U
#define EINK_BORDER_CTRL_PIN	4U

#define EINK_PWM_PIN			0U

#define EINK_BUSY_IS_BUSY		1U

/* information for epd header arrays */
typedef struct epdArrayData
{
	int num_rows;
	int num_bytes_per_row;
}epdArrayData_t;

static bool __display_black_on_white;
static uint16 dummysenddata[BYTES_IN_1_LINE_TO_EPD];
static uint16 returnValue[BYTES_IN_1_LINE_TO_EPD];
static uint16 waiter;

/* scratch_screen is memory copy of what's queued to be transmitted to the
 * display.  This is stored in the non-bit-flipped version */
extern uint8 scratch_screen[LINES_ON_SCREEN][BYTES_IN_1_LINE];
static int l_current_row;

/* SPI port config data */
spiDAT1_t dataconfig1_t;

void init_display_buffers_and_pins(void);

void uploadImageLine_post_bitflip(spiDAT1_t, uint16 *);

void uploadImageLine_pre_bitflip(spiDAT1_t dataconfig1_t, uint8 *displayBuf, int linenum);

void wait_to_not_busy(void);

void uploadImageFromHeader(spiDAT1_t, uint16 *);

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

einkstate_t manage_eink(einkstate_t);

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

void epaper_power_on_sequence();

BaseType_t epaper_start_COG_driver();

uint16_t read_epaper_version();

void discharge_epaper();

void write_epaper_register_one_byte(uint8_t, uint8_t);

int read_epaper_register(uint8_t regidx, uint8_t * arguments);

uint8_t read_epaper_register_single_byte(uint8_t regidx);

void write_epaper_register(uint8_t regidx, uint8_t arguments[], uint8_t arg_len);

#endif /* EINK_H_ */
