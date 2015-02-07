#ifndef __LIGHT_SENSOR_H
#define __LIGHT_SENSOR_h


#include "twi_master_config.h"
#include "twi_master.h"

#define REG_INT_STATUS				0x00
#define REG_MAIN_CFG				0x01
#define REG_AMB_CFG					0x02
#define REG_AMB_CLEAR_HIGH_BYTE		0x04
#define REG_AMB_CLEAR_LOW_BYTE		0x05
#define REG_AMB_RED_HIGH_BYTE		0x06
#define REG_AMB_RED_LOW_BYTE		0x07
#define REG_AMB_GREEN_HIGH_BYTE		0x08
#define REG_AMB_GREEN_LOW_BYTE		0x09
#define REG_AMB_BLUE_HIGH_BYTE		0x0A
#define REG_AMB_BLUE_LOW_BYTE		0x0B
#define REG_AMB_IR_HIGH_BYTE		0x0C
#define REG_AMB_IR_LOW_BYTE			0x0D
#define REG_AMB_IRCOMP_HIGH_BYTE	0x0E
#define REG_AMB_IRCOMP_LOW_BYTE		0x0F
#define REG_AMB_TEMP_HIGH_BYTE		0x12
#define REG_AMB_TEMP_LOW_BYTE		0x13

#define MAX_44008_ADDR				0x80

void init_light_sensor(void);

bool read_int_register(void);




#endif
