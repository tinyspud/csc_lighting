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

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
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

	for(i = 0; i < BYTES_IN_RETURN_MAX; i++)
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
		// once display is activated must wait min 6.5ms before command will be accepted by the display
		/* check to see if the device is busy - if it isn't busy then you can try to move on
		 * otherwise wait for the timer to elapse */
		epaper_power_on_sequence();

		rtnval = EinkInitializingCOGDriver;
		break;
	case EinkInitializingCOGDriver:
		if(epaper_start_COG_driver() == pdFAIL){
			/* Discharge */
			discharge_epaper();
			rtnval = EinkError;
		}
		else
			rtnval = EinkIdle;

		break;
	case EinkLoading:
		/* Load scratch_screen rows into the epaper */
#ifdef ROTATE_DISPLAY
		for(i = (LINES_ON_SCREEN - 1); i >= 0; i--){
			wait_to_not_busy();
			uploadImageLine_pre_bitflip(dataconfig1_t, scratch_screen[i]);
			/* Clear out the scratch_screen line */
			clear_scratch_screen_line(i);
			wait_to_not_busy();
			temp = getCmdResponse(dataconfig1_t);
		}
#else
		for(i = 0; i < LINES_ON_SCREEN; i++){
			wait_to_not_busy();
			uploadImageLine_pre_bitflip(dataconfig1_t, scratch_screen[i], i);
			/* Clear out the scratch_screen line */
			clear_scratch_screen_line(i);
			wait_to_not_busy();
			temp = getCmdResponse(dataconfig1_t);
		}
#endif
		rtnval = EinkIdle;
		break;
	case EinkIdle:
		/* Do nothing */
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

uint32 getCmdResponse(spiDAT1_t dataconfig1_t) {
	uint32 value;
	// max 250
	uint16 dummysenddata[2] = {0U,0U};
	uint16 returnValue[2] = {0U,0U};

	//wait
	display_cs_on();
	waitDisplayDrver(EINK_CS_DELAY);
	spiTransmitAndReceiveData(EINK_SPIPORT, &dataconfig1_t, 2, dummysenddata, returnValue);
	waitDisplayDrver(EINK_CS_DELAY);
	display_cs_off();

	value = (uint32) returnValue[0];
	value = value << 8;
	value += (uint32) returnValue[1];

	//0X9000 IS COMMAND WAS SUCCESSFUL

	return value;
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

void uploadImageLine_pre_bitflip(spiDAT1_t dataconfig1_t, uint8 *displayBuf, int linenum) {

	uint16 i = 0, j = 0;
	uint8_t buff[BYTES_IN_1_LINE_TO_EPD] = { 0 };
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

	/* DUMMY - write black screen */
	for(i = 0; i < NUM_DATA_BYTES_FIRST; i++, j++)
		buff[j] = 0xFF;

	buff[i + ((DISPLAY_HEIGHT_PIXELS - linenum) >> 2)] = (0X03 << (linenum % 4));

	j += NUM_SCAN_BYTES;

	/* DUMMY - write black screen */
	for(i = 0; i < NUM_DATA_BYTES_SECOND; i++, j++)
		buff[j] = 0xFF;

	//	/* Create the first half (move from right to left) */
	//	for(i = 0; i < NUM_DATA_BYTES_FIRST; i++, j++)
	//		dummysenddata[j] = ((i * 8) + 2);
	//



	/* Upload the line to SPI */
	write_epaper_register(EINK_REG_IDX_WRITE_LINE, buff, BYTES_IN_1_LINE_TO_EPD);

	write_epaper_register_one_byte(EINK_REG_IDX_OE_CTRL, EINK_OE_CMD_DRIVE);
}

uint16 do_bit_flip_for_epd(uint8 bit_in){
	uint16 x = 0;

	/* do the bit flipping */
	x |= (uint16)(bit_in & 0x01U);  /* Bit 0 */
	x |= (uint16)((bit_in & 0x02U) >> 1 << 2);  /* Bit 1 */
	x |= (uint16)((bit_in & 0x04U) >> 2 << 4);  /* Bit 2 */
	x |= (uint16)((bit_in & 0x08U) >> 3 << 6);  /* Bit 3 */
	x |= (uint16)((bit_in & 0x10U) >> 4 << 1);  /* Bit 4 */
	x |= (uint16)((bit_in & 0x20U) >> 5 << 3);  /* Bit 5 */
	x |= (uint16)((bit_in & 0x40U) >> 6 << 5);  /* Bit 6 */
	x |= (uint16)((bit_in & 0x80U) >> 7 << 7);  /* Bit 7 */

	return x;
}

/* Send a single byte */
void write_epaper_register_one_byte(uint8_t regidx, uint8_t argument){
	static uint16_t send_data[1] = { 0 };

	send_data[0] = argument;

	write_epaper_register(regidx, send_data, 1);
}

void write_epaper_register(uint8_t regidx, uint8_t arguments[], uint8_t arg_len){
	/* This function assumes the user has formatted [arguments] properly
	 * and will use the reg idx length */

	int len = 0, i = 0;
	static uint16_t send_data[MAX_BYTES_PER_PACKET] = { 0 },
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
		spiTransmitAndReceiveData(EINK_SPIPORT, &dataconfig1_t, 2U, send_data, receive_data);
		waitDisplayDrver(EINK_CS_DELAY);
		display_cs_off();

		/* Send data */
		send_data[0] = MAKE_UINT8_T_INTO_UINT16_T_LSB(EINK_REG_WRITE);
		for(i = 0; i < arg_len; i++)
			send_data[i + 1] = arguments[i];

		/* Send write to register */
		display_cs_on();
		waitDisplayDrver(EINK_CS_DELAY);
		spiTransmitAndReceiveData_checking_busy(EINK_SPIPORT, &dataconfig1_t, arg_len + EINK_COMMAND_OVERHEAD, send_data, receive_data);
		waitDisplayDrver(EINK_CS_DELAY);
		display_cs_off();
	}
}

uint8_t read_epaper_register_single_byte(uint8_t regidx){
	static uint16_t rxval[1] = { 0 };

	rxval[0] = 0;

	if((get_eink_reg_len(regidx) == 1) && (read_epaper_register(regidx, &rxval) == 1)){
		return MAKE_UINT16_T_LSB_INTO_UINT8_T(rxval[0]);
	}
	return 0;
}


int read_epaper_register(uint8_t regidx, uint8_t * arguments){
	/* This function assumes the user has formatted [arguments] properly
	 * and will use the reg idx length */

	int len = 0, i = 0;
	static uint16_t send_data[MAX_BYTES_COMMAND_PACKET] = { 0 },
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
		spiTransmitAndReceiveData(EINK_SPIPORT, &dataconfig1_t, 2U, send_data, receive_data);
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
		spiTransmitAndReceiveData(EINK_SPIPORT, &dataconfig1_t, len + EINK_COMMAND_OVERHEAD, send_data, receive_data);
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
	spiTransmitAndReceiveData(EINK_SPIPORT, &dataconfig1_t, 2U, send_data, receive_data);
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

	spiTransmitAndReceiveData(EINK_SPIPORT, &dataconfig1_t, 3U, _dummysenddata, _returnValue);
	waitDisplayDrver(EINK_CS_DELAY);
	display_cs_off();
}

void displayResetPointer(spiDAT1_t dataconfig1_t) {
	uint16 _dummysenddata[3] = {0x0020,0x000D,0x0000};
	uint16 _returnValue[3] = {0U,0U};

	display_cs_on();
	waitDisplayDrver(EINK_CS_DELAY);
	spiTransmitAndReceiveData(EINK_SPIPORT, &dataconfig1_t, 3U, _dummysenddata, _returnValue);
	waitDisplayDrver(EINK_CS_DELAY);
	display_cs_off();
}

/* Wait [time] x 10 us */
/* TODO fix all magic values coming into here */
static void waitDisplayDrver(TickType_t time)
{
	TickType_t start_tick = xTaskGetTickCount();
	while((xTaskGetTickCount()) < (time + start_tick)) { }
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


