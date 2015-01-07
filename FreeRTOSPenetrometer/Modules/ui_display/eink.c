/*
 * eink.c
 *
 *  Created on: Jan 2, 2014 *      Author: skatsu
 *
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  This file contains a lot of the low level graphics
 *  manipulation that's used to render things to the user.
 *  The user renders text to the screen by using bitmaps
 *  stored in vis.*.
 *
 *  Items are rendered to the screen by using an array
 *  that is the same dimensions as the screen (400 x 300)
 *  and characters/shapes/etc. are rendered onto that.  When
 *  the manage_eink timer is executed, it then loads the buffer
 *  (the entire screen) to the EINK display, then it refreshes
 *  the display.
 *
 *  A copy of what was transmitted is kept in a minipulatable state
 *  called current_screen.  It can be brought out of storage and
 *  used and manipulated onto instead of regenerating all that was
 *  created.
 *
 *  Jan 31, '14 - C Chock, IDG
 *
 *
 *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *  Modifying for use on pervisave displays 2.0" and 2.7" e apper displays
 *
 *  Modified for use on RM42 (smaller ARM) so the current screen parameter is
 *  deleted and the screen is only modified upon changes from anyone who wants
 *  display to UI
 *
 *  Modified Dec. 14, '14 - C Chock, Beaumont
 *  chris@beaumontdesign.com
 *
 */

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "os_mpu_wrappers.h"
#include "sys_common.h"
#include "system.h"
#include "spi.h"
#include "vis.h"
//#include "sample_image.h"
#include "het.h"
#include "eink.h"
#include "gio.h"
#include "DisplayTask.h"
#include "common.h"

/* scratch_screen is memory copy of what's been transmitted to the
 * display.  This is stored in the non-bit-flipped version */
uint8 scratch_screen[LINES_ON_SCREEN][BYTES_IN_1_LINE];

/*
 * This will init all of the buffers that the display driver will use
 * to load.  There will be a local copy of the image stored in memory and
 * a working copy of the image that can be manipulated by the driver.
 * Upon a load, the local copy will be updated and the working copy is
 * cleared.
 */
void init_display_buffers_and_pins(void) {
	__display_black_on_white = TRUE;

	int i = 0;
	int j = 0;
	for(i = 0; i < BYTES_IN_1_LINE_TO_EPD; i++)
		dummysenddata[i] = 0;

	for(i = 0; i < BYTES_IN_1_LINE_TO_EPD; i++)
		returnValue[i] = 0;

	waiter = EINK_WAIT_PERIOD;
	for(i = 0; i < LINES_ON_SCREEN; i++){
		for(j = 0; j < BYTES_IN_1_LINE; j++){
			/* Set the current screen to 0 */
			scratch_screen[i][j] = 0;
		}
	}

	/* Init the data config parameter too */
	dataconfig1_t.CS_HOLD = FALSE;
	dataconfig1_t.WDEL    = TRUE;
	dataconfig1_t.DFSEL   = SPI_FMT_0;
	dataconfig1_t.CSNR    = 0;

	/* Set all pins low */
	gioSetBit(EINK_PANEL_ON_PORT, EINK_PANEL_ON_PIN, 0);
	gioSetBit(EINK_CS_PORT, EINK_CS_PIN, 0);
	gioSetBit(EINK_RESET_PORT, EINK_RESET_PIN, 0);
	gioSetBit(EINK_BORDER_CTRL_PORT, EINK_BORDER_CTRL_PIN, 0); // border pin ignored on 2" display

	/* Set discharge low */
	gioSetBit(EINK_DISCHARGE_PORT, EINK_DISCHARGE_PIN, 0);
}



void epaper_power_on_sequence(){
	/* Power on sequence for G2 COG driver - taken from Pervasive Displays Doc
	 * #4P015-00 */
	/* Set discharge low */
	gioSetBit(EINK_DISCHARGE_PORT, EINK_DISCHARGE_PIN, 0);

	/* Set all pins low */
	gioSetBit(EINK_PANEL_ON_PORT, EINK_PANEL_ON_PIN, 0);
	gioSetBit(EINK_CS_PORT, EINK_CS_PIN, 0);
	gioSetBit(EINK_RESET_PORT, EINK_RESET_PIN, 0);
	gioSetBit(EINK_BORDER_CTRL_PORT, EINK_BORDER_CTRL_PIN, 0); // border pin ignored on 2" display

	/* Set SPI pins SCK and SI to SPI functionality */
	spiSetFunctional(EINK_SPI_PORT, EINK_SPI_PORT->PC0 | ((1U << SPI_PIN_SIMO) | (1U << SPI_PIN_CLK)));

	waitDisplayDrver(10 * (SYS_TICKS_IN_1_MS));

	/* Set CS, border, reset high */
	gioSetBit(EINK_PANEL_ON_PORT, EINK_PANEL_ON_PIN, 1);
	gioSetBit(EINK_CS_PORT, EINK_CS_PIN, 1);
	gioSetBit(EINK_RESET_PORT, EINK_RESET_PIN, 1);
	gioSetBit(EINK_BORDER_CTRL_PORT, EINK_BORDER_CTRL_PIN, 1); // border pin ignored on 2" display

	/* Wait 5 ms */
	waitDisplayDrver(5	 * (SYS_TICKS_IN_1_MS));

	/* After 5 ms delay, set reset low */
	gioSetBit(EINK_RESET_PORT, EINK_RESET_PIN, 0);

	/* Wait 5 ms */
	waitDisplayDrver(5 * (SYS_TICKS_IN_1_MS));

	/* After 5 ms delay, set reset back high */
	gioSetBit(EINK_RESET_PORT, EINK_RESET_PIN, 1);

	/* Wait 5 ms */
	waitDisplayDrver(5 * (SYS_TICKS_IN_1_MS));

	/* Now can use COG driver */
	return;
}

void epaper_power_off_sequence(){
	/* Power off sequence for G2 COG driver - taken from Pervasive Displays Doc
	 * #4P015-00 */
	uint8_t scratch_byte = 0;
	uint8_t num_tries = 0;

#ifdef USING_2_7_INCH_EPAPER
	/* This is called >= 25 ms after the data has been input */
	waitDisplayDrver(25 * (SYS_TICKS_IN_1_MS));

	/* Set border to 0 */
	gioSetBit(EINK_BORDER_CTRL_PORT, EINK_BORDER_CTRL_PIN, 0);

	/* Wait 200-300 ms - take 300ms to be safe */
	waitDisplayDrver(300 * (SYS_TICKS_IN_1_MS));

	/* Set border to 1 */
	gioSetBit(EINK_BORDER_CTRL_PORT, EINK_BORDER_CTRL_PIN, 1);

	/* Check DC/DC */
	scratch_byte = read_epaper_register_single_byte(EINK_REG_IDX_SELF_CHECK);

	/* Check it for breakage */
	if(MASK_AND_CHECK_VALUE(scratch_byte, EINK_SELF_CHECK_DCDC_MASK) != 0){
		/* Turn on latch reset */
		write_epaper_register_one_byte(EINK_REG_IDX_DRIVER_LATCH, EINK_DRIVER_LATCH_ON);

		/* Turn off OE */
		write_epaper_register_one_byte(EINK_REG_IDX_OE_CTRL, EINK_OE_CMD_TURN_OFF);

		/* Power off pos. charge pump */
		write_epaper_register_one_byte(EINK_REG_IDX_CHARGE_PUMP_CTRL, EINK_CHARGE_PUMP_POS_V_OFF);

		/* Power off charge pump Vcom */
		write_epaper_register_one_byte(EINK_REG_IDX_CHARGE_PUMP_CTRL, EINK_CHARGE_PUMP_VCOM_OFF);

		/* Turn off all charge pumps */
		write_epaper_register_one_byte(EINK_REG_IDX_CHARGE_PUMP_CTRL, EINK_CHARGE_PUMP_ALL_OFF);

		/* Turn off OSC */
		write_epaper_register_one_byte(EINK_REG_IDX_PWR_OSC_SETTING, EINK_PWR_OSC_OFF);

		/* Discharge internal */
		write_epaper_register_one_byte(EINK_REG_IDX_POWER_SETTING_1, EINK_PWR_1_DISCHARGE_INTERNAL);

		/* Delay >= 120 ms */
		waitDisplayDrver(120 * (SYS_TICKS_IN_1_MS));

		/* Turn off discharge internal */
		write_epaper_register_one_byte(EINK_REG_IDX_POWER_SETTING_1, EINK_PWR_1_DISCHARGE_INT_OFF);

		/* Set powers and signals = 0 (VCC, VDD, /RESET, /CS, SI, SCLK) */
		gioSetBit(EINK_PANEL_ON_PORT, EINK_PANEL_ON_PIN, 0);
		gioSetBit(EINK_CS_PORT, EINK_CS_PIN, 0);
		gioSetBit(EINK_RESET_PORT, EINK_RESET_PIN, 0);
		gioSetBit(EINK_BORDER_CTRL_PORT, EINK_BORDER_CTRL_PIN, 0);
		/* Need to configure SPI pins SI and SCLK to GIO, then turn low */
		spiSetFunctional(EINK_SPI_PORT, EINK_SPI_PORT->PC0 & ~((0U << SPI_PIN_SIMO) | (0U << SPI_PIN_CLK)));
		gioSetBit(EINK_SPI_GIO_PORT, SPI_PIN_SIMO, 0);
		gioSetBit(EINK_SPI_GIO_PORT, SPI_PIN_CLK, 0);

		for(num_tries = 0; num_tries < NUM_TRIES_TO_DO_EXT_DISCHARGE; num_tries++){
			/* Delay >= 10 ms */
			waitDisplayDrver(10 * (SYS_TICKS_IN_1_MS));

			/* External discharge = 1 */
			gioSetBit(EINK_DISCHARGE_PORT, EINK_DISCHARGE_PIN, 1);

			/* Delay >= 10 ms */
			waitDisplayDrver(10 * (SYS_TICKS_IN_1_MS));

			/* External discharge = 0 */
			gioSetBit(EINK_DISCHARGE_PORT, EINK_DISCHARGE_PIN, 0);
		}
		/* Finished */
	}
#else
#ifdef USING_2_0_INCH_EPAPER
#error IMPLEMENT (page 38 of 4P015-00)
	/* This is called >= 25 ms after the data has been input */
	waitDisplayDrver(25 * (SYS_TICKS_IN_1_MS));

	/* Set border to 0 */
	gioSetBit(EINK_BORDER_CTRL_PORT, EINK_BORDER_CTRL_PIN, 0);

	/* Wait 200-300 ms - take 300ms to be safe */
	waitDisplayDrver(300 * (SYS_TICKS_IN_1_MS));

	/* Set border to 1 */
	gioSetBit(EINK_BORDER_CTRL_PORT, EINK_BORDER_CTRL_PIN, 1);

	/* Check DC/DC */
	scratch_byte = read_epaper_register_single_byte(EINK_REG_IDX_SELF_CHECK);

	/* Check it for breakage */
	if(MASK_AND_CHECK_VALUE(scratch_byte, EINK_SELF_CHECK_DCDC_MASK) != 0){
		/* Turn on latch reset */
		write_epaper_register_one_byte(EINK_REG_IDX_DRIVER_LATCH, EINK_DRIVER_LATCH_ON);

		/* Turn off OE */
		write_epaper_register_one_byte(EINK_REG_IDX_OE_CTRL, EINK_OE_CMD_TURN_OFF);

		/* Power off pos. charge pump */
		write_epaper_register_one_byte(EINK_REG_IDX_CHARGE_PUMP_CTRL, EINK_CHARGE_PUMP_POS_V_OFF);

		/* Power off charge pump Vcom */
		write_epaper_register_one_byte(EINK_REG_IDX_CHARGE_PUMP_CTRL, EINK_CHARGE_PUMP_VCOM_OFF);

		/* Turn off all charge pumps */
		write_epaper_register_one_byte(EINK_REG_IDX_CHARGE_PUMP_CTRL, EINK_CHARGE_PUMP_ALL_OFF);

		/* Turn off OSC */
		write_epaper_register_one_byte(EINK_REG_IDX_PWR_OSC_SETTING, EINK_PWR_OSC_OFF);

		/* Discharge internal */
		write_epaper_register_one_byte(EINK_REG_IDX_POWER_SETTING_1, EINK_PWR_1_DISCHARGE_INTERNAL);

		/* Delay >= 120 ms */
		waitDisplayDrver(120 * (SYS_TICKS_IN_1_MS));

		/* Turn off discharge internal */
		write_epaper_register_one_byte(EINK_REG_IDX_POWER_SETTING_1, EINK_PWR_1_DISCHARGE_INT_OFF);

		/* Set powers and signals = 0 (VCC, VDD, /RESET, /CS, SI, SCLK) */
		gioSetBit(EINK_PANEL_ON_PORT, EINK_PANEL_ON_PIN, 0);
		gioSetBit(EINK_CS_PORT, EINK_CS_PIN, 0);
		gioSetBit(EINK_RESET_PORT, EINK_RESET_PIN, 0);
		/* Need to configure SPI pins SI and SCLK to GIO, then turn low */
		spiSetFunctional(EINK_SPI_PORT, EINK_SPI_PORT->PC0 & ~((0U << SPI_PIN_SIMO) | (0U << SPI_PIN_CLK)));
		gioSetBit(EINK_SPI_GIO_PORT, SPI_PIN_SIMO, 0);
		gioSetBit(EINK_SPI_GIO_PORT, SPI_PIN_CLK, 0);

		for(num_tries = 0; num_tries < NUM_TRIES_TO_DO_EXT_DISCHARGE; num_tries++){
			/* Delay >= 10 ms */
			waitDisplayDrver(10 * (SYS_TICKS_IN_1_MS));

			/* External discharge = 1 */
			gioSetBit(EINK_DISCHARGE_PORT, EINK_DISCHARGE_PIN, 1);

			/* Delay >= 10 ms */
			waitDisplayDrver(10 * (SYS_TICKS_IN_1_MS));

			/* External discharge = 0 */
			gioSetBit(EINK_DISCHARGE_PORT, EINK_DISCHARGE_PIN, 0);
		}
		/* Finished */
	}
#endif
#endif
}

void discharge_epaper(){
	/* Juset set power on to low wait and set discharge high */
	gioSetBit(EINK_PANEL_ON_PORT, EINK_PANEL_ON_PIN, 0);

	waitDisplayDrver((300 * (SYS_TICKS_IN_1_MS)));

	gioSetBit(EINK_DISCHARGE_PORT, EINK_DISCHARGE_PIN, 1);
}

/* Start EPaper COG (chip-on-glass) driver */
BaseType_t epaper_start_COG_driver(){
	uint8_t scratch_byte = 0;
	uint8_t ver[EINK_REG_IDX_1_LEN] = { EINK_SCREEN_BANNER };
	uint8_t num_tries = 0;

	/* Start with power on sequence, then wait for busy pin to go high */
	wait_to_not_busy();

	/* Check the COG ID */
	scratch_byte = MAKE_UINT16_T_LSB_INTO_UINT8_T(read_epaper_version());

	/* Check the version against what this code is written for */
	if(MASK_AND_CHECK_VALUE(scratch_byte, EINK_VERSION_MASK) != EINK_VERSION)
		return pdFAIL; /* version mismatch */

	/* epaper is correct version, go to next step - disable OE */
	write_epaper_register_one_byte(EINK_REG_IDX_OE_CTRL, EINK_OE_CMD_DISABLE);

	/* Read out the breakage */
	scratch_byte = read_epaper_register_single_byte(EINK_REG_IDX_SELF_CHECK);

	/* Check it for breakage */
	if(MASK_AND_CHECK_VALUE(scratch_byte, EINK_SELF_CHECK_BREAKAGE_MASK) != EINK_SELF_CHECK_BREAKAGE_MASK)
		return pdFAIL;

	/* Send into power saving mode */
	write_epaper_register_one_byte(EINK_REG_IDX_POWER_SAVING_MODE, EINK_PWR_SAVING_MODE_ENABLE);

	/* Set channel select */
	write_epaper_register(EINK_REG_IDX_SCREEN_SIZE, ver, EINK_REG_IDX_1_LEN);

	/* Set high power mode */
	write_epaper_register_one_byte(EINK_REG_IDX_PWR_OSC_SETTING, EINK_PWR_OSC_HIGH_POWER);

	/* Set power setting */
	write_epaper_register_one_byte(EINK_REG_IDX_POWER_SETTING_2, EINK_PWR_2_CMD);

	/* Set vcom level */
	write_epaper_register_one_byte(EINK_REG_IDX_VCOM_LEVEL, EINK_VCOM_CMD);

	/* Set other power setting */
	write_epaper_register_one_byte(EINK_REG_IDX_POWER_SETTING_1, EINK_PWR_1_CMD);

	/* Driver latch on */
	write_epaper_register_one_byte(EINK_REG_IDX_DRIVER_LATCH, EINK_DRIVER_LATCH_ON);

	/* Driver latch off */
	write_epaper_register_one_byte(EINK_REG_IDX_DRIVER_LATCH, EINK_DRIVER_LATCH_OFF);

	/* Delay >= 5 ms */
	waitDisplayDrver(5 * (SYS_TICKS_IN_1_MS));

	/* Start the charge pump */
	for(num_tries = 0; num_tries < MAX_TRIES_TO_USE_CHARGE_PUMP; num_tries++){
		/* Start the charge pump +V */
		write_epaper_register_one_byte(EINK_REG_IDX_CHARGE_PUMP_CTRL, EINK_CHARGE_PUMP_POS_V);

		/* Delay >= 240 ms */
		waitDisplayDrver(240 * (SYS_TICKS_IN_1_MS));

		/* Start the charge pump -V */
		write_epaper_register_one_byte(EINK_REG_IDX_CHARGE_PUMP_CTRL, EINK_CHARGE_PUMP_NEG_V);

		/* Delay >= 40 ms */
		waitDisplayDrver(40 * (SYS_TICKS_IN_1_MS));

		/* Start charge pump Vcom driver to ON */
		write_epaper_register_one_byte(EINK_REG_IDX_CHARGE_PUMP_CTRL, EINK_CHARGE_PUMP_VCOM_ON);

		/* Delay >= 40 ms */
		waitDisplayDrver(40 * (SYS_TICKS_IN_1_MS));

		/* Check DC/DC */
		scratch_byte = read_epaper_register_single_byte(EINK_REG_IDX_SELF_CHECK);

		/* Check it for breakage */
		if(MASK_AND_CHECK_VALUE(scratch_byte, EINK_SELF_CHECK_DCDC_MASK) == EINK_SELF_CHECK_DCDC_MASK){
			/* Output enable to disable */
			write_epaper_register_one_byte(EINK_REG_IDX_OE_CTRL, EINK_OE_CMD_DISABLE);

			return pdPASS;
		}

	}
	return pdFAIL;
}

#define NUM_WASH_CYCLES	4

/* Break code into blocks that can be executed on timer ticks by the RTOS -
 * advance the execution through blocks at states since the RM48 is so fast and the
 * chip takes a long time to respond to things */
einkstate_t manage_eink(einkstate_t state){
	int i;
	uint16 temp = 0x0000;
	boolean prev = false;

	einkstate_t rtnval = state;
	switch(state){
	default:
	case EinkUninitialized:
		/* Uninitialized E Ink display, power it on */
		init_display_buffers_and_pins();

		rtnval = EinkPoweringOn;
		break;
	case EinkPoweringOn:
		/* Set HET08 high */
		gioSetBit(hetPORT1, 8, 0);
		// once display is activated must wait min 6.5ms before command will be accepted by the display
		/* check to see if the device is busy - if it isn't busy then you can try to move on
		 * otherwise wait for the timer to elapse */
		epaper_power_on_sequence();

		rtnval = EinkInitializingCOGDriver;
		break;
	case EinkInitializingCOGDriver:
		if(epaper_start_COG_driver() == pdFAIL){
			/* Discharge */
			epaper_power_off_sequence();
			rtnval = EinkError;
		}
		else
			rtnval = EinkJustInitializedReadyForFirstFrame;

		break;
	case EinkJustInitializedReadyForFirstFrame:
	case EinkLoading:
		/* Load scratch_screen rows into the epaper */
#ifdef ROTATE_DISPLAY
		for(i = (LINES_ON_SCREEN - 1); i >= 0; i--){
			wait_to_not_busy();
			uploadImageLine_pre_bitflip(dataconfig1_t, scratch_screen[i]);
			/* Clear out the scratch_screen line */
			clear_scratch_screen_line(i);
			wait_to_not_busy();
		}
#else
		/* Load inverted image */
		for(temp = 0; temp < 2; temp++){
			for(i = 0; i < LINES_ON_SCREEN; i++){
				wait_to_not_busy();
				uploadImageLine_pre_bitflip(dataconfig1_t, scratch_screen[i], i, NegativeImage);
				wait_to_not_busy();
			}
		}
		waitDisplayDrver(400 * SYS_TICKS_IN_1_MS);

		for(temp = 0; temp < NUM_WASH_CYCLES; temp++){
			/* Load black frame */
			write_epaper_solid_flush(BlackScreenFlush);

			waitDisplayDrver(196 * SYS_TICKS_IN_1_MS);
		}
		/* Load white frame */
		for(temp = 0; temp < NUM_WASH_CYCLES; temp++){
			write_epaper_solid_flush(WhiteScreenFlush);
			waitDisplayDrver(196 * SYS_TICKS_IN_1_MS);
		}
		/* Load black frame */
		for(temp = 0; temp < NUM_WASH_CYCLES; temp++){
			write_epaper_solid_flush(BlackScreenFlush);
			waitDisplayDrver(196 * SYS_TICKS_IN_1_MS);
		}
		/* Load white frame */
		for(temp = 0; temp < NUM_WASH_CYCLES; temp++){
			write_epaper_solid_flush(WhiteScreenFlush);
			waitDisplayDrver(196 * SYS_TICKS_IN_1_MS);
		}
		/* Load image */
		for(temp = 0; temp < 3 * (NUM_WASH_CYCLES); temp++){
			for(i = 0; i < LINES_ON_SCREEN; i++){
				wait_to_not_busy();
				uploadImageLine_pre_bitflip(dataconfig1_t, scratch_screen[i], i, PositiveImage);
				/* Clear out the scratch_screen line */
				clear_scratch_screen_line(i);
				wait_to_not_busy();
			}
		}
		/* Load white frame */
		for(temp = 0; temp < NUM_WASH_CYCLES; temp++){
			write_epaper_solid_flush(NothingScreenFlush);
			waitDisplayDrver(196 * SYS_TICKS_IN_1_MS);
		}
#endif
		rtnval = EinkIdleAndOn;
		break;
	case EinkIdleAndOn:
		/* Turn it off */
		rtnval = EinkPoweringOff;
		break;
	case EinkPoweringOff:
		epaper_power_off_sequence();
		/* Set HET08 high */
		gioSetBit(hetPORT1, 8, 1);
		rtnval = EinkIdleAndOff;
		break;
	case EinkError:
		/* Do nothing - broken display */
		break;
	}
	return rtnval;
}

/* This clears the memory array that stores what will be transmitted to the
 * screen next */
void clear_scratch_screen(){
	int i = 0;
	for(i = 0; i < LINES_ON_SCREEN; i++)
		clear_scratch_screen_line(i);
}

void clear_scratch_screen_line(int line_number){
	if(can_write_to_display()){
		int i = 0;
		for(i = 0; i < BYTES_IN_1_LINE; i++){
			scratch_screen[line_number][i] = 0x00;
		}
	}
}

/* Set entire screen black (scratch_screen) */
void blackout_screen(){
	if(can_write_to_display()){
		/* Set every byte in scratch_screen to 0xFF */
		int i = 0;
		int j = 0;
		for(i = 0; i < LINES_ON_SCREEN; i++)
			for(j = 0; j < BYTES_IN_1_LINE; j++)
				scratch_screen[i][j] = 0xFF;
	}
}

/* Set entire screen white (scratch_screen) */
void whiteout_screen(){
	if(can_write_to_display()){
		/* Set every byte in scratch_screen to 0x00 */
		int i = 0;
		int j = 0;
		for(i = 0; i < LINES_ON_SCREEN; i++)
			for(j = 0; j < BYTES_IN_1_LINE; j++)
				scratch_screen[i][j] = 0x00;
	}
}

uint8_t get_eink_reg_len(uint16_t RegIdx){
	switch(RegIdx){
	case 0x01:
		return EINK_REG_IDX_1_LEN;
	case 0x02:
		return EINK_REG_IDX_2_LEN;
	case 0x03:
		return EINK_REG_IDX_3_LEN;
	case 0x04:
		return EINK_REG_IDX_4_LEN;
	case 0x05:
		return EINK_REG_IDX_5_LEN;
	case 0x07:
		return EINK_REG_IDX_7_LEN;
	case 0x08:
		return EINK_REG_IDX_8_LEN;
	case 0x09:
		return EINK_REG_IDX_9_LEN;
	case 0x0A:
		return EINK_REG_IDX_A_LEN;
	case 0x0B:
		return EINK_REG_IDX_B_LEN;
	case 0x0F:
		return EINK_REG_IDX_F_LEN;
	default:
		return 0;
	}
}

void display_cs_on(){
	gioSetBit(EINK_CS_PORT, EINK_CS_PIN, 0);
}

void display_cs_off(){
	gioSetBit(EINK_CS_PORT, EINK_CS_PIN, 1);
}

void uploadImageLine_pre_bitflip(spiDAT1_t dataconfig1_t, uint8 *displayBuf, int linenum, ScreenUploading_t screen){

	int i = 0, j = 0;
	uint8_t buff[BYTES_IN_1_LINE_TO_EPD] = { 0 };
	uint8_t byte_write_black = 0x00, byte_write_white = 0x00;

	/* Clear the buffer */
	for(i = 1; i < BYTES_IN_1_LINE_TO_EPD; i++)
		buff[i] = 0;

	/* Trivial statement but here for consistancy */
	buff[0] = EINK_BORDER_BYTE_VAL; /* Border byte */
	j++;
	//
	//#ifdef ROTATE_DISPLAY
	//	uint8_t endianflipper;
	//#endif
	//
	//	for(i = 0; i < BYTES_IN_1_LINE ; i++) {
	//#ifdef ROTATE_DISPLAY
	//		endianflipper = (((displayBuf[BYTES_IN_1_LINE - i - 1] & 0x01U) << 7)) |
	//				(((displayBuf[BYTES_IN_1_LINE - i - 1] & 0x02U) << 5)) |
	//				(((displayBuf[BYTES_IN_1_LINE - i - 1] & 0x04U) << 3)) |
	//				(((displayBuf[BYTES_IN_1_LINE - i - 1] & 0x08U) << 1)) |
	//				(((displayBuf[BYTES_IN_1_LINE - i - 1] & 0x10U) >> 1)) |
	//				(((displayBuf[BYTES_IN_1_LINE - i - 1] & 0x20U) >> 3)) |
	//				(((displayBuf[BYTES_IN_1_LINE - i - 1] & 0x40U) >> 5)) |
	//				(((displayBuf[BYTES_IN_1_LINE - i - 1] & 0x80U) >> 7));
	//
	//		dummysenddata[i+4] = __display_black_on_white ? do_bit_flip_for_epd(endianflipper) : ~do_bit_flip_for_epd(endianflipper);
	//#else
	//		dummysenddata[i+4] = __display_black_on_white ? do_bit_flip_for_epd(displayBuf[i]) : ~do_bit_flip_for_epd(displayBuf[i]);
	//#endif
	//	}

	switch(screen){
	case PositiveImage:
		byte_write_black = EINK_CMD_BLACK_BYTE & EINK_CMD_COLOR_BITS_MASK;
		byte_write_white = EINK_CMD_WHITE_BYTE & EINK_CMD_COLOR_BITS_MASK;
		break;
	case NegativeImage:
		byte_write_black = EINK_CMD_WHITE_BYTE & EINK_CMD_COLOR_BITS_MASK;
		byte_write_white = EINK_CMD_BLACK_BYTE & EINK_CMD_COLOR_BITS_MASK;
		break;
	case BlackScreenFlush:
	case WhiteScreenFlush:
	case NothingScreenFlush:
	default:
		byte_write_black = EINK_CMD_NOTHING_BYTE & EINK_CMD_COLOR_BITS_MASK;
		byte_write_white = EINK_CMD_NOTHING_BYTE & EINK_CMD_COLOR_BITS_MASK;
		break;
	}

	/* DUMMY - write black screen */
	for(i = (BYTES_IN_1_LINE - 1); (i >= 0); i--, j++)
		buff[j] = ((((displayBuf[i] & (0x80)) >> 7) == 0x01) ? byte_write_black : byte_write_white) |
		(((((displayBuf[i] & (0x20)) >> 5) == 0x01) ? byte_write_black : byte_write_white) << 2) |
		(((((displayBuf[i] & (0x08)) >> 3) == 0x01) ? byte_write_black : byte_write_white) << 4) |
		(((((displayBuf[i] & (0x02)) >> 1) == 0x01) ? byte_write_black : byte_write_white) << 6);

	buff[BYTES_IN_1_LINE + ((DISPLAY_HEIGHT_PIXELS - linenum - 1) >> 2) + 1] = (0X03 << ((linenum % 4) * 2));

	j += NUM_SCAN_BYTES;

	/* DUMMY - write black screen */
	for(i = 0; i < BYTES_IN_1_LINE; i++, j++)
		buff[j] = (((((displayBuf[i] & (0x40)) >> 6) == 0x01) ? byte_write_black : byte_write_white) << 6)|
		(((((displayBuf[i] & (0x10)) >> 4) == 0x01) ? byte_write_black : byte_write_white) << 4) |
		(((((displayBuf[i] & (0x04)) >> 2) == 0x01) ? byte_write_black : byte_write_white) << 2) |
		(((((displayBuf[i] & (0x01)) >> 0) == 0x01) ? byte_write_black : byte_write_white) << 0);

	/* Upload the line to SPI */
	write_epaper_register(EINK_REG_IDX_WRITE_LINE, buff, BYTES_IN_1_LINE_TO_EPD);

	write_epaper_register_one_byte(EINK_REG_IDX_OE_CTRL, EINK_OE_CMD_DRIVE);
}

/* Send a single byte */
void write_epaper_register_one_byte(uint8_t regidx, uint8_t argument){
	static uint8_t send_data[1] = { 0 };

	send_data[0] = argument;

	write_epaper_register(regidx, send_data, 1);
}

void write_epaper_register(uint8_t regidx, uint8_t arguments[], uint8_t arg_len){
	/* This function assumes the user has formatted [arguments] properly
	 * and will use the reg idx length */

	int len = 0, i = 0;
	uint16_t send_data[MAX_BYTES_PER_PACKET] = { 0 },
			receive_data[MAX_BYTES_PER_PACKET] = { 0 };

	for(i = 0; (i < MAX_BYTES_PER_PACKET); i++){
		if(i < MAX_BYTES_PER_PACKET){ send_data[i] = 0U; }
		if(i < MAX_BYTES_PER_PACKET){ receive_data[i] = 0U; }
	}

	len = get_eink_reg_len(regidx);

	if((len > 0) && (len >= arg_len)){
		/* Load in the command and reg index */
		send_data[0] = MAKE_UINT8_T_INTO_UINT16_T_LSB(EINK_REG_HEADER);
		send_data[1] = MAKE_UINT8_T_INTO_UINT16_T_LSB(regidx);

		/* Send write to register */
		display_cs_on();
		waitDisplayDrver(EINK_CS_DELAY);
		spiTransmitAndReceiveData(EINK_SPI_PORT, &dataconfig1_t, 2U, send_data, receive_data);
		waitDisplayDrver(EINK_CS_DELAY);
		display_cs_off();

		/* Send data */
		send_data[0] = MAKE_UINT8_T_INTO_UINT16_T_LSB(EINK_REG_WRITE);
		for(i = 0; i < arg_len; i++)
			send_data[i + 1] = arguments[i];

		/* Send write to register */
		display_cs_on();
		waitDisplayDrver(EINK_CS_DELAY);
		spiTransmitAndReceiveData_checking_busy(EINK_SPI_PORT, &dataconfig1_t, arg_len + EINK_COMMAND_OVERHEAD, send_data, receive_data);
		waitDisplayDrver(EINK_CS_DELAY);
		display_cs_off();
	}
}

uint8_t read_epaper_register_single_byte(uint8_t regidx){
	static uint8_t rxval[1] = { 0 };

	rxval[0] = 0;

	if((get_eink_reg_len(regidx) == 1) && (read_epaper_register(regidx, &rxval) == 1)){
		return *rxval;
	}
	return 0;
}


int read_epaper_register(uint8_t regidx, uint8_t * arguments){
	/* This function assumes the user has formatted [arguments] properly
	 * and will use the reg idx length */

	int len = 0, i = 0;
	uint16_t send_data[MAX_BYTES_COMMAND_PACKET] = { 0 },
			receive_data[MAX_BYTES_COMMAND_PACKET] = { 0 };

	for(i = 0; i < MAX_BYTES_COMMAND_PACKET; i++){
		send_data[i] = 0U;
		receive_data[i] = 0U;
	}

	len = get_eink_reg_len(regidx);

	if(len > 0){
		/* Load in the command and reg index */
		send_data[0] = MAKE_UINT8_T_INTO_UINT16_T_LSB(EINK_REG_HEADER);
		send_data[1] = MAKE_UINT8_T_INTO_UINT16_T_LSB(regidx);

		/* Send write to register */
		display_cs_on();
		waitDisplayDrver(EINK_CS_DELAY);
		spiTransmitAndReceiveData(EINK_SPI_PORT, &dataconfig1_t, 2U, send_data, receive_data);
		waitDisplayDrver(EINK_CS_DELAY);
		display_cs_off();

		/* Send data */
		send_data[0] = MAKE_UINT8_T_INTO_UINT16_T_LSB(EINK_REG_READ);
		receive_data[0] = 0;
		for(i = 0; i < len; i++){
			send_data[i + 1] = 0U;
			receive_data[i + 1] = 0U;
		}

		/* Send write to register */
		display_cs_on();
		waitDisplayDrver(EINK_CS_DELAY);
		spiTransmitAndReceiveData(EINK_SPI_PORT, &dataconfig1_t, len + EINK_COMMAND_OVERHEAD, send_data, receive_data);
		waitDisplayDrver(EINK_CS_DELAY);
		display_cs_off();

		/* Copy from receive_data into arguments */
		for(i = 0; i < len; i++)
			arguments[i] = receive_data[i + 1];
	}

	return len;
}

uint16_t read_epaper_version(){

	/* This function assumes the user has formatted [arguments] properly
	 * and will use the reg idx length */

	int len = 0, i = 0;
	uint16_t send_data[EINK_COG_ID_PACKET_LEN] = { 0 },
			receive_data[EINK_COG_ID_PACKET_LEN] = { 0 };

	for(i = 0; i < EINK_COG_ID_PACKET_LEN; i++){
		if(i < EINK_COG_ID_PACKET_LEN){ send_data[i] = 0U; }
		if(i < EINK_COG_ID_PACKET_LEN){ receive_data[i] = 0U; }
	}

	/* Load in the command and reg index */
	send_data[0] = MAKE_UINT8_T_INTO_UINT16_T_LSB(EINK_REG_COG_ID);
	send_data[1] = 0U;

	/* Send write to register */
	display_cs_on();
	waitDisplayDrver(EINK_CS_DELAY);
	spiTransmitAndReceiveData(EINK_SPI_PORT, &dataconfig1_t, 2U, send_data, receive_data);
	waitDisplayDrver(EINK_CS_DELAY);
	display_cs_off();

	return receive_data[1];
}

/*
 * Note: Method has internal delay on it that hangs until the busy pin reads not busy
 * */
void displayUpdate(spiDAT1_t dataconfig1_t) {
	uint16 _dummysenddata[3] = {0x0024,0x0001,0x0000};
	uint16 _returnValue[3] = {0U,0U};

	wait_to_not_busy();

	display_cs_on();
	waitDisplayDrver(EINK_CS_DELAY);

	spiTransmitAndReceiveData(EINK_SPI_PORT, &dataconfig1_t, 3U, _dummysenddata, _returnValue);
	waitDisplayDrver(EINK_CS_DELAY);
	display_cs_off();
}

void displayResetPointer(spiDAT1_t dataconfig1_t) {
	uint16 _dummysenddata[3] = {0x0020,0x000D,0x0000};
	uint16 _returnValue[3] = {0U,0U};

	display_cs_on();
	waitDisplayDrver(EINK_CS_DELAY);
	spiTransmitAndReceiveData(EINK_SPI_PORT, &dataconfig1_t, 3U, _dummysenddata, _returnValue);
	waitDisplayDrver(EINK_CS_DELAY);
	display_cs_off();
}

/* Wait [time] x 10 us */
/* TODO fix all magic values coming into here */
inline static void waitDisplayDrver(TickType_t time)
{
	TickType_t start_tick = xTaskGetTickCount();
//	while((xTaskGetTickCount()) < (time + start_tick)) { }
	vTaskDelayUntil(&start_tick, time);
}

/* return true if port is busy, false if not busy
 * */
boolean check_if_busy(){
	return (gioGetBit(EINK_BUSY_PORT, EINK_BUSY_PIN)) == EINK_BUSY_IS_BUSY;
}

/* returns false if busy and timed out, true if not busy
 * only executes if inval is true
 */
boolean timeout_to_not_busy(boolean inval){
	int timeoutval = EINK_WAIT_TO_TIMEOUT;
	boolean rtnval = false;
	while((!rtnval) & inval){
		/* Return once the pin read is true */
		rtnval = (gioGetBit(EINK_BUSY_PORT, EINK_BUSY_PIN) == 1);

		if(!rtnval){
			//do nothing wait while busy
			waitDisplayDrver(420);

			timeoutval--;
			if(timeoutval <= 0)
				break;
		}
	}
	return rtnval;
}

/* Stalls until the busy pin goes high (indicating not busy) */
void wait_to_not_busy(){
	while((gioGetBit(EINK_BUSY_PORT, EINK_BUSY_PIN) &
			gioGetBit(EINK_BUSY_PORT, EINK_BUSY_PIN)) == EINK_BUSY_IS_BUSY) {
		//do nothing wait while busy
		waitDisplayDrver(1);
		_nop();
	}
}

void ui_display_set_black_on_white(){
	__display_black_on_white = TRUE;
}

void ui_display_set_white_on_black(){
	__display_black_on_white = FALSE;
}

/* Check the char input to see if it's whitespace (line feed, new line, space or tab) */
boolean is_whitespace(char char2check){
	return (char2check == ' ') || (char2check == '\r') || (char2check == '\n') || (char2check == '\t');
}


void write_epaper_solid_flush(ScreenUploading_t screen){
	int i = 0;
	uint16_t send_data[MAX_BYTES_COMMAND_PACKET] = { 0 },
			receive_data[MAX_BYTES_COMMAND_PACKET] = { 0 };

	for(i = 0; i < MAX_BYTES_COMMAND_PACKET; i++){
		send_data[i] = 0U;
		receive_data[i] = 0U;
	}

	for(i = 0; i < DISPLAY_HEIGHT_PIXELS; i++){
		send_data[0] = EINK_REG_HEADER;
		send_data[1] = EINK_REG_IDX_WRITE_LINE;

		/* Send write to register */
		display_cs_on();
		waitDisplayDrver(EINK_CS_DELAY);
		spiTransmitAndReceiveData(EINK_SPI_PORT, &dataconfig1_t, 2U, send_data, receive_data);
		waitDisplayDrver(EINK_CS_DELAY);
		display_cs_off();

		display_cs_on();
		waitDisplayDrver(EINK_CS_DELAY);
		spiTransmit_solid_color_line_data(EINK_SPI_PORT, &dataconfig1_t, screen, i);
		waitDisplayDrver(EINK_CS_DELAY);
		display_cs_off();

		write_epaper_register_one_byte(EINK_REG_IDX_OE_CTRL, EINK_OE_CMD_DRIVE);
	}
}

/* Upload function specifically for the black/white screen draws */
uint32 spiTransmit_solid_color_line_data(spiBASE_t *spi, spiDAT1_t *dataconfig_t, ScreenUploading_t screen, int linenum)
{
	int blocksize = 0;
	uint16 Tx_Data, trashrx;
	uint32 Chip_Select_Hold = (dataconfig_t->CS_HOLD) ? 0x10000000U : 0U;
	uint32 WDelay = (dataconfig_t->WDEL) ? 0x04000000U : 0U;
	SPIDATAFMT_t DataFormat = dataconfig_t->DFSEL;
	uint8 ChipSelect = dataconfig_t->CSNR;

	Tx_Data = (screen == BlackScreenFlush) ? (EINK_CMD_BLACK_BYTE) :
			((screen == WhiteScreenFlush) ? EINK_CMD_WHITE_BYTE :
					((screen == NothingScreenFlush) ? EINK_CMD_NOTHING_BYTE : EINK_CMD_NOTHING_BYTE));
	int msglen = (BYTES_IN_1_LINE_TO_EPD + 1);
	while(blocksize < msglen)
	{
		if((spi->FLG & 0x000000FFU) !=0U)
		{
			break;
		}

		if(blocksize == (msglen - 1))
		{
			Chip_Select_Hold = 0U;
		}
		/*SAFETYMCUSW 45 D MR:21.1 <APPROVED> "Valid non NULL input parameters are only allowed in this driver" */

		if(blocksize == 0){
			Tx_Data = EINK_REG_WRITE;
		}
		else if(blocksize == 1){
			Tx_Data = EINK_BORDER_BYTE_VAL;
		}
		else if(blocksize < (NUM_DATA_BYTES_FIRST + EINK_BORDER_LEN + EINK_COMMAND_OVERHEAD)){
			Tx_Data = (screen == BlackScreenFlush) ? (EINK_CMD_BLACK_BYTE) :
					((screen == WhiteScreenFlush) ? EINK_CMD_WHITE_BYTE :
							((screen == NothingScreenFlush) ? EINK_CMD_NOTHING_BYTE : EINK_CMD_NOTHING_BYTE));
		}
		else if(blocksize < (NUM_DATA_BYTES_FIRST + NUM_SCAN_BYTES + EINK_BORDER_LEN + EINK_COMMAND_OVERHEAD)){
			Tx_Data = ((BYTES_IN_1_LINE + ((DISPLAY_HEIGHT_PIXELS - linenum - 1) >> 2) + 1) == (blocksize - (EINK_COMMAND_OVERHEAD))) ?
					(0X03 << ((linenum % 4) * 2)) : 0x00;
		}
		else{
			Tx_Data = (screen == BlackScreenFlush) ? (EINK_CMD_BLACK_BYTE) :
					((screen == WhiteScreenFlush) ? EINK_CMD_WHITE_BYTE :
							((screen == NothingScreenFlush) ? EINK_CMD_NOTHING_BYTE : EINK_CMD_NOTHING_BYTE));
		}

		spi->DAT1 =((uint32)DataFormat << 24U) |
				((uint32)ChipSelect << 16U) |
				(WDelay)           |
				(Chip_Select_Hold) |
				(uint32)Tx_Data;
		/*SAFETYMCUSW 28 D MR:NA <APPROVED> "Hardware status bit read check" */
		while((spi->FLG & 0x00000100U) != 0x00000100U)
		{
		} /* Wait */
		while((gioGetBit(EINK_BUSY_PORT, EINK_BUSY_PIN) &
				gioGetBit(EINK_BUSY_PORT, EINK_BUSY_PIN)) == EINK_BUSY_IS_BUSY) {
			//do nothing wait while busy
			_nop();
		}

		/*SAFETYMCUSW 45 D MR:21.1 <APPROVED> "Valid non NULL input parameters are only allowed in this driver" */
		trashrx = (uint16)spi->BUF;
		/*SAFETYMCUSW 567 S MR:17.1,17.4 <APPROVED> "Pointer increment needed" */

		blocksize++;
	}


	return (spi->FLG & 0xFFU);
}


