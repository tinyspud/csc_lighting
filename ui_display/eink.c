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
 *  called scratch_screen.  It can be brought out of storage and
 *  used and manipulated onto instead of regenerating all that was
 *  created.
 *
 *  Jan 31, '14 - C Chock, IDG
 */

#include "sys_common.h"
#include "system.h"
#include "spi.h"
#include "vis.h"
#include "het.h"
#include "eink.h"


//spiREG1
#define EINK_SPIPORT 	spiREG1


/* scratch_screen is memory copy of what's been transmitted to the
 * display.  This is stored in the non-bit-flipped version */
uint8 scratch_screen[LINES_ON_SCREEN][BYTES_IN_1_LINE];

bool can_write_to_display(){
	return true;
}
/*
 * This will init all of the buffers that the display driver will use
 * to load.  There will be a local copy of the image stored in memory and
 * a working copy of the image that can be manipulated by the driver.
 * Upon a load, the local copy will be updated and the working copy is
 * cleared.
 */
void init_display_buffers(void) {
	__display_black_on_white = TRUE;

	int i = 0;
	int j = 0;
	for(i = 0; i < BYTES_IN_1_LINE_PACKET; i++)
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
}

/* Break code into blocks that can be executed on timer ticks by the RTOS -
 * advance the execution through blocks at states since the RM48 is so fast and the
 * chip takes a long time to respond to things */
int manage_eink(int state){
	int i;
	uint16 temp = 0x0000;
	boolean prev = false;

	int rtnval = state;
	switch(state){
	default:
	case EINK_UNINIT:
		// init spi port configuration parameters
		dataconfig1_t.CS_HOLD = FALSE;
		dataconfig1_t.WDEL    = TRUE;
		dataconfig1_t.DFSEL   = SPI_FMT_0;
		dataconfig1_t.CSNR    = 0xFE;

		display_cs_on();
		display_cs_off();

		display_en_on();
		rtnval = EINK_GETDEV;
		break;
	case EINK_GETDEV:
		// once display is activated must wait min 6.5ms before command will be accepted by the display
		/* check to see if the device is busy - if it isn't busy then you can try to move on
		 * otherwise wait for the timer to elapse */
		if(!check_if_busy()){
			temp =	getDevInfo(dataconfig1_t);
			waiter = 1;
			rtnval = (temp == RSP_GOOD) ? EINK_LOADHD : EINK_GETDEV;
		}
		break;
	case EINK_LOADHD:
		// reset pointer
		if(waiter > 0)
			waiter--;


		if(waiter < 1){
			temp = 0;
			prev = timeout_to_not_busy(true);
			if(prev){
				displayResetPointer(dataconfig1_t);
			}

			prev &= timeout_to_not_busy(prev);
			if(prev){
				temp = getCmdResponse(dataconfig1_t);
			}

			temp = 0;

			prev &= timeout_to_not_busy(prev);
			if(prev){
				uploadEpdHeader(dataconfig1_t);


				wait_to_not_busy();

				temp = getCmdResponse(dataconfig1_t);
			}
			rtnval= temp == RSP_GOOD ? EINK_LOADLN : EINK_LOADHD;
		}
		break;
	case EINK_LOADLN:
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
			uploadImageLine_pre_bitflip(dataconfig1_t, scratch_screen[i]);
			/* Clear out the scratch_screen line */
			clear_scratch_screen_line(i);
			wait_to_not_busy();
			temp = getCmdResponse(dataconfig1_t);
		}
#endif
		rtnval = EINK_REFRESH;
		break;
	case EINK_REFRESH:


		wait_to_not_busy();
		displayUpdate(dataconfig1_t);
		waiter = EINK_WAIT_PERIOD;
		rtnval = EINK_WAITING;
		break;
	case EINK_WAITING:


		waiter--;
		if(waiter < 1){
			wait_to_not_busy();
			temp = getCmdResponse(dataconfig1_t);
			rtnval = temp == RSP_GOOD ? EINK_LOADED : EINK_LOADHD;
			display_cs_off();
		}
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

void force_clear_scratch_screen(){
	int i = 0;
	int j = 0;
	for(i = 0; i < LINES_ON_SCREEN; i++)
		for(j = 0; j < BYTES_IN_1_LINE; j++)
			scratch_screen[i][j] = 0x00;
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

uint32 getDevInfo(spiDAT1_t dataconfig1_t) {
	// max 250
	dummysenddata[0] = 0x30;
	dummysenddata[1] = 0x01;
	dummysenddata[2] = 0x01;
	dummysenddata[3] = 0x00;
	uint16 dummysenddata2[30];
	uint16 returnValue2[30] = {0U,0U};


	uint8 i =0;

	for(i=0; i << 30; i++){
		returnValue2[i] = 0;
		dummysenddata2[i] = 0;

	}

	//wait
	display_cs_on();
	waitDisplayDrver(EINK_CS_DELAY);
	spiTransmitAndReceiveData(EINK_SPIPORT, &dataconfig1_t, 4, dummysenddata, returnValue);
	waitDisplayDrver(EINK_CS_DELAY);
	display_cs_off();
	waitDisplayDrver(7000);

	wait_to_not_busy();
	//	while(gioGetBit(EINK_BUSY_PORT, EINK_BUSY_PIN) != 1 && (i++ < 40)) {
	//		//do nothing wait while busy
	//		waitDisplayDrver(420);
	//	}

	display_cs_on();
	waitDisplayDrver(EINK_CS_DELAY);
	spiTransmitAndReceiveData(EINK_SPIPORT, &dataconfig1_t, 30, dummysenddata2, returnValue2);
	waitDisplayDrver(EINK_CS_DELAY);
	display_cs_off();


	//0X9000 IS COMMAND WAS SUCCESSFUL
	if(returnValue2[0] == 'M') {
		return RSP_GOOD;
	}

	return 0x00FF;
}

void display_cs_on(){
	gioSetBit(EINK_CS_PORT, EINK_CS_PIN, 0);
	gioSetBit(hetPORT1, 2, 1);
}

void display_cs_off(){
	gioSetBit(EINK_CS_PORT, EINK_CS_PIN, 1);
	gioSetBit(hetPORT1, 2, 0);
}

void display_en_on(){
	gioSetBit(EINK_DISPLAY_EN_PORT, EINK_DISPLAY_EN_PIN, 0);
}

void display_en_off(){
	gioSetBit(EINK_DISPLAY_EN_PORT, EINK_DISPLAY_EN_PIN, 1);
}

/* Deprecated */
void uploadImageData(spiDAT1_t dataconfig1_t,uint16 *displayBuf) {
	dummysenddata[0] = 0x20;
	dummysenddata[1] = 0x01;
	dummysenddata[2] = 0x00;
	dummysenddata[3] = 250U;
	uint16 i = 0;
	//uint16 temp = 0U;

	for(i = 0; i < 250  ; i++) {
		dummysenddata[i+4] = displayBuf[i];
	}

	//wait
	display_cs_on();
	waitDisplayDrver(EINK_CS_DELAY);
	spiTransmitAndReceiveData(EINK_SPIPORT, &dataconfig1_t, 254U, dummysenddata, returnValue);

	waitDisplayDrver(EINK_CS_DELAY);
	display_cs_off();
	i = 0;
	gioGetBit(EINK_BUSY_PORT, EINK_BUSY_PIN);
	// about 8uS should be sufficient
	waitDisplayDrver(300U);
}

///* Show test text and snoopy */
//void splash_sample(){
//	/* Set the write buffer to the sample image */
//	int i = 0;
//	int j = 0;
//	for(i = 0; i < __SAMPLE_IMAGE_NUM_ROWS; i++){
//		for(j = 0; j < __SAMPLE_IMAGE_NUM_BYTES; j++){
//			scratch_screen[i][j] = sample_image[i][j];
//		}
//	}
//}

void uploadImageFromHeader(spiDAT1_t dataconfig1_t,uint16 *displayBuf) {

	dummysenddata[0] = 0x20;
	dummysenddata[1] = 0x01;
	dummysenddata[2] = 0x00;
	dummysenddata[3] = BYTES_IN_1_LINE;
	uint16 i = 0;

	for(i = 0; i < BYTES_IN_1_LINE ; i++) {
		dummysenddata[i+4] = displayBuf[i];
	}

	//wait
	display_cs_on();
	waitDisplayDrver(EINK_CS_DELAY);
	spiTransmitAndReceiveData(EINK_SPIPORT, &dataconfig1_t, BYTES_IN_1_LINE_PACKET, dummysenddata, returnValue);

	waitDisplayDrver(EINK_CS_DELAY);
	display_cs_off();

	// about 8uS should be sufficient
	waitDisplayDrver(300U);
}

void uploadImageLine_post_bitflip(spiDAT1_t dataconfig1_t,uint16 *displayBuf) {

	dummysenddata[0] = 0x20;
	dummysenddata[1] = 0x01;
	dummysenddata[2] =  0x00;
	dummysenddata[3] = BYTES_IN_1_LINE;
	uint16 i = 0;

	for(i = 0; i < BYTES_IN_1_LINE ; i++) {
		dummysenddata[i+4] = __display_black_on_white ? displayBuf[i] : ~displayBuf[i];
		/* TODO:make copy to scratch_screen[][] here */
		scratch_screen[l_current_row][i] = undo_bit_flip_for_epd(dummysenddata[i+4]);
	}

	//wait
	display_cs_on();
	waitDisplayDrver(EINK_CS_DELAY);
	spiTransmitAndReceiveData(EINK_SPIPORT, &dataconfig1_t, BYTES_IN_1_LINE_PACKET, dummysenddata, returnValue);

	/* Only can load that many lines */
	if(l_current_row < LINES_ON_SCREEN)
		l_current_row++;

	waitDisplayDrver(EINK_CS_DELAY);
	display_cs_off();

	// about 8uS should be sufficient
	waitDisplayDrver(300U);
}

void uploadImageLine_pre_bitflip(spiDAT1_t dataconfig1_t, uint8 *displayBuf) {

	dummysenddata[0] = 0x20;
	dummysenddata[1] = 0x01;
	dummysenddata[2] =  0x00;
	dummysenddata[3] = BYTES_IN_1_LINE;
	uint16 i = 0;

#ifdef ROTATE_DISPLAY
	uint8_t endianflipper;
#endif

	for(i = 0; i < BYTES_IN_1_LINE ; i++) {
#ifdef ROTATE_DISPLAY
		endianflipper = (((displayBuf[BYTES_IN_1_LINE - i - 1] & 0x01U) << 7)) |
				(((displayBuf[BYTES_IN_1_LINE - i - 1] & 0x02U) << 5)) |
				(((displayBuf[BYTES_IN_1_LINE - i - 1] & 0x04U) << 3)) |
				(((displayBuf[BYTES_IN_1_LINE - i - 1] & 0x08U) << 1)) |
				(((displayBuf[BYTES_IN_1_LINE - i - 1] & 0x10U) >> 1)) |
				(((displayBuf[BYTES_IN_1_LINE - i - 1] & 0x20U) >> 3)) |
				(((displayBuf[BYTES_IN_1_LINE - i - 1] & 0x40U) >> 5)) |
				(((displayBuf[BYTES_IN_1_LINE - i - 1] & 0x80U) >> 7));

		dummysenddata[i+4] = __display_black_on_white ? do_bit_flip_for_epd(endianflipper) : ~do_bit_flip_for_epd(endianflipper);
#else
		dummysenddata[i+4] = __display_black_on_white ? do_bit_flip_for_epd(displayBuf[i]) : ~do_bit_flip_for_epd(displayBuf[i]);
#endif
		/* TODO:make copy to scratch_screen[][] here */
		scratch_screen[l_current_row][i] = displayBuf[i];
	}

	//wait
	display_cs_on();
	waitDisplayDrver(EINK_CS_DELAY);
	spiTransmitAndReceiveData(EINK_SPIPORT, &dataconfig1_t, BYTES_IN_1_LINE_PACKET, dummysenddata, returnValue);

	/* Only can load that many lines */
	if((l_current_row + 1) < LINES_ON_SCREEN)
		l_current_row++;

	waitDisplayDrver(EINK_CS_DELAY);
	display_cs_off();

	// about 8uS should be sufficient
	waitDisplayDrver(300U);
}
/* This will also perform the bit flips */
void uploadArray(spiDAT1_t dataconfig1_t, uint8 *displayBuf, int num_bytes_in_array) {

	/* Create a blank line that's the max message length + the 4 for the header information */
	uint16 dummysenddata[MAX_BYTES_PER_PACKET + 4] = {0x20, 0x01, 0x00, 0x00};
	uint16 i = 0;

	/* put in length to expect */
	dummysenddata[3] = num_bytes_in_array;

	for(i = 0; i < BYTES_IN_1_LINE ; i++) {
		dummysenddata[i+4] = do_bit_flip_for_epd(displayBuf[i]);
	}

	//wait
	display_cs_on();
	waitDisplayDrver(EINK_CS_DELAY);
	spiTransmitAndReceiveData(EINK_SPIPORT, &dataconfig1_t, num_bytes_in_array + 4, dummysenddata, returnValue);

	waitDisplayDrver(EINK_CS_DELAY);
	display_cs_off();

	// about 8uS should be sufficient
	waitDisplayDrver(300U);
}

void load_one_line(spiDAT1_t dataconfig1_t, uint8 *displayBuf, int num_bytes_in_array) {
	/* Create a blank line that's the max message length + the 4 for the header information */
	uint16 dummysenddata[MAX_BYTES_PER_PACKET + 4] = {0x20, 0x01, 0x00, 0x00};
	uint16 i = 0;

	/* put in length to expect */
	dummysenddata[3] = num_bytes_in_array;

	for(i = 0; i < BYTES_IN_1_LINE ; i++) {
		dummysenddata[i+4] = do_bit_flip_for_epd(displayBuf[i]);
	}

	//wait
	display_cs_on();
	waitDisplayDrver(EINK_CS_DELAY);
	spiTransmitAndReceiveData(EINK_SPIPORT, &dataconfig1_t, num_bytes_in_array + 4, dummysenddata, returnValue);

	waitDisplayDrver(EINK_CS_DELAY);
	display_cs_off();

	// about 8uS should be sufficient
	waitDisplayDrver(300U);
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

uint8 undo_bit_flip_for_epd(uint16 bit_in){
	uint8 x = 0;

	/* do the bit flipping */
	x |= (uint8)(bit_in & 0x01U);  /* Bit 0 */
	x |= (uint8)((bit_in & 0x02U) >> 1 << 2);  /* Bit 1 */
	x |= (uint8)((bit_in & 0x04U) >> 2 << 4);  /* Bit 2 */
	x |= (uint8)((bit_in & 0x08U) >> 3 << 6);  /* Bit 3 */
	x |= (uint8)((bit_in & 0x10U) >> 4 << 1);  /* Bit 4 */
	x |= (uint8)((bit_in & 0x20U) >> 5 << 3);  /* Bit 5 */
	x |= (uint8)((bit_in & 0x40U) >> 6 << 5);  /* Bit 6 */
	x |= (uint8)((bit_in & 0x80U) >> 7 << 7);  /* Bit 7 */

	return x;
}

void uploadEpdHeader(spiDAT1_t dataconfig1_t) {
	uint16 dummysenddata[30] = {0x20,0x01,0x00,16U,0x33, 0x01, 0x90, 0x01, 0x2C, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	//	uint16 returnValue[30];

	//wait
	display_cs_on();
	waitDisplayDrver(EINK_CS_DELAY);
	spiTransmitAndReceiveData(EINK_SPIPORT, &dataconfig1_t, 20U, dummysenddata, returnValue);
	waitDisplayDrver(EINK_CS_DELAY);
	display_cs_off();
	l_current_row = 0;
	waitDisplayDrver(300U);
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
static void waitDisplayDrver(uint32 time)
{
	while(time != 0)
		time--;
}

/* return true if port is busy, false if not busy
 * */
boolean check_if_busy(){
	return (gioGetBit(EINK_BUSY_PORT, EINK_BUSY_PIN)) == 0;
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
	//	gioSetBit(gioPORTA, 2, 1);
	while((gioGetBit(EINK_BUSY_PORT, EINK_BUSY_PIN) &
			gioGetBit(EINK_BUSY_PORT, EINK_BUSY_PIN))!= 1) {
		//do nothing wait while busy
		waitDisplayDrver(420);
		_nop();
	}
	//	gioSetBit(gioPORTA, 2, 0);
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


