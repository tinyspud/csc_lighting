#include "Common.h"
#include <stdbool.h>
#include <stdint.h>

#include "LightSensor.h"
#include "I2CInterface.h"

uint8_t scratch = 0;

#define AMB_BUFF_LEN	120

uint16_t R_buffer[AMB_BUFF_LEN];
uint16_t G_buffer[AMB_BUFF_LEN];
uint16_t B_buffer[AMB_BUFF_LEN];
uint16_t IR_buffer[AMB_BUFF_LEN];
uint16_t T_buffer[AMB_BUFF_LEN];

MAX44008_indexer_t R_indexer;
MAX44008_indexer_t G_indexer;
MAX44008_indexer_t B_indexer;
MAX44008_indexer_t IR_indexer;
MAX44008_indexer_t T_indexer;

static uint8_t config_state = 0;

void init_light_sensor(){
	int i = 0;
	for(i = 0; i < AMB_BUFF_LEN; i++){
		R_buffer[i] = 0;
		G_buffer[i] = 0;
		B_buffer[i] = 0;
		IR_buffer[i] = 0;
		T_buffer[i] = 0;
	}

	R_indexer.curval = GRAB_HIGH_BYTE;
	R_indexer.index = 0;
	G_indexer.curval = GRAB_HIGH_BYTE;
	G_indexer.index = 0;
	B_indexer.curval = GRAB_HIGH_BYTE;
	B_indexer.index = 0;
	IR_indexer.curval = GRAB_HIGH_BYTE;
	IR_indexer.index = 0;
	T_indexer.curval = GRAB_HIGH_BYTE;
	T_indexer.index = 0;

	/* Init I2C */
	init_i2c_ucb();

	/* Set the /INT pin */
	P4DIR &= ~BIT0;
	/* Read the interrupt register */
	read_int_register();

}

void configure_MAX44008(){
	switch(config_state){
	default:
	case 0:
		/* Enable all channels */
		scratch = MAIN_CFG_MODE_CLEAR_IR_RGB;
		coproc_write(REG_MAIN_CFG, &scratch, configure_MAX44008);
		config_state = 1;
		break;
	case 1:
		/* Enable all channels */
		scratch = AMB_CFG_AMBTIM_100_MS_14_BIT | AMB_CFG_TEMPEN_MASK;
		coproc_write(REG_MAIN_CFG, &scratch, read_R_val);
		break;
	}
}

void read_int_register(){
	/* Configure the main and ambient registers */
	config_state = 0;

	/* Read the INT register */
	coproc_read_to_buff(REG_INT_STATUS, &scratch, NULL, configure_MAX44008);
}

void read_R_val(){
	switch(R_indexer.curval){
	default:
	case GRAB_HIGH_BYTE:
		coproc_read_to_buff(REG_AMB_RED_HIGH_BYTE, &scratch, NULL, read_R_val);
		R_indexer.curval = GRAB_LOW_BYTE;
		break;
	case GRAB_LOW_BYTE:
		R_buffer[R_indexer.index] = (uint16_t)(((uint16_t)scratch) << 8);
		coproc_read_to_buff(REG_AMB_RED_LOW_BYTE, &scratch, NULL, read_R_val);
		R_indexer.curval = STORE_AND_INCREMENT;
		break;
	case STORE_AND_INCREMENT:
		R_indexer.curval = GRAB_HIGH_BYTE;
		R_indexer.index++;
		if(R_indexer.index >= AMB_BUFF_LEN)
			R_indexer.index = 0;

		TryRegisterWithSysTimer(read_G_val, 1);
		break;
	}
}

void read_G_val(){
	switch(G_indexer.curval){
	default:
	case GRAB_HIGH_BYTE:
		coproc_read_to_buff(REG_AMB_GREEN_HIGH_BYTE, &scratch, NULL, read_G_val);
		G_indexer.curval = GRAB_LOW_BYTE;
		break;
	case GRAB_LOW_BYTE:
		G_buffer[G_indexer.index] = (uint16_t)(((uint16_t)scratch) << 8);
		coproc_read_to_buff(REG_AMB_GREEN_LOW_BYTE, &scratch, NULL, read_G_val);
		G_indexer.curval = STORE_AND_INCREMENT;
		break;
	case STORE_AND_INCREMENT:
		G_indexer.curval = GRAB_HIGH_BYTE;
		G_indexer.index++;
		if(G_indexer.index >= AMB_BUFF_LEN)
			G_indexer.index = 0;

		TryRegisterWithSysTimer(read_B_val, 1);
		break;
	}
}

void read_B_val(){
	switch(B_indexer.curval){
	default:
	case GRAB_HIGH_BYTE:
		coproc_read_to_buff(REG_AMB_BLUE_HIGH_BYTE, &scratch, NULL, read_B_val);
		B_indexer.curval = GRAB_LOW_BYTE;
		break;
	case GRAB_LOW_BYTE:
		B_buffer[B_indexer.index] = (uint16_t)(((uint16_t)scratch) << 8);
		coproc_read_to_buff(REG_AMB_BLUE_LOW_BYTE, &scratch, NULL, read_B_val);
		B_indexer.curval = STORE_AND_INCREMENT;
		break;
	case STORE_AND_INCREMENT:
		B_indexer.curval = GRAB_HIGH_BYTE;
		B_indexer.index++;
		if(B_indexer.index >= AMB_BUFF_LEN)
			B_indexer.index = 0;

		TryRegisterWithSysTimer(read_T_val, 1);
		break;
	}
}

void read_T_val(){
	switch(T_indexer.curval){
	default:
	case GRAB_HIGH_BYTE:
		coproc_read_to_buff(REG_AMB_TEMP_HIGH_BYTE, &scratch, NULL, read_T_val);
		T_indexer.curval = GRAB_LOW_BYTE;
		break;
	case GRAB_LOW_BYTE:
		T_buffer[T_indexer.index] = (uint16_t)(((uint16_t)scratch) << 8);
		coproc_read_to_buff(REG_AMB_TEMP_LOW_BYTE, &scratch, NULL, read_T_val);
		T_indexer.curval = STORE_AND_INCREMENT;
		break;
	case STORE_AND_INCREMENT:
		T_indexer.curval = GRAB_HIGH_BYTE;
		T_indexer.index++;
		if(T_indexer.index >= AMB_BUFF_LEN)
			T_indexer.index = 0;

		TryRegisterWithSysTimer(read_IR_val, 1);
		break;
	}
}

void read_IR_val(){
	switch(IR_indexer.curval){
	default:
	case GRAB_HIGH_BYTE:
		coproc_read_to_buff(REG_AMB_IR_HIGH_BYTE, &scratch, NULL, read_IR_val);
		IR_indexer.curval = GRAB_LOW_BYTE;
		break;
	case GRAB_LOW_BYTE:
		IR_buffer[IR_indexer.index] = (uint16_t)(((uint16_t)scratch) << 8);
		coproc_read_to_buff(REG_AMB_IR_LOW_BYTE, &scratch, NULL, read_IR_val);
		IR_indexer.curval = STORE_AND_INCREMENT;
		break;
	case STORE_AND_INCREMENT:
		IR_indexer.curval = GRAB_HIGH_BYTE;
		IR_indexer.index++;
		if(IR_indexer.index >= AMB_BUFF_LEN)
			IR_indexer.index = 0;

		TryRegisterWithSysTimer(read_R_val, (100 * SYS_TIMER_ONE_MSEC));
		break;
	}
}






















